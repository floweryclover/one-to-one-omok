#include <stdio.h>

#include "network.h"
#include "common.h"
#include "packet.h"

#define GAMESTATE_ERROR (-1)
#define GAMESTATE_NOTHING 0
#define GAMESTATE_BLACK_WIN 1
#define GAMESTATE_WHITE_WIN 2

int ProcessGame(SOCKET whose, SOCKET other, PlayerColor turn, PlayerColor board[][15]);

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		fprintf(stderr, "usage: %s <listen-address> <port> <server-name>", argv[0]);
		goto Cleanup_0;
	}

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result)
	{
		printf("WSAStartup() failed: %d", result);
		goto Cleanup_0;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Failed to create listenSocket: %d", WSAGetLastError());
		goto Cleanup_1;
	}
	SOCKADDR_IN hint;
	ZeroMemory(&hint, sizeof(SOCKADDR_IN));
	inet_pton(AF_INET, argv[1], &hint.sin_addr);
	hint.sin_port = htons(atoi(argv[2]));
	hint.sin_family = AF_INET;

	result = bind(listenSocket, (struct sockaddr*)&hint, sizeof(hint));
	if (result == SOCKET_ERROR)
	{
		printf("bind() failed: %d", WSAGetLastError());
		goto Cleanup_2;
	}

	result = listen(listenSocket, 5);
	if (result == SOCKET_ERROR)
	{
		printf("listen() failed: %d", WSAGetLastError());
		goto Cleanup_2;
	}

    PlayerNameRequest request;
    ZeroMemory(&request, sizeof(PlayerNameRequest));
    int receivedType;
    void* receivedPacketBody;
	SOCKADDR_IN blackSockAddr;
	int blackSockAddrSize = sizeof(blackSockAddr);
	printf("waiting for player-black...\n");
	SOCKET blackSocket = accept(listenSocket, (struct sockaddr*)&blackSockAddr, &blackSockAddrSize);
	if (blackSocket == INVALID_SOCKET)
	{
		printf("accept() failed(black): %d", WSAGetLastError());
		goto Cleanup_2;
	}
    if (SendOmokPacket(blackSocket, REQUEST_PLAYER_NAME, &request) <= 0)
    {
        fprintf(stderr, "흑돌 클라이언트에게 이름 입력을 요청할 수 없습니다: %d", WSAGetLastError());
        goto Cleanup_3;
    }
    if (ReceiveOmokPacket(blackSocket, &receivedType, &receivedPacketBody) <= 0)
    {
        fprintf(stderr, "흑돌 클라이언트에게서 이름 데이터를 받을 수 없습니다: %d", WSAGetLastError());
        goto Cleanup_3;
    }
    if (receivedType != RESPONSE_PLAYER_NAME)
    {
        fprintf(stderr, "흑돌 클라이언트에게서 이름 데이터를 받으려 했으나 다른 패킷이 도착했습니다.\n");
        goto Cleanup_3;
    }
    PlayerNameResponse blackPlayerNameResponse = *((PlayerNameResponse*)receivedPacketBody);
    free(receivedPacketBody);
	printf("Player-black joined: %s\n", blackPlayerNameResponse.name);
    PlayerColorPacket playerColorPacket;
    playerColorPacket.color = BLACK;
    if (SendOmokPacket(blackSocket, SET_PLAYER_COLOR, (void*)(&playerColorPacket)) <= 0)
    {
        fprintf(stderr, "흑돌 클라이언트에게 색상 데이터를 전송할 수 없습니다: %d\n", WSAGetLastError());
        goto Cleanup_3;
    }

	printf("waiting for player-white...\n");
	SOCKADDR_IN whiteSockAddr;
	int whiteSockAddrSize = sizeof(whiteSockAddr);
	SOCKET whiteSocket = accept(listenSocket, (struct sockaddr*)&whiteSockAddr, &whiteSockAddrSize);
	if (whiteSocket == INVALID_SOCKET)
	{
		printf("accept() failed(white): %d", WSAGetLastError());
		goto Cleanup_3;
	}
    if (SendOmokPacket(whiteSocket, REQUEST_PLAYER_NAME, &request) <= 0)
    {
        fprintf(stderr, "백돌 클라이언트에게 이름 입력을 요청할 수 없습니다: %d", WSAGetLastError());
        goto Cleanup_4;
    }
    if (ReceiveOmokPacket(whiteSocket, &receivedType, &receivedPacketBody) <= 0)
    {
        fprintf(stderr, "백돌 클라이언트에게서 이름 데이터를 받을 수 없습니다: %d", WSAGetLastError());
        goto Cleanup_4;
    }
    if (receivedType != RESPONSE_PLAYER_NAME)
    {
        fprintf(stderr, "백돌 클라이언트에게서 이름 데이터를 받으려 했으나 다른 패킷이 도착했습니다.\n");
        goto Cleanup_4;
    }
    PlayerNameResponse whitePlayerNameResponse = *((PlayerNameResponse*)receivedPacketBody);
    free(receivedPacketBody);
	printf("Player-white joined: %s\n", whitePlayerNameResponse.name);
    playerColorPacket.color = WHITE;
    if (SendOmokPacket(whiteSocket, SET_PLAYER_COLOR, (void*)(&playerColorPacket)) <= 0)
    {
        fprintf(stderr, "백돌 클라이언트에게 색상 데이터를 전송할 수 없습니다: %d\n", WSAGetLastError());
        goto Cleanup_4;
    }

    PlayerColor board[15][15];
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			board[i][j] = EMPTY;
		}
	}
	int remainCellCount = 15 * 15;

	printf("Game Start!\n\n");
    PlayerColor turn = BLACK;
	while (1)
	{
		result = ProcessGame((turn == BLACK ? blackSocket : whiteSocket), (turn == BLACK ? whiteSocket : blackSocket), turn, board);
		if (result == GAMESTATE_ERROR)
		{
			printf("An error occured\n");
			break;
		}
		else if (result == GAMESTATE_BLACK_WIN)
		{
			printf("Player-black win!\n");
			break;
		}
		else if (result == GAMESTATE_WHITE_WIN)
		{
			printf("Player-white win!\n");
			break;
		}
		else // GAMESTATE_NOTHING
		{
			if (remainCellCount <= 0)
			{
				printf("No winner!\n");
				break;
			}
			else
			{
				turn = turn == BLACK ? WHITE : BLACK;
				remainCellCount--;
			}
		}
	}

