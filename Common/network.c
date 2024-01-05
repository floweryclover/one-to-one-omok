#include "network.h"

#include <stdio.h>

int ReceiveExact(SOCKET from, int size, char* buf)
{
    if (size <= 0)
    {
        return -1;
    }

    int remain = size;
    while (remain > 0)
    {
        int received = recv(from, buf, remain, 0);
        if (received == SOCKET_ERROR)
        {
            return SOCKET_ERROR;
        }
        else if (received == 0)
        {
            printf("connection closed\n");
            return 0;
        }
        else
        {
            remain -= received;
        }
    }
    return size;
}