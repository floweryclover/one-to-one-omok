#include <stdio.h>
#include <SDL2/SDL.h>

#include "CheckerBoard.h"

#pragma comment(lib, "SDL2main.lib")

#define CELL_SIZE 32
#define CELL_MARGIN 4
#define OFFSET_X 32
#define OFFSET_Y 32

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	// Window 초기화
	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Omok Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
	if (!pWindow)
	{
		printf("SDL_CreateWindow() failed: %s\n", SDL_GetError());
		SDL_Quit();
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
		return 1;
	}

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(pRenderer);
	DrawCheckerBoard(pCheckerBoard, pRenderer);
	SDL_RenderPresent(pRenderer);

	UpdateCell(pCheckerBoard, 1, 2, BLACK);
	UpdateCell(pCheckerBoard, 3, 6, WHITE);

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(pRenderer);
	DrawCheckerBoard(pCheckerBoard, pRenderer);
	SDL_RenderPresent(pRenderer);

	// 메인 루프
	Uint64 lastUpdateTick = SDL_GetTicks64();
	while (1)
	{

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
		}
	}

	DestroyCheckerBoard(pCheckerBoard);
	SDL_DestroyWindow(pWindow);
	SDL_DestroyRenderer(pRenderer);
	SDL_Quit();
	return 0;
}