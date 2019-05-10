cmake_minimum_required(VERSION 3.13)
project(Compilator)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compilator main.cpp My_Asm.h My_Asm.cpp Table_And_Const.h)