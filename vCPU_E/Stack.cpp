//
// Created by denis on 23.04.19.
//

#include "Stack.h"
//##############################-----getters-----####################################


int Stack::Get_esp(){
    return esp;
}

int Stack::Get_Size(){
    return size_stack;
}

int* Stack::Get_ptr_ram(){
    return ptr_ram;
}

int Stack::Get_ebp(){
    return ebp;
}

int* Stack::Get_ebp_ptr(){
    return &ebp;
}

int* Stack::Get_esp_ptr() {
    return &esp;
}
//###########################-----setters-----#####################################
void Stack::Set_ptr_ram(int* ptr){
    ptr_ram = ptr;
}

void Stack::Set_esp(int value)
{
    esp = value;
}

void Stack::Set_ebp(int value)
{
    ebp = value;
}
//#################################################################################
// Different functions for stack
Stack::Stack()
{
    memory_for_stack = MEMORY_STACK;
    size_stack = 0;
}

bool Stack::check_size_stack()
{
    if(size_stack > memory_for_stack)
        return false;
    return true;
}

void Stack::make_right_size() {
    size_stack = SIZE_RAM - esp;
}

Stack::~Stack(){
    memory_for_stack = 0;
    ptr_ram = nullptr;
}

//#########################----different types of PuSh-----#############################
//Push_from_registor
//Push_value
//Push_eax ### in eax - the result from function
//Push_esp ### in esp
//Push_ebp ###
//######################################################################################
bool Stack::push_r(int* reg)
{
    if(reg != nullptr)
    {
        if(check_size_stack())
        {
            if(esp != ebp || (esp == ebp && size_stack != 0))
            {
                esp--;
                ptr_ram--;
                *ptr_ram = *reg;
                size_stack++;
                return true;
            }
            else if(esp == ebp && size_stack == 0)
            {
                *ptr_ram = *reg;
                size_stack++;
                return true;
            }
        }
        else{
            cout << "The memory for stack is overfilled" << endl;
            return false;
        }
    }
    else
    {
        cout << "push_r can not work with NULL reg" << endl;
        return false;
    }
}

bool Stack::push_v(int value)
{
    if(check_size_stack())
    {
        if(esp != ebp || (esp == ebp && size_stack != 0))
        {
            esp--;
            ptr_ram--;
            *ptr_ram = value;
            size_stack++;
            return true;
        }
        if(esp == ebp && size_stack == 0)
        {
            *ptr_ram = value;
            size_stack++;
            return true;
        }
    }
    cout << "Memory for stack is overfilled" << endl;
    return false;

}

bool Stack::push_esp(int num){
    if(check_size_stack()){
        if(num >= 0 && size_stack != 0){
            esp--;
            *(ptr_ram - 1) = *(ptr_ram + num);
            ptr_ram--;
            size_stack++;
            return true;
        }
        else{
            cout << "Sigmintation fault!!! operation push_esp can work only with positive value" << endl;
            return false;
        }

    }
    else{
        cout << "The memory for stack is overfilled" << endl;
        return false;
    }
}

bool Stack::push_ebp(int num){
    if(check_size_stack()){
        if(num <= 0 && size_stack != 0){
            esp--;
            *(ptr_ram - 1) = *(ptr_ram + size_stack - 1 - num);
            ptr_ram--;
            size_stack++;
            return true;
        }
        else{
            cout << "Sigmintation fault!!! operation push_ebp can work only with negative value" << endl;
            return false;
        }

    }
    else{
        cout << "The memory for stack is overfilled" << endl;
        return false;
    }

}
//#############################------different types PoP---------######################
//Pop_registor
//####################################################################################
bool Stack::pop_r(int* reg)
{
    if(reg != nullptr)
    {
        if(esp != ebp)
        {
            *reg = *ptr_ram;
            *ptr_ram = 0;
            ptr_ram++;
            if(reg != &esp)
                esp++;
            size_stack--;
            return true;
        }
        else
        {
            if(size_stack != 0){
                *reg = *ptr_ram;
                *ptr_ram = 0;
                size_stack--;
                if(reg == &ebp){
                    ptr_ram++;
                    esp++;
                }
                return true;

            }
            else{
                cout << "In stack no elements" << endl;
                return false;
            }
        }
    }
    else
    {
        cout << "ERROR!!! The program can not insert in NULL reg" << endl;
        return false;
    }

}



//##################################---- functions for different jumps ------#########
//check_equality ### check equality of the last two elements in stack
//check_less ###  check if the penultimate element less than the last
//check_the_last_elem ### check if the last element is ZERO
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

bool Stack::check_less(){
    if(size_stack > 1){
        if(*(ptr_ram + 1) < *ptr_ram)
            return true;
        return false;
    }
    else{
        cout << "STACK IS EMPTY FOR THIS OPERATION!!!" << endl;
    }
}

bool Stack::check_equality(){
    if(size_stack > 1){
        if(*ptr_ram == *(ptr_ram + 1))
            return true;
        return false;
    }
    else{
        cout << "STACK IS EMPTY FOR THIS OPERATION!!!" << endl;
    }
}

bool Stack::check_the_last_elem()
{
    if(size_stack != 0){
        if(*ptr_ram == 0)
            return false;
        return true;

    }
    else{
        cout << "STACK IS EMPTY FOR THIS OPERATION" << endl;
    }
}

//@@@@@@@@@@@@@@@@@@@@@@/// Different arithmetic operations ///@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//multiplication
//taking root
//addition
//subtraction
//division
//out - take element from stack and put it in reg
//###########################################################################################

bool Stack::mul()
{
    if(esp != ebp)
    {
        *ptr_ram = *ptr_ram * *(ptr_ram + 1);
        return true;
    }
    else
    {
        cout << "ERROR!!! Operation mul can not be made" << endl;
        return false;
    }
}

bool Stack::sqrt_()
{
    if(*ptr_ram >= 0){
        *ptr_ram = (int)sqrt((double)*ptr_ram);
        return true;
    }
    else{
        cout << "Sqrt can not be done with negative values" << endl;
        return false;
    }
}

bool Stack::add()
{
    if(esp != ebp)
    {
        *ptr_ram = *ptr_ram + *(ptr_ram + 1);
        return true;
    }
    else
    {
        cout << "ERROR!!! Operation mul can not be made" << endl;
        return false;
    }
}

bool Stack::sub()
{
    if(esp != ebp)
    {
        *ptr_ram = *(ptr_ram + 1) - *ptr_ram;
        return true;
    }
    else
    {
        cout << "ERROR!!! Operation sub can not be made" << endl;
        return false;
    }
}

bool Stack::div(){
    if(esp != ebp)
    {
        try{
            if(*ptr_ram == 0)
            {
                throw ERROR_N;
            }
            *ptr_ram = *(ptr_ram + 1) / *ptr_ram;
            return true;
        }
        catch(char* str){
            cout << str  << endl;
            return false;
        }
    }
    else{
        cout << "ERROR!!! Operation div can not be made" << endl;
        return false;
    }
}

bool Stack::out(int* reg)
{
    if(reg != nullptr)
    {
        *reg = *ptr_ram;
        cout <<  "THE RESULT IS " <<  *reg << endl;
        return true;
    }
    else
    {
        cout << "Nothing can be put in NULL reg" << endl;
        return false;
    }
}