#include <stdio.h>
#include <SDL2/SDL.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "CheckerBoard.h"

#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "ws2_32.lib")

#define CELL_SIZE 32
#define CELL_MARGIN 4
#define OFFSET_X 32
#define OFFSET_Y 32

#define MAX_PLAYER_NAME_LENGTH 16

// 프로토콜 정의(4바이트 INT 값의 뜻)
// 0~14: 행 또는 열 입력
#define OMOKPROTO_NOW_TURN 100 // 니 차례니까 입력하세요
#define OMOKPROTO_GAMEOVER_BLACKWIN 101 // 게임 끝, 흑돌 승
#define OMOKPROTO_GAMEOVER_WHITEWIN 102 // 게임 끝, 백돌 승

int ReceiveExact(SOCKET from, int size, char* buf);

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	if (argc != 4)
	{
		printf("usage: %s <server-address> <server-port> <your-name>", argv[0]);
		return 1;
	}

	// Window 초기화
	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Omok Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
	if (!pWindow)
	{
		printf("SDL_CreateWindow() failed: %s\n", SDL_GetError());
		SDL_Quit();
		system("pause");
		return 1;
	}

	// Renderer 초기화
	SDL_Renderer* pRenderer = NULL;
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	if (!pRenderer)
	{
		printf("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(pWindow);
		SDL_Quit();
		system("pause");
		return 1;
	}

	// 오목판 객체 테스트
	CheckerBoard* pCheckerBoard = NULL;
	pCheckerBoard = CreateCheckerBoard(OFFSET_X, OFFSET_Y, CELL_SIZE, CELL_MARGIN);
	if (!pCheckerBoard)
	{
		printf("CreateCheckerBoard() failed.\n");
		SDL_DestroyWindow(pWindow);
		SDL_DestroyRenderer(pRenderer);
		SDL_Quit();
		system("pause");
		return 1;
	}

	// 통신 준비
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result)
	{
		printf("WSAStartup() failed.\n");
		SDL_DestroyWindow(pWindow);
		SDL_DestroyRenderer(pRenderer);
		SDL_Quit();
		system("pause");
		return 1;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		SDL_DestroyWindow(pWindow);
		SDL_DestroyRenderer(pRenderer);
		SDL_Quit();
		WSACleanup();
		system("pause");
		return 1;
	}

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(SOCKADDR_IN));
	inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_family = AF_INET;
	
	result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR)
	{
		printf("connect() failed: %d\n", WSAGetLastError());
		SDL_DestroyWindow(pWindow);
		SDL_DestroyRenderer(pRenderer);
		SDL_Quit();
		WSACleanup();
		system("pause");
		return 1;
	}

	// 플레이어 이름 전송
	char nameBuf[MAX_PLAYER_NAME_LENGTH];
	strcpy_s(nameBuf, MAX_PLAYER_NAME_LENGTH, argv[3]);
	send(clientSocket, nameBuf, MAX_PLAYER_NAME_LENGTH, 0);

	char buf[64];
	CellState me; // 내 색상 수신
	result = ReceiveExact(clientSocket, 4, buf);
	if (result == SOCKET_ERROR)
	{
		printf("recv() failed(색상 수신): %d\n", WSAGetLastError());
		SDL_DestroyWindow(pWindow);
		SDL_DestroyRenderer(pRenderer);
		SDL_Quit();
		closesocket(clientSocket);
		WSACleanup();
		system("pause");
		return 1;
	}
	memcpy(&me, buf, 4);
	printf("I AM %s", me == BLACK? "BLACK" : "WHITE");

	// 논블로킹 소켓으로 전환
	u_long on = 1;
	result = ioctlsocket(clientSocket, FIONBIO, &on);
	if (result == SOCKET_ERROR)
	{
		printf("ioctlsocket() failed: %d\n", WSAGetLastError());
		SDL_DestroyWindow(pWindow);
		SDL_DestroyRenderer(pRenderer);
		SDL_Quit();
		closesocket(clientSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	// 메인 루프
	Uint64 lastUpdateTick = SDL_GetTicks64();
	int receivedData;
	int receivedRow = 0;
	int receivedColumn = 0;
	int isMyTurn = 0;
	while (1)
	{
		int received = ReceiveExact(clientSocket, 4, buf);
		if (received == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			if (errorCode != WSAEWOULDBLOCK)
			{
				printf("recv() error: %d", errorCode);
				break;
			}
		}
		else
		{
			if (received == 0)
			{
				break;
			}
			memcpy(&receivedData, buf, 4);
			if (receivedData == OMOKPROTO_NOW_TURN)
			{
				isMyTurn = 1;
			}
			else if (receivedData == OMOKPROTO_GAMEOVER_BLACKWIN)
			{

			}
			else if (receivedData == OMOKPROTO_GAMEOVER_WHITEWIN)
			{

			}
			else
			{
				if (receivedData >= 0 && receivedData <= 14)
				{
					if (receivedRow == 0)
					{
						receivedRow = receivedData;
					}
					else
					{
						receivedColumn = receivedData;
						UpdateCell(pCheckerBoard, receivedRow, receivedColumn, me == BLACK ? WHITE : BLACK);

						receivedRow = 0;
						receivedColumn = 0;
					}
				}
				else
				{
					printf("received unknown packet: %d", receivedData);
					break;
				}
			}
		}

		// 이벤트 처리
		SDL_Event event;
		
		int eventOccured = SDL_PollEvent(&event);
		if (eventOccured)
		{
			if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					break;
				}
			}
			else if (event.type == SDL_QUIT)
			{
				break;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button = SDL_BUTTON_LEFT)
				{
					if (isMyTurn)
					{
						int row;
						int column;
						if (GetMousePositionOverBoard(pCheckerBoard, &event, &row, &column) && pCheckerBoard->cellStates_[row][column] == EMPTY)
						{
							u_long off = 0;
							result = ioctlsocket(clientSocket, FIONBIO, &off);
							if (result == SOCKET_ERROR)
							{
								printf("ioctlsocket() (off) failed: %d\n", WSAGetLastError());
								break;
							}

							memcpy(buf, &row, 4);
							int sendResult = send(clientSocket, buf, 4, 0);
							if (sendResult == 0)
							{
								printf("connection closed\n");
								break;
							}
							memcpy(buf, &column, 4);
							sendResult = send(clientSocket, buf, 4, 0);
							if (sendResult == 0)
							{
								printf("connection closed\n");
								break;
							}

							UpdateCell(pCheckerBoard, row, column, me);

							isMyTurn = 0;

							u_long on = 1;
							result = ioctlsocket(clientSocket, FIONBIO, &on);
							if (result == SOCKET_ERROR)
							{
								printf("ioctlsocket() (on) failed: %d\n", WSAGetLastError());
								break;
							}
						}
					}
				}
			}
		}
		// 이벤트 처리 후 업데이트
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(pRenderer);

		DrawCheckerBoard(pCheckerBoard, pRenderer);
		if (isMyTurn)
		{
			DrawCursorHovering(pCheckerBoard, pRenderer, &event);
		}

		SDL_RenderPresent(pRenderer);
	}

	DestroyCheckerBoard(pCheckerBoard);
	SDL_DestroyWindow(pWindow);
	SDL_DestroyRenderer(pRenderer);
	SDL_Quit();
	shutdown(clientSocket, SD_SEND);
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
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
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			printf("connection closed\n");
			return 0;
		}
		else
		{
			remain -= received;
		}
	}
	return size;
}