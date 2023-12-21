#pragma once

typedef struct CheckerBoard_t
{
	int a;
} CheckerBoard;

// 생성 실패시 NULL을 반환합니다.
CheckerBoard* CreateCheckerBoard();
// \param toDestroy: NULL이 아닌 포인터 전달 시 오목판과 관련된 모든 메모리를 해제합니다.
void DestroyCheckerBoard(CheckerBoard* toDestroy);