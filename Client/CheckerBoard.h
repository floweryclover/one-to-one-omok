#pragma once

typedef struct CheckerBoard_t
{
	int a;
} CheckerBoard;

// ���� ���н� NULL�� ��ȯ�մϴ�.
CheckerBoard* CreateCheckerBoard();
// \param toDestroy: NULL�� �ƴ� ������ ���� �� �����ǰ� ���õ� ��� �޸𸮸� �����մϴ�.
void DestroyCheckerBoard(CheckerBoard* toDestroy);