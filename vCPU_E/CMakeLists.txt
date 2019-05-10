cmake_minimum_required(VERSION 3.13)
project(Processor)

set(CMAKE_CXX_STANDARD 14)

add_executable(Processor main.cpp Stack.h Stack.cpp MY_vCPU.h MY_vCPU.cpp Enums_lib.h)