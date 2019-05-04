//
// Created by denis on 23.04.19.
//
#pragma once

#include "Enums_lib.h"

#define SUCCESS 120

#define TRANS_TO_FUNC 4

#define CALL_FUNC_WITHOUT_ARG(name, big_name, error_name)\
if(type == 0 && value == 0){\
    if(stack->name()){\
        current_ip += TRANS_TO_FUNC;\
        return SUCCESS;\
    }\
}\
cout << "ERROR!!!! " <<  #big_name << " does not take any parametrs" << endl;\
return error_name;\


class MY_vCPU {

    Stack *stack;

    int size_code;

    int array_reg[4]; // array of four registors

    int RAM[SIZE_RAM];

    map<int, int> Table_For_Call; // consist of counter and size of stack for ret when we want ptr_ram point on the the
                                  // top element of stack(i made it because after calling func a big part of stack can consist a lot of rubbish

    int num_call; // counter for calling functions

    int eax; // registor for result value from function

    int current_ip; // it contains the current counter in executable file

    void dec_r(int i);

    void in(int i);

    int Run_Add(int type, int value);

    int Run_Mul(int type, int value);

    int Run_Sqrt(int type, int value);

    int Run_In(int type, int value);

    int Run_Jnz(int type, int value);

    int Run_Je(int type, int value);

    int Run_Jl(int type, int value);

    int Run_Jmp(int type, int value);

    int Run_Out(int type, int value);

    int Run_Dec(int type, int value);

    int Run_Pop(int type, int value);

    int Run_Push(int type, int value);

    int Run_Div(int type, int value);

    int Run_Sub(int type, int value);

    int Run_Call(int type, int value);

    int Run_Ret(int type, int value);

    int Run_End(int type, int value);

    int Run_Push_R(int type, int value);

    int Run_Pop_R(int type, int value);

    bool Push_Smth_By_Strange_Adr(int value);

    bool Pop_Smth_By_Strange_Adr(int value);

    int Find_Location(int n);

public:
    typedef int (MY_vCPU::*Run_Func)(int, int);

    MY_vCPU();
    ~MY_vCPU();

    int Read_Bin_File();

    void Run();

    Run_Func run_f[SIZE];

};


#include "MY_vCPU.cpp"
