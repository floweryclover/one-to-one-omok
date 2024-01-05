cmake_minimum_required(VERSION 3.26)

add_executable(Client
        main.c
        CheckerBoard.h
        CheckerBoard.c
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_FOLDER_NAME "cmake-build-debug")
else()
    set(BUILD_FOLDER_NAME "cmake-build-release")
endif()
set(SDL_INCLUDE_DIR "" CACHE PATH "Path to SDL2 include folder e.g. /your/sdl2/include")
set(SDL_LIB_DIR "" CACHE PATH "Path to SDL2 lib(x64) folder e.g. /your/sdl2/lib/x64")

# SDL2
target_include_directories(Client PRIVATE ${SDL_INCLUDE_DIR})
target_link_libraries(Client PRIVATE "${SDL_LIB_DIR}/SDL2.lib")
target_link_libraries(Client PRIVATE "${SDL_LIB_DIR}/SDL2main.lib")

# Common
target_include_directories(Client PRIVATE "${CMAKE_SOURCE_DIR}/Common/include")
target_link_libraries(Client PRIVATE "${CMAKE_SOURCE_DIR}/${BUILD_FOLDER_NAME}/Common/Common.lib")


project(
        Client
        VERSION 0.1
        DESCRIPTION "클라이언트"
        LANGUAGES C
)