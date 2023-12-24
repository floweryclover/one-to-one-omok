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

// ������ ��ü�� �����մϴ�.
// \returns ���� �Ҵ��� CheckerBoard ��ü���� �����͸� ��ȯ�մϴ�. �޸� �Ҵ� ���� �� NULL�� ��ȯ�˴ϴ�.
CheckerBoard* CreateCheckerBoard(unsigned short offsetX, unsigned short offsetY, unsigned short cellSize, unsigned short margin);
// \param toDestroy: NULL�� �ƴ� ������ ���� �� �����ǰ� ���õ� ��� �޸𸮸� �����մϴ�.
void DestroyCheckerBoard(CheckerBoard* toDestroy);

void ClearCheckerBoard(CheckerBoard* pCheckerBoard);
// �Է¹��� SDL_Renderer�� ���� ������ ���´�� �׸��ϴ�.
void DrawCheckerBoard(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer);
// �����ǿ� ȣ���� �ִϸ��̼��� ������Ʈ�մϴ�.
void DrawCursorHovering(CheckerBoard* pCheckerBoard, struct SDL_Renderer* pRenderer, union SDL_Event* pEvent);

// �ش� ���� �Է� ������ ������Ʈ�մϴ�.
void UpdateCell(CheckerBoard* pCheckerBoard, int row, int column, CellState value);

// ���콺 Ŀ���� ���� �������� � ĭ�� ��ġ�Ͽ������� ���մϴ�.
// \returns �������� �ƹ� ĭ ���� ��ġ�� ��� 1, �׷��� ���� ��� 0�Դϴ�.
// \param pEvent �Ǻ��� ����� SDL ����ü�Դϴ�.
// \param [out]outRow ��ȯ ���� 1�� ��� Ŀ���� ��ġ�� ĭ�� ���� �����մϴ�.
// \param [out]outCol ��ȯ ���� 1�� ��� Ŀ���� ��ġ�� ĭ�� ���� �����մϴ�.
int GetMousePositionOverBoard(CheckerBoard* pCheckerBoard, union SDL_Event* pEvent, int* outRow, int* outCol);