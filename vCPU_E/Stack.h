//
// Created by denis on 23.04.19.
//
#pragma once

#include "Enums_lib.h"

#define ERROR_N "ON Zero can not be divided"


class Stack {
private:
    int esp;
    int ebp;
    int memory_for_stack;
    int size_stack;
    int* ptr_ram; // the pointer to the last cell in RAM, from which the stack is grown
public:

    int Get_esp();
    int Get_ebp();
    int* Get_ebp_ptr();
    int* Get_esp_ptr();
    int* Get_ptr_ram();


    int Get_Size();

    void Set_ptr_ram(int* ptr);

    void Set_esp(int value);

    void Set_ebp(int value);


    void make_right_size();

    Stack();
    ~Stack();

    bool push_r(int* reg);

    bool push_v(int value);

    bool pop_r(int* reg);

    bool push_esp(int num);

    bool push_ebp(int num);

    bool mul();

    bool sqrt_();

    bool add();

    bool sub();

    bool div();

    bool out(int* reg);

    bool check_the_last_elem();

    bool check_equality();

    bool check_less();

    bool check_size_stack();

};

#include "Stack.cpp"
