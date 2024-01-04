cmake_minimum_required(VERSION 3.26)

add_executable(Client
        main.c
        CheckerBoard.h
        CheckerBoard.c
)

set(SDL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../../SDL2-2.28.5/include") # SDL2 include folder
set(SDL_LIB_DIR "${CMAKE_SOURCE_DIR}/../../SDL2-2.28.5/lib/x64") # SDL2 lib(x64) folder

target_include_directories(Client PRIVATE ${SDL_INCLUDE_DIR})
target_link_libraries(Client PRIVATE "${SDL_LIB_DIR}/SDL2.lib")
target_link_libraries(Client PRIVATE "${SDL_LIB_DIR}/SDL2main.lib")


project(
        Client
        VERSION 0.1
        DESCRIPTION "클라이언트"
        LANGUAGES C
)