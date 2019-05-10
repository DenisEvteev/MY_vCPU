//
// Created by denis on 23.04.19.
//

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <clocale>
#include <cmath>
#include <map>
#include <cassert>
//-----------------------------------------------------------------
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::map;
using std::make_pair;
//-----------------------------------------------------------------

const int SIZE_RAM = 5000;


enum {CMD_LEVEL = 17,
    CMD_PUSH = 17,
    CMD_POP,
    CMD_ADD,
    CMD_MUL,
    CMD_OUT,
    CMD_IN,
    CMD_JMP,
    CMD_DEC,
    CMD_MAIN,
    CMD_CALL,
    CMD_RET,
    CMD_JE,
    CMD_JNZ,
    CMD_SUB,
    CMD_DIV,
    CMD_SQRT,
    CMD_END,
    CMD_JL,
    CMD_PUSH_RAM,
    CMD_POP_RAM};

enum {T_FUNC = 2,
    T_N,
    T_REG,
    T_LABEL,
    T_ADR};

enum {REG_NOL = 7,
        REG_AX = 7,
        REG_BX,
        REG_CX,
        REG_DX,
        REG_EAX,
        REG_ESP,
        REG_EBP};

enum {ERROR_PUSH = 230,
    ERROR_POP,
    ERROR_ADD,
    ERROR_IN,
    ERROR_MUL,
    ERROR_OUT,
    ERROR_JMP,
    ERROR_DEC,
    ERROR_CALL,
    ERROR_MAIN,
    ERROR_RET,
    ERROR_SUB,
    ERROR_DIV,
    ERROR_SQRT,
    ERROR_END,
    ERROR_JNZ,
    ERROR_JL,
    ERROR_JE,
    ERROR_IN_OPENNING_FILE,
    ERROR_IN_PUSH_RAM,
    ERROR_IN_POP_RAM};

