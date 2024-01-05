#pragma once

#include "common.h"

typedef struct CheckerBoard_t
{
	PlayerColor cellStates_[15][15];
	unsigned short offsetX_;
	unsigned short offsetY_;
	unsigned short cellSize_;
	unsigned short margin_;
} CheckerBoard;

CheckerBoard* CreateCheckerBoard(unsigned short offsetX, unsigned short offsetY, unsigned short cellSize, unsigned short margin);

void DestroyCheckerBoard(CheckerBoard* toDestroy);

void ClearCheckerBoard(CheckerBoard* pCheckerBoard);

void DrawCheckerBoard(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer);

void DrawCursorHovering(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer, union SDL_Event* pEvent);

void UpdateCell(CheckerBoard* pCheckerBoard, int row, int column, PlayerColor value);

int GetMousePositionOverBoard(CheckerBoard* pCheckerBoard, union SDL_Event* pEvent, int* outRow, int* outCol);