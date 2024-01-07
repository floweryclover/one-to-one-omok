//
// Created by floweryclover on 2024-01-06.
//

#ifndef ONETOONEOMOK_PACKET_H
#define ONETOONEOMOK_PACKET_H

typedef enum PacketType
{
    RESPONSE_PLAYER_NAME,
} PacketType;

typedef struct PlayerNameResponse
{
    int nameLength;
    char name[];
} PlayerNameResponse;

#endif //ONETOONEOMOK_PACKET_H
