//
// Created by denis on 23.04.19.
//

#include "MY_vCPU.h"


MY_vCPU::MY_vCPU() {

    stack = new Stack;
    for(int i = 0; i < SIZE_RAM; i++)
    {
        RAM[i] = 0;
    }
    run_f[0] = &MY_vCPU::Run_Push;
    run_f[1] = &MY_vCPU::Run_Pop;
    run_f[2] = &MY_vCPU::Run_Add;
    run_f[3] = &MY_vCPU::Run_Mul;
    run_f[4] = &MY_vCPU::Run_Out;
    run_f[5] = &MY_vCPU::Run_In;
    run_f[6] = &MY_vCPU::Run_Jmp;
    run_f[7] = &MY_vCPU::Run_Dec;
    run_f[8] = nullptr;               // on this place MAIN must situated, but main doesn't need run operation
    run_f[9] = &MY_vCPU::Run_Call;
    run_f[10] = &MY_vCPU::Run_Ret;
    run_f[11] = &MY_vCPU::Run_Je;
    run_f[12] = &MY_vCPU::Run_Jnz;
    run_f[13] = &MY_vCPU::Run_Sub;
    run_f[14] = &MY_vCPU::Run_Div;
    run_f[15] = &MY_vCPU::Run_Sqrt;
    run_f[16] = &MY_vCPU::Run_End;
    run_f[17] = &MY_vCPU::Run_Jl;
    run_f[18] = &MY_vCPU::Run_Push_R;
    run_f[19] = &MY_vCPU::Run_Pop_R;

    stack->Set_ebp(SIZE_RAM - 1);
    stack->Set_esp(SIZE_RAM - 1);

    stack->Set_ptr_ram(RAM + SIZE_RAM - 1);

    array_reg[0] = 0;
    array_reg[1] = 0;
    array_reg[2] = 0;
    array_reg[3] = 0;

    eax = 0;

    size_code = 0;

    num_call = 0;

    current_ip = 0;

}


int MY_vCPU::Read_Bin_File() {

    FILE *file_read = fopen("/home/denis/CLionProjects/Compilator/result.txt", "rb");

    if (!file_read) {
        cout << "Can not open RAM.bin" << endl;
        return ERROR_IN_OPENNING_FILE;
    }

    int size = 0;

    fread(&size, sizeof(int), 1, file_read);

    size_code = size / sizeof(int);

    fread(RAM + SIZE_VACANT_MEM, sizeof(int), size_code, file_read);

    fclose(file_read);

    return SUCCESS;
}

void MY_vCPU::Run() {
    int ip = SIZE_VACANT_MEM;

    while (ip < size_code + SIZE_VACANT_MEM) {
        if (RAM[ip] == T_FUNC && RAM[ip + 1] == CMD_MAIN){
            ip += 5;
            break;
        }
        else {
            ip += 4;
        }
    }

    while (ip < size_code + SIZE_VACANT_MEM) {
        current_ip = ip;
        int check = (this->*run_f[RAM[ip] - CMD_LEVEL])(RAM[ip + 1], RAM[ip + 2]);
        assert(check == SUCCESS);
        ip = current_ip;

    }
}

int MY_vCPU::Run_Call(int type, int value){
    if(type == T_LABEL) {
        if (stack->push_v(current_ip + 4))       // Addres of returning

            if (stack->push_v(stack->Get_ebp()))     // push ebp

                if (stack->push_v(stack->Get_esp()))   // push esp

                    if (stack->pop_r(stack->Get_ebp_ptr()))   // pop ebp
                    {
                        int size = stack->Get_Size();
                        Table_For_Call.insert(make_pair(num_call, size));
                        num_call++;
                        current_ip = Find_Location(RAM[current_ip + 2]);   // change current_ip
                        return SUCCESS;
                    }

    }
    cout << "ERROR!!! CALL can not take uch an argument" << endl;
    return ERROR_CALL;
}

int MY_vCPU::Find_Location(int n) {
    int j = SIZE_VACANT_MEM;
    int kolvo_func = 0;
    while (j < size_code +
               SIZE_VACANT_MEM) {       // The func return the place in final array where
        if (kolvo_func == n)         //jmp is (not type_of_func) but const_value
            return j + 1;
        else {
            kolvo_func++;
            j += 4;
        }
    }
}

int MY_vCPU::Run_Sub(int type, int value)
{
    CALL_FUNC_WITHOUT_ARG(sub, SUB, ERROR_SUB)
}

int MY_vCPU::Run_Div(int type, int value)
{
    CALL_FUNC_WITHOUT_ARG(div, DIV, ERROR_DIV)

}

