cmake_minimum_required(VERSION 3.26)

add_subdirectory(Client)
add_subdirectory(Server)

project(
        OneToOneOmok
        VERSION 0.1
        DESCRIPTION "소켓 프로그래밍 예제용 1대1 오목 게임 프로젝트"
        LANGUAGES C
)