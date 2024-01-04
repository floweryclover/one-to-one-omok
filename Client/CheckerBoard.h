#pragma once

typedef enum CellState_t
{
	EMPTY,
	BLACK,
	WHITE
} CellState;

typedef struct CheckerBoard_t
{
	CellState cellStates_[15][15];
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

void UpdateCell(CheckerBoard* pCheckerBoard, int row, int column, CellState value);

int GetMousePositionOverBoard(CheckerBoard* pCheckerBoard, union SDL_Event* pEvent, int* outRow, int* outCol);