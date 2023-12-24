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

// 오목판 객체를 생성합니다.
// \returns 새로 할당한 CheckerBoard 객체로의 포인터를 반환합니다. 메모리 할당 실패 시 NULL이 반환됩니다.
CheckerBoard* CreateCheckerBoard(unsigned short offsetX, unsigned short offsetY, unsigned short cellSize, unsigned short margin);
// \param toDestroy: NULL이 아닌 포인터 전달 시 오목판과 관련된 모든 메모리를 해제합니다.
void DestroyCheckerBoard(CheckerBoard* toDestroy);

void ClearCheckerBoard(CheckerBoard* pCheckerBoard);
// 입력받은 SDL_Renderer에 현재 오목판 상태대로 그립니다.
void DrawCheckerBoard(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer);
// 오목판에 호버링 애니메이션을 업데이트합니다.
void DrawCursorHovering(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer, union SDL_Event* pEvent);

// 해당 셀을 입력 값으로 업데이트합니다.
void UpdateCell(CheckerBoard* pCheckerBoard, int row, int column, CellState value);

// 마우스 커서가 지금 오목판의 어떤 칸에 위치하였는지를 구합니다.
// \returns 오목판의 아무 칸 위에 위치한 경우 1, 그렇지 않은 경우 0입니다.
// \param pEvent 판별에 사용할 SDL 구조체입니다.
// \param [out]outRow 반환 값이 1인 경우 커서가 위치한 칸의 행을 저장합니다.
// \param [out]outCol 반환 값이 1인 경우 커서가 위치한 칸의 열을 저장합니다.
int GetMousePositionOverBoard(CheckerBoard* pCheckerBoard, union SDL_Event* pEvent, int* outRow, int* outCol);