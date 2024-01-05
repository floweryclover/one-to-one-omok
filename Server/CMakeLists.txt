cmake_minimum_required(VERSION 3.26)

add_executable(Server main.c)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_FOLDER_NAME "cmake-build-debug")
else()
    set(BUILD_FOLDER_NAME "cmake-build-release")
endif()

target_include_directories(Server PRIVATE "${CMAKE_SOURCE_DIR}/Common/include")
target_link_libraries(Server PRIVATE "${CMAKE_SOURCE_DIR}/${BUILD_FOLDER_NAME}/Common/Common.lib")

project(
        Server
        VERSION 0.1
        DESCRIPTION "서버"
        LANGUAGES C
)