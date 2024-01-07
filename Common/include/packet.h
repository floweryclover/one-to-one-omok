//
// Created by floweryclover on 2024-01-06.
//

#ifndef ONETOONEOMOK_PACKET_H
#define ONETOONEOMOK_PACKET_H

typedef enum PacketType
{
    REQUEST_PLAYER_NAME = 0,
    RESPONSE_PLAYER_NAME = 1,
    SET_PLAYER_COLOR = 2,
    REQUEST_PLACE_STONE = 3,
    RESPONSE_PLACE_STONE = 4,
} PacketType;

typedef struct PlayerNameRequest
{
    char _dummy;
} PlayerNameRequest;

typedef struct PlayerNameResponse
{
    int nameLength;
    char name[];
} PlayerNameResponse;

typedef struct PlayerColorPacket
{
    int color; // PlayerColor를 사용하세요.
} PlayerColorPacket;

typedef struct PlaceStoneRequest
{
    char _dummy;
} PlaceStoneRequest;

typedef struct PlaceStoneResponse
{
    int playerColor; // 이 필드는 클라이언트가 서버에게 보낼 경우 무시됩니다.
    int row;
    int column;
} PlaceStoneResponse;

#endif //ONETOONEOMOK_PACKET_H
