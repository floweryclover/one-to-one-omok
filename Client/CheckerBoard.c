#include <stdlib.h>

#include "CheckerBoard.h"

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