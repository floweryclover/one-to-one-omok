//
// Created by floweryclover on 2024-01-05.
//

#ifndef ONETOONEOMOK_NETWORK_H
#define ONETOONEOMOK_NETWORK_H

#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define CELL_SIZE 32
#define CELL_MARGIN 4
#define OFFSET_X 32
#define OFFSET_Y 32

#define MAX_PLAYER_NAME_LENGTH 16

#define OMOKPROTO_NOW_TURN 100
#define OMOKPROTO_GAMEOVER_BLACKWIN 101
#define OMOKPROTO_GAMEOVER_WHITEWIN 102

int ReceiveExact(SOCKET from, int size, char* buf);

#endif //ONETOONEOMOK_NETWORK_H
