#include <stdio.h>

#include "network.h"
#include "common.h"

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

	SOCKADDR_IN blackSockAddr;
	int blackSockAddrSize = sizeof(blackSockAddr);
	printf("waiting for player-black...\n");
	SOCKET blackSocket = accept(listenSocket, (struct sockaddr*)&blackSockAddr, &blackSockAddrSize);
	if (blackSocket == INVALID_SOCKET)
	{
		printf("accept() failed(black): %d", WSAGetLastError());
		goto Cleanup_2;
	}

	char sendBuf[4];

	char blackPlayerName[MAX_PLAYER_NAME_LENGTH];
	ReceiveExact(blackSocket, MAX_PLAYER_NAME_LENGTH, blackPlayerName);
	printf("Player-black joined: %s\n", blackPlayerName);
    PlayerColor black = BLACK;
	memcpy(sendBuf, &black, 4);
	result = send(blackSocket, sendBuf, 4, 0);
	if (result == SOCKET_ERROR)
	{
		printf("send() failed(black): %d", WSAGetLastError());
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

	char whitePlayerName[MAX_PLAYER_NAME_LENGTH];
	ReceiveExact(whiteSocket, MAX_PLAYER_NAME_LENGTH, whitePlayerName);
	printf("Player-white joined: %s\n", whitePlayerName);
    PlayerColor white = WHITE;
	memcpy(sendBuf, &white, 4);
	result = send(whiteSocket, sendBuf, 4, 0);
	if (result == SOCKET_ERROR)
	{
		printf("send() failed(white): %d", WSAGetLastError());
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
	char buf[4];
	int toSend = OMOKPROTO_NOW_TURN;
	memcpy(buf, &toSend, 4);
	int result = send(whose, buf, sizeof(buf), 0);
	if (result == SOCKET_ERROR)
	{
		printf("TURN SEND ERROR\n");
		return GAMESTATE_ERROR;
	}
	
	int row, column;

	ReceiveExact(whose, 4, buf);
	memcpy(&row, buf, 4);

	ReceiveExact(whose, 4, buf);
	memcpy(&column, buf, 4);

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

	board[row][column] = (turn == BLACK ? BLACK : WHITE);

	memcpy(buf, &row, 4);
	result = send(other, buf, sizeof(buf), 0);
	if (result == SOCKET_ERROR)
	{
		printf("ROW SEND ERROR\n");
		return GAMESTATE_ERROR;
	}
	memcpy(buf, &column, 4);
	result = send(other, buf, sizeof(buf), 0);
	if (result == SOCKET_ERROR)
	{
		printf("COLUMN SEND ERROR\n");
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
