#include <stdlib.h>
#include <SDL2/SDL.h>

#include "CheckerBoard.h"

#define CELL_SIZE 32
#define CELL_MARGIN 4

CheckerBoard* CreateCheckerBoard()
{
	CheckerBoard* pCheckerBoard = (CheckerBoard*)malloc(sizeof(CheckerBoard));

	return pCheckerBoard;
}

void DestroyCheckerBoard(CheckerBoard* toDestroy)
{
	if (toDestroy == NULL)
	{
		return;
	}

	free(toDestroy);
}

void DrawCheckerBoard(CheckerBoard* pCheckerBoard, SDL_Renderer* pRenderer, int offsetX, int offsetY)
{
	int finalCellSize = CELL_SIZE + (CELL_MARGIN / 2);
	SDL_Rect cell;
	cell.x = 0;
	cell.y = 0;
	cell.w = CELL_SIZE;
	cell.h = CELL_SIZE;
	
	SDL_SetRenderDrawColor(pRenderer, 208, 147, 109, 255);
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			cell.y = offsetY + finalCellSize * i;
			cell.x = offsetX + finalCellSize * j;
			SDL_RenderFillRect(pRenderer, &cell);
		}
	}
}
