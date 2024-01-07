cmake_minimum_required(VERSION 3.26)

add_subdirectory(Client)
add_subdirectory(Server)
add_subdirectory(Common)

project(
        OneToOneOmok
        VERSION 2.0
        DESCRIPTION "소켓 프로그래밍 예제용 1대1 오목 게임 프로젝트"
        LANGUAGES C
)