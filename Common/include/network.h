//
// Created by floweryclover on 2024-01-05.
//

#ifndef ONETOONEOMOK_NETWORK_H
#define ONETOONEOMOK_NETWORK_H

#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_PLAYER_NAME_LENGTH 16

#define OMOKPROTO_NOW_TURN 100
#define OMOKPROTO_GAMEOVER_BLACKWIN 101
#define OMOKPROTO_GAMEOVER_WHITEWIN 102

int ReceiveOmokPacket(SOCKET from, int* outType, void** outReceivedPacket);
int SendOmokPacket(SOCKET to, int type, const void* pOmokPacketStruct);

#endif //ONETOONEOMOK_NETWORK_H
