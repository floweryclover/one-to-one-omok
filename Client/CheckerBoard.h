#pragma once

typedef struct CheckerBoard_t
{
	int a;
} CheckerBoard;

// ������ ��ü�� �����մϴ�.
// \returns ���� �Ҵ��� CheckerBoard ��ü���� �����͸� ��ȯ�մϴ�. �޸� �Ҵ� ���� �� NULL�� ��ȯ�˴ϴ�.
CheckerBoard* CreateCheckerBoard();
// \param toDestroy: NULL�� �ƴ� ������ ���� �� �����ǰ� ���õ� ��� �޸𸮸� �����մϴ�.
void DestroyCheckerBoard(CheckerBoard* toDestroy);

// �Է¹��� SDL_Renderer�� ���� ������ ���´�� �׸��ϴ�.
void DrawCheckerBoard(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer, int offsetX, int offsetY);