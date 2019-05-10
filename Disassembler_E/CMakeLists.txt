cmake_minimum_required(VERSION 3.13)
project(disassembler_)

set(CMAKE_CXX_STANDARD 14)

add_executable(disassembler_ main.cpp Translator.cpp Translator.h Table_And_Const.h)