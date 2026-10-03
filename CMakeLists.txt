cmake_minimum_required(VERSION 3.20)
project(APS_Ordenacao C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES "src/*.c")

add_executable(APS_Ordenacao ${SOURCES}
        src/performace.h)