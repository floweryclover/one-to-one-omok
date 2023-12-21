#include <stdio.h>
#include <SDL2/SDL.h>

#include "CheckerBoard.h"

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
	pCheckerBoard = CreateCheckerBoard();
	if (!pCheckerBoard)
	{
		printf("CreateCheckerBoard() failed.\n");
		SDL_DestroyWindow(pWindow);
		SDL_DestroyRenderer(pRenderer);
		SDL_Quit();
		return 1;
	}
	DestroyCheckerBoard(pCheckerBoard);

	SDL_Rect aRect;
	aRect.x = 0;
	aRect.y = 10;
	aRect.w = 200;
	aRect.h = 200;
	// 메인 루프
	Uint64 lastUpdateTick = SDL_GetTicks64();
	while (1)
	{
		// 그리기 테스트
		Uint64 currentTick = SDL_GetTicks64();
		if (currentTick - lastUpdateTick > 100)
		{
			aRect.x++;
			lastUpdateTick = currentTick;
			SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
			SDL_RenderClear(pRenderer);
			SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(pRenderer, &aRect);
			SDL_RenderPresent(pRenderer);
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
		}
	}

	SDL_DestroyWindow(pWindow);
	SDL_DestroyRenderer(pRenderer);
	SDL_Quit();
	return 0;
}