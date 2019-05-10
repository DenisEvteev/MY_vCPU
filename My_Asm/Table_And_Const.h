//
// Created by denis on 18.04.19.
//


//@@@@@@@@@@                             @@@@@@@@@@@//
//    This File consist of Enum and TAble with      //
//     functions, registers, type_of_elements,     //
//     labels                                      //
//                                                //
//@@@@@@@@@@                           @@@@@@@@@@//
#pragma once


#include <iostream>
#include <vector>
#include <cstdio>
#include <map>
#include <cstdlib>
#include <iterator>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
///-------------------------------------------------------
using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::map;
using std::ofstream;
using std::ios;
using std::make_pair;
//--------------------------------------------------------------------
//
//
//---------------------------------------------
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
    CMD_JE,  //28
    CMD_JNZ,
    CMD_SUB,
    CMD_DIV,
    CMD_SQRT, //32
    CMD_END,
    CMD_JL, // 34
    CMD_PUSH_RAM, //35
    CMD_POP_RAM};// 36

enum {T_FUNC = 2,
    T_N,
    T_REG,
    T_LABEL,
    T_ADR};

enum {REG_AX = 7,
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
    ERROR_IN_OPENNING_FILE,
    ERROR_IN_PUSH_RAM,
    ERROR_IN_POP_RAM};

//---------------------------------------------------------

typedef map<string,int> Table;

extern Table Table_Register;

extern Table Table_Functions;