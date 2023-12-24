#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_PLAYER_NAME_LENGTH 16
#define GAMESTATE_ERROR -1
#define GAMESTATE_NOTHING 0
#define GAMESTATE_BLACK_WIN 1
#define GAMESTATE_WHITE_WIN 2

int ReceiveExact(SOCKET from, int size, char* buf);
int ProcessGame(SOCKET whose, CellState turn, CellState** board);
enum CellState;

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		fprintf(stderr, "usage: %s <listen-address> <port> <server-name>", argv[0]);
		return 1;
	}

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result)
	{
		printf("WSAStartup() failed: %d", result);
		return 1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Failed to create listenSocket: %d", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	SOCKADDR_IN hint;
	ZeroMemory(&hint, sizeof(SOCKADDR_IN));
	hint.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	hint.sin_port = htons(atoi(argv[2]));
	hint.sin_family = AF_INET;

	result = bind(listenSocket, (struct sockaddr*)&hint, sizeof(hint));
	if (result == SOCKET_ERROR)
	{
		printf("bind() failed: %d", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	result = listen(listenSocket, 5);
	if (result == SOCKET_ERROR)
	{
		printf("listen() failed: %d", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	SOCKADDR_IN blackSockAddr;
	int blackSockAddrSize = sizeof(blackSockAddr);
	printf("waiting for player-black...\n");
	SOCKET blackSocket = accept(listenSocket, (struct sockaddr*)&blackSockAddr, &blackSockAddrSize);
	if (blackSocket == INVALID_SOCKET)
	{
		printf("accept() failed(black): %d", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	char blackPlayerName[MAX_PLAYER_NAME_LENGTH];
	ReceiveExact(blackSocket, MAX_PLAYER_NAME_LENGTH, blackPlayerName);
	printf("Player-black joined: %s\n", blackPlayerName);


	printf("waiting for player-white...\n");
	SOCKADDR_IN whiteSockAddr;
	int whiteSockAddrSize = sizeof(whiteSockAddr);
	SOCKET whiteSocket = accept(listenSocket, (struct sockaddr*)&whiteSockAddr, &whiteSockAddrSize);
	if (blackSocket == INVALID_SOCKET)
	{
		printf("accept() failed(white): %d", WSAGetLastError());
		closesocket(listenSocket);
		closesocket(blackSocket);
		WSACleanup();
		return 1;
	}
	char whitePlayerName[MAX_PLAYER_NAME_LENGTH];
	ReceiveExact(whiteSocket, MAX_PLAYER_NAME_LENGTH, whitePlayerName);
	printf("Player-white joined: %s\n", whitePlayerName);

	CellState board[15][15];
	ZeroMemory(board, sizeof(CellState));
	int remainCellCount = 15 * 15;

	CellState turn = BLACK;
	while (1)
	{
		int result = ProcessGame((turn == BLACK ? blackSocket : whiteSocket), turn, board);
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

	result = shutdown(blackSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("shutdown() failed: %d", WSAGetLastError());
	}
	result = shutdown(whiteSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("shutdown() failed: %d", WSAGetLastError());
	}
	closesocket(blackSocket);
	closesocket(whiteSocket);
	WSACleanup();
	return 0;
}

typedef enum CellState_t {
	EMPTY,
	BLACK,
	WHITE
} CellState;

int ProcessGame(SOCKET whose, CellState turn, CellState** board)
{
	char buf[4];
	int row, column;
	// 흑돌 행
	ReceiveExact(whose, 4, buf);
	memcpy(&row, buf, 4);
	// 흑돌 열
	ReceiveExact(whose, 4, buf);
	memcpy(&column, buf, 4);

	// 유효한 범위인지 확인
	if (row < 0 || row > 14 || column < 0 || column > 14)
	{
		return GAMESTATE_ERROR;
	}

	// 놓을 수 없는데 놓으려고 하는건지 검증
	if (board[row][column] != EMPTY)
	{
		return GAMESTATE_ERROR;
	}

	// 놓기
	board[row][column] = (turn == BLACK ? BLACK : WHITE);
	
	// 오목을 완성했는지 확인
	for (int i = 0; i < 15; i++) // 가로
	{
		for (int j = 0; j < 15 - 5; j++)
		{
			if (board[i][j] == turn && board[i][j + 1] == turn && board[i][j + 2] == turn && board[i][j + 3] == turn && board[i][j + 4] == turn)
			{
				return (turn == BLACK ? GAMESTATE_BLACK_WIN : GAMESTATE_WHITE_WIN);
			}
		}
	}
	for (int i = 0; i < 15-5; i++) // 세로
	{
		for (int j = 0; j < 15; j++)
		{
			if (board[i][j] == turn && board[i+1][j] == turn && board[i+2][j] == turn && board[i+3][j] == turn && board[i+4][j] == turn)
			{
				return (turn == BLACK ? GAMESTATE_BLACK_WIN : GAMESTATE_WHITE_WIN);
			}
		}
	}
	for (int i = 0; i < 15 - 5; i++) // 대각선
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

int ReceiveExact(SOCKET from, int size, char* buf)
{
	if (size <= 0)
	{
		return -1;
	}

	int remain = size;
	while (remain > 0)
	{
		int received = recv(from, buf, remain, 0);
		if (received == 0)
		{
			printf("connection closed");
			return 0;
		}
		remain -= received;
	}
	return size;
}