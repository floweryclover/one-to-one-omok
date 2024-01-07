#include <stdio.h>

#include "SDL.h"
#include "CheckerBoard.h"
#include "network.h"
#include "packet.h"

#define CELL_SIZE 32
#define CELL_MARGIN 4
#define OFFSET_X 32
#define OFFSET_Y 32

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	if (argc != 4)
	{
		fprintf(stderr, "usage: %s <server-address> <server-port> <your-name>", argv[0]);
		goto Cleanup_0;
	}

	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Omok Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
	if (!pWindow)
	{
		printf("SDL_CreateWindow() failed: %s\n", SDL_GetError());
		goto Cleanup_1;
	}

	SDL_Renderer* pRenderer = NULL;
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	if (!pRenderer)
	{
		printf("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		goto Cleanup_2;
	}

	CheckerBoard* pCheckerBoard = NULL;
	pCheckerBoard = CreateCheckerBoard(OFFSET_X, OFFSET_Y, CELL_SIZE, CELL_MARGIN);
	if (!pCheckerBoard)
	{
		printf("CreateCheckerBoard() failed.\n");
		goto Cleanup_3;
	}

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result)
	{
		printf("WSAStartup() failed.\n");
		goto Cleanup_4;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		goto Cleanup_5;
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
		goto Cleanup_6;
	}

    int myColor = EMPTY;
    int isMyTurn = 0;
    fd_set readFdSet, readFdSetCopy;
    struct timeval time;
    FD_ZERO(&readFdSet);
    FD_SET(clientSocket, &readFdSet);
	while (1)
	{
        time.tv_usec = 100;
        time.tv_sec = 0;
        readFdSetCopy = readFdSet;
        result = select(1, &readFdSetCopy, NULL, NULL, &time);
        if (result == -1)
        {
            fprintf(stderr, "An error occured in select().\n");
            break;
        }
        else if (result > 0)
        {
            int receivedType;
            void* receivedPacket;
            result = ReceiveOmokPacket(clientSocket, &receivedType, &receivedPacket);
            if (result == 0)
            {
                fprintf(stderr, "Connection closed.\n");
                goto Cleanup_6;
            }
            else if (result < 0)
            {
                fprintf(stderr, "An error occured while receiving packet: %d\n", WSAGetLastError());
                goto Cleanup_6;
            }

            result = 1;
            switch (receivedType)
            {
                case REQUEST_PLAYER_NAME:
                    PlayerNameResponse* response;
                    response = (PlayerNameResponse*)malloc(sizeof(int) + strlen(argv[3]));
                    response->nameLength = strlen(argv[3]);
                    memcpy(response->name, argv[3], response->nameLength);
                    result = SendOmokPacket(clientSocket, RESPONSE_PLAYER_NAME, (void*)response);
                    free(response);
                    break;
                case RESPONSE_PLAYER_NAME:
                    break;
                case SET_PLAYER_COLOR:
                    PlayerColorPacket* pColor = (PlayerColorPacket*)receivedPacket;
                    myColor = pColor->color;
                    break;
                case REQUEST_PLACE_STONE:
                    isMyTurn = 1;
                    break;
                case RESPONSE_PLACE_STONE:
                    PlaceStoneResponse* pStoneResponse = (PlaceStoneResponse*)receivedPacket;
                    UpdateCell(pCheckerBoard, pStoneResponse->row, pStoneResponse->column, pStoneResponse->playerColor);
                    break;
                default:
                    fprintf(stderr, "Received unknown type of packet: %d\n", receivedType);
                    goto Cleanup_6;
            }
            if (result <= 0)
            {
                fprintf(stderr, "Failed to send packet.\n");
                goto Cleanup_6;
            }

            free(receivedPacket);
        }

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
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (isMyTurn)
					{
						int row;
						int column;
						if (GetMousePositionOverBoard(pCheckerBoard, &event, &row, &column) && pCheckerBoard->cellStates_[row][column] == EMPTY)
						{
                            PlaceStoneResponse myResponse;
                            myResponse.playerColor = myColor;
                            myResponse.row = row;
                            myResponse.column = column;
                            if (SendOmokPacket(clientSocket, RESPONSE_PLACE_STONE, (const void*)(&myResponse)) <= 0)
                            {
                                fprintf(stderr, "Failed to send packet containing the location to be placed.\n");
                                goto Cleanup_6;
                            }
						}
					}
				}
			}
		}

		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(pRenderer);

		DrawCheckerBoard(pCheckerBoard, pRenderer);
		if (isMyTurn)
		{
			DrawCursorHovering(pCheckerBoard, pRenderer, &event);
		}

		SDL_RenderPresent(pRenderer);
	}




Cleanup_6:
    if (shutdown(clientSocket, SD_SEND) == SOCKET_ERROR)
    {
        fprintf(stderr, "shutdown() error: %d\n", WSAGetLastError());
    }
	closesocket(clientSocket);
Cleanup_5:
	WSACleanup();
Cleanup_4:
    DestroyCheckerBoard(pCheckerBoard);
Cleanup_3:
    SDL_DestroyRenderer(pRenderer);
Cleanup_2:
    SDL_DestroyWindow(pWindow);
Cleanup_1:
    SDL_Quit();
Cleanup_0:
	return 0;
}