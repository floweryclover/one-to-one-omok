#include <stdio.h>
#include <SDL2/SDL.h>

#include "CheckerBoard.h"

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	// Window �ʱ�ȭ
	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Omok Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
	if (!pWindow)
	{
		printf("SDL_CreateWindow() failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	// Renderer �ʱ�ȭ
	SDL_Renderer* pRenderer = NULL;
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	if (!pRenderer)
	{
		printf("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(pWindow);
		SDL_Quit();
		return 1;
	}

	// ������ ��ü �׽�Ʈ
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

	SDL_RenderClear(pRenderer);
	DrawCheckerBoard(pCheckerBoard, pRenderer, 64, 64);
	SDL_RenderPresent(pRenderer);

	// ���� ����
	Uint64 lastUpdateTick = SDL_GetTicks64();
	while (1)
	{

		// �̺�Ʈ ó��
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