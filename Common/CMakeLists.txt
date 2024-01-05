cmake_minimum_required(VERSION 3.26)

add_library(Common
        network.c)

target_include_directories(Common PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

project(
        Common
        VERSION 0.1
        DESCRIPTION "공용 라이브러리"
        LANGUAGES C
)