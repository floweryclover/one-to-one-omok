#pragma once

typedef struct CheckerBoard_t
{
	int a;
} CheckerBoard;

// 오목판 객체를 생성합니다.
// \returns 새로 할당한 CheckerBoard 객체로의 포인터를 반환합니다. 메모리 할당 실패 시 NULL이 반환됩니다.
CheckerBoard* CreateCheckerBoard();
// \param toDestroy: NULL이 아닌 포인터 전달 시 오목판과 관련된 모든 메모리를 해제합니다.
void DestroyCheckerBoard(CheckerBoard* toDestroy);

// 입력받은 SDL_Renderer에 현재 오목판 상태대로 그립니다.
void DrawCheckerBoard(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer, int offsetX, int offsetY);