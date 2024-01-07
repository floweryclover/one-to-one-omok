#include "network.h"
#include "packet.h"

#include <stdio.h>

int ReceiveOmokPacket(SOCKET from, int* outType, void** outOmokPacketStruct)
{
    int remainder;
    int result = 0;
    // bodySize 읽기
    int bodySize;
    remainder = sizeof(int);
    while (remainder > 0)
    {
        int received = 0;
        received += recv(from, (char*)((&bodySize)+(sizeof(bodySize)-remainder)), remainder, 0);

        remainder -= received;
        result += received;
    }

    // type 읽기
    int type;
    remainder = sizeof(int);
    while (remainder > 0)
    {
        int received = 0;
        received += recv(from, (char*)((&type)+(sizeof(type)-remainder)), remainder, 0);

        remainder -= received;
        result += received;
    }

    // body 읽기
    char* buffer = malloc(bodySize);
    remainder = bodySize;
    {
        int received = 0;
        received += recv(from, buffer + (bodySize-remainder), remainder, 0);

        remainder -= received;
        result += received;
    }

    // 역직렬화
    void* pOmokPacketStruct;
    switch (type)
    {
        case RESPONSE_PLAYER_NAME:
            pOmokPacketStruct = malloc(sizeof(int) + bodySize); // nameLength 크기 + name 크기(여기서는 bodySize와 동일)
            memcpy(&(((PlayerNameResponse*)pOmokPacketStruct)->nameLength), &bodySize, sizeof(int));
            memcpy(((PlayerNameResponse*)pOmokPacketStruct)->name, buffer, bodySize);
            break;
        default:
            fprintf(stderr, "ReceiveOmokPacket() 에러: 알 수 없는 패킷 타입입니다: %d\n", type);
            free(buffer);
            return 0;
    }

    *outType = type;
    *outOmokPacketStruct = pOmokPacketStruct;

    free(buffer);
    return result;
}

int SendOmokPacket(SOCKET to, int type, void* pOmokPacketStruct)
{
    char* buffer = NULL;
    int bodySize;
    // 직렬화
    switch(type)
    {
        case RESPONSE_PLAYER_NAME:
            PlayerNameResponse* pPlayerName = (PlayerNameResponse*)pOmokPacketStruct;
            bodySize = pPlayerName->nameLength;
            buffer = (char*)malloc(bodySize);
            memcpy(buffer, pPlayerName->name, bodySize);
            break;
        default:
            fprintf(stderr, "SendOmokPacket() 에러: 알 수 없는 패킷 타입입니다: %d\n", type);
            return 0;
    }

    char header[8];
    int result;
    memcpy(header, &bodySize, 4);
    memcpy(header + 4, &type, 4);
    result = send(to, header, 8, 0);
    result += send(to, buffer, bodySize, 0);

    free(buffer);
    return result;
}