#include <stdlib.h>
#include <SDL2/SDL.h>

#include "CheckerBoard.h"

CheckerBoard* CreateCheckerBoard(unsigned short offsetX, unsigned short offsetY, unsigned short cellSize, unsigned short margin)
{
	CheckerBoard* pCheckerBoard = (CheckerBoard*)malloc(sizeof(CheckerBoard));

	if (pCheckerBoard == NULL)
	{
		return NULL;
	}

	ClearCheckerBoard(pCheckerBoard);
	pCheckerBoard->cellSize_ = cellSize;
	pCheckerBoard->margin_ = margin;
	pCheckerBoard->offsetX_ = offsetX;
	pCheckerBoard->offsetY_ = offsetY;

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

void ClearCheckerBoard(CheckerBoard* pCheckerBoard)
{
	if (pCheckerBoard == NULL)
	{
		return;
	}

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			pCheckerBoard->cellStates_[i][j] = EMPTY;
		}
	}
}

void DrawCheckerBoard(CheckerBoard* pCheckerBoard, SDL_Renderer* pRenderer)
{
	if (pCheckerBoard == NULL)
	{
		return;
	}

	int finalCellSize = pCheckerBoard->cellSize_ + (pCheckerBoard->margin_ / 2);
	SDL_Rect cell;
	cell.x = 0;
	cell.y = 0;
	cell.w = pCheckerBoard->cellSize_;
	cell.h = pCheckerBoard->cellSize_;
	
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			cell.y = pCheckerBoard->offsetY_ + finalCellSize * i;
			cell.x = pCheckerBoard->offsetX_ + finalCellSize * j;
			SDL_SetRenderDrawColor(pRenderer, 208, 147, 109, 255);
			SDL_RenderFillRect(pRenderer, &cell);

			if (pCheckerBoard->cellStates_[i][j] != EMPTY)
			{
				if (pCheckerBoard->cellStates_[i][j] == BLACK)
				{
					SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
				}

				int fillSize = 3*(pCheckerBoard->cellSize_ / 4);
				SDL_Rect fill;
				fill.x = cell.x + (pCheckerBoard->cellSize_ - fillSize) / 2;
				fill.y = cell.y + (pCheckerBoard->cellSize_ - fillSize) / 2;
				fill.w = fillSize;
				fill.h = fillSize;
				
				SDL_RenderFillRect(pRenderer, &fill);
			}
		}
	}
}

void UpdateCell(CheckerBoard* pCheckerBoard, int row, int column, CellState value)
{
	if (pCheckerBoard == NULL)
	{
		return;
	}

	pCheckerBoard->cellStates_[row][column] = value;
}

int ProcessCellClick(CheckerBoard* pCheckerBoard, SDL_Event* pEvent, int* outRow, int* outCol)
{
	if (pCheckerBoard == NULL)
	{
		return 0;
	}

	if (pEvent == NULL)
	{
		return 0;
	}
	return 0;
}