Cleanup_4:
    if (shutdown(whiteSocket, SD_SEND) == SOCKET_ERROR)
    {
        printf("whiteSocket shutdown() failed: %d", WSAGetLastError());
    }
    closesocket(whiteSocket);
Cleanup_3:
    if (shutdown(blackSocket, SD_SEND) == SOCKET_ERROR)
    {
        printf("blackSocket shutdown() failed: %d", WSAGetLastError());
    }
    closesocket(blackSocket);
Cleanup_2:
    closesocket(listenSocket);
Cleanup_1:
	WSACleanup();
Cleanup_0:
	return 0;
}

int ProcessGame(SOCKET whose, SOCKET other, PlayerColor turn, PlayerColor board[][15])
{
	printf("%s's turn\n", turn == BLACK ? "BLACK" : "WHITE");
    PlaceStoneRequest request;
    ZeroMemory(&request, sizeof(char));
    if (SendOmokPacket(whose, REQUEST_PLACE_STONE, (void*)(&request)) <= 0)
    {
        fprintf(stderr, "Failed to instruct player-%s to place a stone: %d\n", (turn == BLACK ? "black" : "white"), WSAGetLastError());
        return GAMESTATE_ERROR;
    }

    int receivedType;
	PlaceStoneResponse* response;
    if (ReceiveOmokPacket(whose, &receivedType, (void*)(&response)) <= 0)
    {
        fprintf(stderr, "Failed to receive a location from player-%s: %d\n", (turn == BLACK ? "black" : "white"), WSAGetLastError());
        return GAMESTATE_ERROR;
    }
    if (receivedType != RESPONSE_PLACE_STONE)
    {
        fprintf(stderr, "Failed to send the location of the stone placed this time to player-%s.\n", (turn == BLACK ? "black" : "white"));
        return GAMESTATE_ERROR;
    }

    int row = response->row;
    int column = response->column;
    free(response);
	if (row < 0 || row > 14 || column < 0 || column > 14)
	{
		printf("OUT OF BOARD ERROR\n");
		return GAMESTATE_ERROR;
	}

	if (board[row][column] != EMPTY)
	{
		printf("CELL OCCUPIED ERROR\n");
		return GAMESTATE_ERROR;
	}

	board[row][column] = turn;

    PlaceStoneResponse toApplyScreen;
    toApplyScreen.playerColor = turn;
    toApplyScreen.row = row;
    toApplyScreen.column = column;
    if (SendOmokPacket(whose, RESPONSE_PLACE_STONE, &toApplyScreen) <= 0)
    {
        fprintf(stderr, "%s에게 돌 업데이트 정보를 전송하는 데에 실패했습니다:%d\n", (turn == BLACK ? "흑돌" : "백돌"), WSAGetLastError());
        return GAMESTATE_ERROR;
    }
    if (SendOmokPacket(other, RESPONSE_PLACE_STONE, &toApplyScreen) <= 0)
    {
        fprintf(stderr, "%s에게 돌 업데이트 정보를 전송하는 데에 실패했습니다:%d\n", (turn == BLACK ? "백돌" : "흑돌"), WSAGetLastError());
        return GAMESTATE_ERROR;
    }

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15 - 5; j++)
		{
			if (board[i][j] == turn && board[i][j + 1] == turn && board[i][j + 2] == turn && board[i][j + 3] == turn && board[i][j + 4] == turn)
			{
				return (turn == BLACK ? GAMESTATE_BLACK_WIN : GAMESTATE_WHITE_WIN);
			}
		}
	}
	for (int i = 0; i < 15-5; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (board[i][j] == turn && board[i+1][j] == turn && board[i+2][j] == turn && board[i+3][j] == turn && board[i+4][j] == turn)
			{
				return (turn == BLACK ? GAMESTATE_BLACK_WIN : GAMESTATE_WHITE_WIN);
			}
		}
	}
	for (int i = 0; i < 15 - 5; i++)
	{
		for (int j = 0; j < 15 - 5; j++)
		{
			if (board[i][j] == turn && board[i + 1][j+1] == turn && board[i + 2][j+2] == turn && board[i + 3][j+3] == turn && board[i + 4][j+4] == turn)
			{
				return (turn == BLACK ? GAMESTATE_BLACK_WIN : GAMESTATE_WHITE_WIN);
			}

			if (board[i+4][j] == turn && board[i + 3][j + 1] == turn && board[i + 2][j + 2] == turn && board[i + 1][j + 3] == turn && board[i][j + 4] == turn)
			{
				return (turn == BLACK ? GAMESTATE_BLACK_WIN : GAMESTATE_WHITE_WIN);
			}
		}
	}

	return GAMESTATE_NOTHING;
}