int MY_vCPU::Run_Push(int type, int value) {
    if(type == T_REG && value == REG_EAX){
        if(stack->push_r(&eax)){
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    if (type == T_REG) {
        if(stack->push_r(array_reg + value - REG_NOL)){
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    if (type == T_N){
        if(stack->push_v(value)) {
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    cout << "Operation Push can not work with " << type << " types of tokens" << endl;
    return ERROR_PUSH;
}

int MY_vCPU::Run_Push_R(int type, int value) {
    if(type == T_REG && value == REG_ESP){
        if(stack->push_esp(0)) {
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    else if(type == T_REG && value == REG_EBP){
        if(stack->push_ebp(0)){
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }

    }
    else if(type == T_REG){
        int adr = array_reg[value - REG_NOL];
        if(adr >= 0 && adr < SIZE_VACANT_MEM){
            if(stack->push_v(RAM[adr + 1])){
                current_ip += TRANS_TO_FUNC;
                return SUCCESS;
            }
        }
        else{
            cout << "Segmentation ERROR!!!!" << endl;
            cout << "Registor contains value which can not be in RAM" << endl;
            return ERROR_IN_PUSH_RAM;
        }
    }
    else if(type == T_N && value >= 0 && value < SIZE_VACANT_MEM){
        if(stack->push_v(RAM[value + 1])){
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }

    else if(type == T_ADR){
        if(Push_Smth_By_Strange_Adr(value)) {
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    cout << "ERROR!!! PUSH_IN_RAM can not take such a parametr" << endl;
    return ERROR_IN_PUSH_RAM;
}

bool MY_vCPU::Push_Smth_By_Strange_Adr(int value){
    int reg = *((short*)(&value));
    int num = *((short*)(&value) + 1);
    if(reg == REG_ESP){
        if(stack->push_esp(num))
            return true;
    }
    if(reg == REG_EBP){
        if(stack->push_ebp(num))
            return true;
    }
    if((reg > REG_NOL - 1) && (reg < REG_EAX)){
        int adr = array_reg[reg - REG_NOL] + num;
        if(adr >= 0 && adr < SIZE_VACANT_MEM){
            if(stack->push_v(RAM[adr + 1]))
                return true;
        }
        else{
            cout << "Segmentation ERROR!!!!" << endl;
            cout << "Registor contains value which can not be in RAM" << endl;
            return false;
        }
    }
    else{
        cout << "Such a reg is not situated in tables" << endl;
        return false;
    }
}

int MY_vCPU::Run_Pop(int type, int value) {
    if (type == T_REG) {
        if(stack->pop_r(array_reg + value - REG_NOL)){
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    cout << "ERROR!!!! Pop works only with registers" << endl;
    return ERROR_POP;
}

int MY_vCPU::Run_Pop_R(int type, int value) {
    if(type == T_N && value >= 0 && value < SIZE_VACANT_MEM){
        if(stack->pop_r(RAM + value + 1)){
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    if(type == T_REG && value > REG_NOL - 1 && value < REG_EAX ){
        int adr = array_reg[value - REG_NOL];
        if(adr > 0 && adr < SIZE_VACANT_MEM){
            stack->pop_r(RAM + adr + 1);
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
        else{
            cout << "Segmentation error Can not put in RAM by this address" << endl;
            return ERROR_IN_POP_RAM;
        }
    }
    if(type == T_ADR){
        if(Pop_Smth_By_Strange_Adr(value)){
            current_ip += TRANS_TO_FUNC;
            return SUCCESS;
        }
    }
    cout << "Segmentation fault!!! POP_IN_RAM can not take such parametres!!!" << endl;
    return ERROR_IN_POP_RAM;
}

bool MY_vCPU::Pop_Smth_By_Strange_Adr(int value){
    int reg = *((short*)(&value));
    int num = *((short*)(&value) + 1);
    if((reg > REG_NOL - 1) && (reg < REG_EAX)){
        int adr = array_reg[reg - REG_NOL] + num;
        if(adr >= 0 && adr < SIZE_VACANT_MEM){
            if(stack->pop_r(RAM + adr + 1))
                return true;
        }
        else{
            cout << "Segmentation ERROR!!!!" << endl;
            cout << "Registor contains value which can not be in RAM" << endl;
            return false;
        }
    }
    cout << "Such a reg is not situated in tables" << endl;
    return false;

}

int MY_vCPU::Run_Dec(int type, int value) {
    if (type == T_REG) {
        dec_r(value - REG_NOL);
        current_ip += TRANS_TO_FUNC;
        return SUCCESS;
    } else {
        cout << "ERROR!!! DEC can not work with not a registrs" << endl;
        return ERROR_DEC;
    }

}

int MY_vCPU::Run_Out(int type, int value){
    if(type == T_REG && value == REG_EAX){
        cout << "EAX = " << eax << endl;
        current_ip += TRANS_TO_FUNC;
        return SUCCESS;
    }
    if (type == T_REG) {
        stack->out(array_reg + value - REG_NOL);
        current_ip += TRANS_TO_FUNC;
        return SUCCESS;
    } else {
        cout << "ERROR!!! OUT can not work with not a registrs" << endl;
        return ERROR_OUT;
    }

}

int MY_vCPU::Run_Jnz(int type, int value)
{
    if(type == T_LABEL){
        if(stack->check_the_last_elem())
        {
            current_ip = Find_Location(value);
            return SUCCESS;
        }
        else{
            current_ip += 4;
            return SUCCESS;
        }
    }
    else{
        cout << "ERROR!!!! JNZ can take only label" << endl;
        return ERROR_JNZ;
    }
}

int MY_vCPU::Run_Jmp(int type, int value){
    if(type == T_LABEL){
        current_ip = Find_Location(value);
        return SUCCESS;
    }
    else{
        cout << "ERROR!!! JMP can take only label" << endl;
        return ERROR_JMP;
    }
}

int MY_vCPU::Run_Jl(int type, int value){
    if(type  == T_LABEL){
        if(stack->check_less()){
            current_ip  = Find_Location(value);
            return SUCCESS;
        }
        else{
            current_ip += 4;
            return SUCCESS;
        }
    }
    else{
        cout << "ERROR!!! JL can take only label" << endl;
        return ERROR_JL;
    }
}

int MY_vCPU::Run_Je(int type, int value){
    if(type == T_LABEL) {
        if (stack->check_equality()) {
            current_ip = Find_Location(value);
            return SUCCESS;
        }
        current_ip += 4;
        return SUCCESS;
    }
    else {
        cout << "ERROR!!!! JE can take only label" << endl;
        return ERROR_JE;
    }

}

int MY_vCPU::Run_In(int type, int value)
{
    if (type == T_REG) {
        in(value - REG_NOL);
        current_ip += TRANS_TO_FUNC;
        return SUCCESS;
    } else {
        cout << "ERROR!!! IN can not work with not a registrs" << endl;
        return ERROR_IN;
    }


}

int MY_vCPU::Run_Ret(int type, int value){
    if(type == T_REG && value == REG_EAX){
        if(stack->pop_r(&eax))                // put the result into eax
            if(stack->push_v(stack->Get_ebp()))   // push ebp
                if(stack->pop_r(stack->Get_esp_ptr()))    // pop esp
                {
                    int new_size = stack->Get_Size();            // change ptr_ram
                    auto it = Table_For_Call.find(num_call - 1);
                    if(num_call != 1)
                        num_call--;
                    int delta = new_size - it->second;
                    stack->Set_ptr_ram(stack->Get_ptr_ram() + delta); // change ptr_ram
                    stack->make_right_size();                            // change right size
                    if(stack->pop_r(stack->Get_ebp_ptr()))   // pop ebp
                        if(stack->pop_r(&current_ip))
                            return SUCCESS;
                }
    }
    cout << "ERROR!!! RET can not take such paramtres!" << endl;
    return ERROR_RET;

}

int MY_vCPU::Run_Sqrt(int type, int value){
    CALL_FUNC_WITHOUT_ARG(sqrt_, SQRT, ERROR_SQRT)
}

int MY_vCPU::Run_Mul(int type, int value){
    CALL_FUNC_WITHOUT_ARG(mul, MUL, ERROR_MUL)
}

int MY_vCPU::Run_Add(int type, int value){
 CALL_FUNC_WITHOUT_ARG(add, ADD, ERROR_ADD)
}

int MY_vCPU::Run_End(int type, int value){
    if(type == 0 && value == 0){
        current_ip = SIZE_VACANT_MEM + size_code + 1;
        return SUCCESS;
    }
    else{
        cout << "Operation END can not take such a parametr" << endl;
        return ERROR_END;
    }
}

void MY_vCPU::in(int i)
{
    int value = 0;
    cout << "Enter the value integer" << endl;
    cin >> value;
    array_reg[i] = value;

}

void MY_vCPU::dec_r(int i)
{
    array_reg[i]--;
}

MY_vCPU::~MY_vCPU() {
    array_reg[0] = 0;
    array_reg[1] = 0;
    array_reg[2] = 0;
    array_reg[3] = 0;
    delete stack;
}