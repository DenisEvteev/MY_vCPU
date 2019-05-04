//
// Created by denis on 18.04.19.
//

#include "My_Asm.h"

const int SIZE = 20;   // size of array of ptr

int const_func = 100;   // value with which name_of_func is started

int Label_P = 60;    // value with which code_labels start

typedef int(My_Assembly::*Arr_Cmd)(type_of_element, type_of_element);

Arr_Cmd arr_for_logic[SIZE];

My_Assembly::My_Assembly() {
    arr_for_logic[0] = &My_Assembly::Log_Push;
    arr_for_logic[1] = &My_Assembly::Log_Pop;
    arr_for_logic[2] = &My_Assembly::Log_Add;
    arr_for_logic[3] = &My_Assembly::Log_Mul;
    arr_for_logic[4] = &My_Assembly::Log_Out;
    arr_for_logic[5] = &My_Assembly::Log_In;
    arr_for_logic[6] = &My_Assembly::Log_Jmp;
    arr_for_logic[7] = &My_Assembly::Log_Dec;
    arr_for_logic[8] = &My_Assembly::Log_Main;
    arr_for_logic[9] = &My_Assembly::Log_Call;
    arr_for_logic[10] = &My_Assembly::Log_Ret;
    arr_for_logic[11] = &My_Assembly::Log_Jmp;
    arr_for_logic[12] = &My_Assembly::Log_Jmp;
    arr_for_logic[13] = &My_Assembly::Log_Sub;
    arr_for_logic[14] = &My_Assembly::Log_Div;
    arr_for_logic[15] = &My_Assembly::Log_Sqrt;
    arr_for_logic[16] = &My_Assembly::Log_End;
    arr_for_logic[17] = &My_Assembly::Log_Jmp;
    arr_for_logic[18] = &My_Assembly::Log_Push_R;
    arr_for_logic[19] = &My_Assembly::Log_Pop_R;

    current_line = 1;
}



int My_Assembly::Tokenization()
{
    const char comment = '/';
    const char end = ';';
    const char end_str = '\n';
    const char space = ' ';

    const char open_ = '[';
    const char close_ = ']';

    int length = 0;
    int real_len = 0;
    FILE* file = fopen("/home/denis/CLionProjects/Compilator/solve_electrical_circuit.txt", "r");
    if (file == nullptr)
    {
        cout << "This file isxodnik.txt can not be opened" << endl;
        return ERROR_IN_OPENNING_FILE;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);
    vector<char> text(length);
    real_len = fread(&text[0], sizeof(char), length, file);
    fclose(file);

    int i = 0;
    while (i < real_len) {
        switch (text[i]) {
            case comment:
            {
                while (text[i] != end_str && i < real_len)
                    ++i;
                break;
            }
            case space:
                ++i;
                break;
            case end:
                ++i;
                break;
            case end_str:
                i++;
                current_line++;
                break;
            case open_: {
                string stroka_for_write;
                ++i;
                while (text[i] != close_) {
                    if (text[i] == space)
                        ++i;
                    else{
                        stroka_for_write += text[i];
                        ++i;
                    }
                }
                tokens.push_back({stroka_for_write, current_line});
                ++i;
                break;
            }

            default: {
                string stroka_for_write;
                while (text[i] != end_str && text[i] != space && text[i] != end && i < real_len) {
                    if(text[i] == open_){
                        stroka_for_write += text[i];
                        break;
                    }
                    if(text[i] == close_){
                        ++i;
                        break;
                    }
                    stroka_for_write += text[i];
                    ++i;
                }
                tokens.push_back({stroka_for_write, current_line});
                break;
            }
        }
    }
    return SUCCESS;
}

void My_Assembly::Translation()
{
    int size_tokens = tokens.size();

    for (int i = 0; i < size_tokens; ++i)
    {
        if(tokens[i].token == F){

            if(sort_tokens.size() != 0){
                auto it = Table_Label.find(tokens[i + 1].token);
                if(it != Table_Label.end())
                {
                    Table_of_Labels_With_Adress.insert(make_pair(it->second, i));
                    sort_tokens.push_back({T_LABEL, it->second});
                    Change_Adr_Label(it->second);
                    i += 1;
                }
                else{
                    cout << "Look at the code no announcment of this function" << endl;
                    cout << "ERROR at " << tokens[i].line << "line" << endl;
                    return;
                }
            }
            else{
                Table_Label.insert(make_pair(tokens[i + 1].token, const_func));
                Table_of_Labels_With_Adress.insert(make_pair(const_func, 0));
                const_func++;
                i += 1;
            }
        }


        else if(tokens[i].token == F_A)
        {
            Table_Label.insert(make_pair(tokens[i + 1].token, const_func));
            const_func++;
            i += 1;
        }


        else if(!Find_Functions(tokens[i].token)) {
            if (!Find_Register(tokens[i].token)) {
                if (!Find_Number(tokens[i].token)) {
                    if(!Find_Label(tokens[i].token, i)){
                        if(!Find_Adr_For_Ram(tokens[i].token)){
                            cout << "Token " << tokens[i].token << " can not be in any table" << endl;
                            cout << "ERROR at " << tokens[i].line << " line" << endl;
                            return;

                        }
                    }
                }
            }
        }

    }
}

bool My_Assembly::Find_Adr_For_Ram(const string &str){
    const char plus = '+';
    const char minus = '-';

    int reg_and_value = 0;
    string reg;

    int i = 0;

    int len_str = str.length();
    while(i < len_str && str[i] != plus && str[i] != minus){
        reg += str[i];
        ++i;
    }

    auto it = Table_Register.find(reg);
    if(it != Table_Register.end()){
        *((short*)(&reg_and_value)) = it->second;
    }
    else{
        return false;
    }

    char sign = str[i];
    if(sign == plus || sign == minus){
        string value;
        i++;
        while(str[i] != '\0'){
            value += str[i];
            ++i;
        }
        int int_value = Str_Int(value);
        if(int_value != NO_INT){
            if(sign == plus)
                *((short*)(&reg_and_value) + 1) = int_value;
            else
                *((short*)(&reg_and_value) + 1) = -int_value;
            sort_tokens.push_back({T_ADR, reg_and_value});
            return true;
        }
        else
            return false;
    }
    return false;
}

void My_Assembly::Logics() {

    for (unsigned int i = 0; i < sort_tokens.size(); i++) {
        if (sort_tokens[i].num_of_type == T_FUNC) {
            (this->*arr_for_logic[sort_tokens[i].const_value - CMD_LEVEL])(sort_tokens[i], sort_tokens[i + 1]);
        }
    }
}

void My_Assembly::Change_Adr_Label(int value){
    auto it = Table_of_Labels_With_Adress.find(value);
    int num_func = 0;
    int size_sort_tokens = sort_tokens.size();
    for(int i = 0; i < size_sort_tokens; ++i)
        {
            if(sort_tokens[i].num_of_type == T_LABEL && sort_tokens[i - 1].const_value != CMD_JMP &&
            sort_tokens[i - 1].const_value != CMD_JE &&  sort_tokens[i - 1].const_value != CMD_JNZ
            && sort_tokens[i - 1].const_value != CMD_CALL && sort_tokens[i - 1].const_value != CMD_JL
            && sort_tokens[i].const_value == value){
                it->second = num_func;
                return;
            }
            else{
                if(sort_tokens[i].num_of_type == T_FUNC)
                    num_func++;
            }
        }
}



int My_Assembly::Making_Object_File()
{
    FILE* file_exe = fopen("/home/denis/CLionProjects/Compilator/result.txt", "wb");

    if(!file_exe)
    {
        cout << "Can not open result.txt" << endl;
        return ERROR_IN_OPENNING_FILE;
    }

    int size_final_array = final_array.size();

    int num_byte = size_final_array * sizeof(logical_);

    fwrite(&num_byte, sizeof(int), 1, file_exe);

    for (int i = 0; i < size_final_array; ++i)
    {
        fwrite(&final_array[i], sizeof(logical_), 1, file_exe);
    }
//----------------For Disasm---------------------------------------------//

    int size_of_tables = Table_Label.size(); // num of elements in Tables

    fwrite(&size_of_tables, sizeof(int), 1, file_exe);  // write it in file for reading later it

    for(auto it_l = Table_Label.begin(); it_l != Table_Label.end(); it_l++){   // write in file table of labels
        FWRITE_TABELS(it_l)
    }

    int size_adr_lab = Table_of_Labels_With_Adress.size();

    fwrite(&size_adr_lab, sizeof(int), 1, file_exe);  // write in file num of elem in tabel_with_addresses

    for(auto it_adr = Table_of_Labels_With_Adress.begin(); it_adr != Table_of_Labels_With_Adress.end(); it_adr++){
        fwrite(&it_adr->second, sizeof(int), 1, file_exe);
        fwrite(&it_adr->first, sizeof(int), 1, file_exe);
    }

    //--------------------------- End writing for Disasm --------------------------------//
    fclose(file_exe);

    return SUCCESS;
}


bool My_Assembly::Func_For_A_I_M_O(type_of_element func, type_of_element next)
{
    if (next.num_of_type == T_REG) {
        final_array.push_back({func, next});
        return true;
    }
    if (next.num_of_type == T_FUNC || next.num_of_type == T_LABEL) {
        final_array.push_back({func, {0, 0}});
        return true;
    }
    return false;
}

int My_Assembly::Log_Main(type_of_element func, type_of_element next)
{
    if(next.num_of_type == T_FUNC || next.num_of_type == T_LABEL)
    {
        final_array.push_back({func, {0, 0}});
        return SUCCESS;
    }
    FUNC(MAIN)
    return ERROR_MAIN;

}

int My_Assembly::Log_Call(type_of_element func, type_of_element next)
{
    if(next.num_of_type == T_LABEL)
    {
        auto it = Table_of_Labels_With_Adress.find(next.const_value);
        final_array.push_back({func, {T_LABEL, it->second} });

        return SUCCESS;
    }
    FUNC(CALL)
    return ERROR_CALL;
}

int My_Assembly::Log_Ret(type_of_element func, type_of_element next)
{
    if(next.num_of_type == T_REG)
    {
        final_array.push_back({func, next});
        return SUCCESS;
    }
    FUNC(RET)
    return ERROR_RET;
}

int My_Assembly::Log_Push(type_of_element func, type_of_element next)
{
    if(next.num_of_type == T_N || next.num_of_type == T_REG)
    {
        final_array.push_back({func, next});
        return SUCCESS;
    }
    FUNC(PUSH)
    return ERROR_PUSH;
}

int My_Assembly::Log_Push_R(type_of_element func, type_of_element next){
    if((next.num_of_type == T_N && next.const_value >= 0)|| next.num_of_type == T_REG || next.num_of_type == T_ADR)
    {
        final_array.push_back({func, next});
        return SUCCESS;
    }
    FUNC(PUSH[)
    return ERROR_IN_PUSH_RAM;
}

int My_Assembly::Log_Pop_R(type_of_element func, type_of_element next){
    if((next.num_of_type == T_N && next.const_value >= 0)|| next.num_of_type == T_REG || next.num_of_type == T_ADR)
    {
        final_array.push_back({func, next});
        return SUCCESS;
    }
    FUNC(POP[)
    return ERROR_IN_POP_RAM;
}

int My_Assembly::Log_Pop(type_of_element func, type_of_element next)
{
    if(next.num_of_type == T_REG)
    {
        final_array.push_back({func, next});
        return SUCCESS;
    }
    FUNC(POP)
    return ERROR_POP;
}

int My_Assembly::Log_In(type_of_element func, type_of_element next) {
    if(Func_For_A_I_M_O(func, next))
        return SUCCESS;
    FUNC(IN)
    return ERROR_IN;
}

int My_Assembly::Log_Mul(type_of_element func, type_of_element next) {
    FUNC_WITHOUT_PARAM(MUL, ERROR_MUL)
}

int My_Assembly::Log_Out(type_of_element func, type_of_element next) {
    if(Func_For_A_I_M_O(func, next))
        return SUCCESS;
    FUNC(OUT)
    return ERROR_OUT;
}

int My_Assembly::Log_Add(type_of_element func, type_of_element next)
{
   FUNC_WITHOUT_PARAM(ADD, ERROR_ADD)
}

int My_Assembly::Log_Jmp(type_of_element func, type_of_element next)
{
    Change_Adr_Label(next.const_value);

    if(next.num_of_type == T_LABEL)
    {
        map<int,int>::iterator it;
        it = Table_of_Labels_With_Adress.find(next.const_value);
        if(it != Table_of_Labels_With_Adress.end())
        {
            final_array.push_back({func, {T_LABEL, it->second }});
            return SUCCESS;
        }
        cout << "ERROR!!!" << endl;
        cout << "Label with this const_value = " << next.const_value << " not found" << endl;
        return ERROR_JMP;
    }
    FUNC(JMP)
    return ERROR_JMP;
}

int My_Assembly::Log_Dec(type_of_element func, type_of_element next)
{
    if (next.num_of_type == T_REG) {
        final_array.push_back({func, next});
        return SUCCESS;
    }
    FUNC(DEC)
    return ERROR_DEC;
}

int My_Assembly::Log_Sub(type_of_element func, type_of_element next)
{
   FUNC_WITHOUT_PARAM(SUB, ERROR_SUB)
}

int My_Assembly::Log_Div(type_of_element func, type_of_element next){
    FUNC_WITHOUT_PARAM(DIV, ERROR_DIV)
}

int My_Assembly::Log_Sqrt(type_of_element func, type_of_element next){
    FUNC_WITHOUT_PARAM(SQRT, ERROR_SQRT)
}

int My_Assembly::Log_End(type_of_element func, type_of_element next){
    FUNC_WITHOUT_PARAM(END, ERROR_END)
}

bool My_Assembly::Find_Functions(string token)
{

    auto it = Table_Functions.find(token);
    if (it != Table_Functions.end())
    {
        sort_tokens.push_back({ T_FUNC, it->second });
        return true;
    }
    return false;
}

bool My_Assembly::Find_Register(string token)
{
    map<string,int> ::iterator it;
    it = Table_Register.find(token);
    if (it != Table_Register.end())
    {
        sort_tokens.push_back( { T_REG, it->second });
        return true;
    }
    return false;
}

bool My_Assembly::Find_Number(string token) {
    int result = 0;
    if(token[0] == '-')
        result = Str_Int(&token[1]);
    else
        result = Str_Int(token);
    if(result == NO_INT)
        return false;
    else
    {
        if(token[0] == '-')
            sort_tokens.push_back({T_N, -result});
        else
            sort_tokens.push_back({T_N, result});
        return true;
    }
}

bool My_Assembly::Find_Label(string token, int i) {
    auto it = Table_Label.find(token);

    if (it != Table_Label.end()) {
        if (sort_tokens[sort_tokens.size() - 1].const_value == CMD_CALL) {
            sort_tokens.push_back({T_LABEL, it->second});
            return true;
        }
        if (sort_tokens[sort_tokens.size() - 1].const_value != CMD_JMP
            && sort_tokens[sort_tokens.size() - 1].const_value != CMD_JE &&
            sort_tokens[sort_tokens.size() - 1].const_value != CMD_JNZ
            && sort_tokens[sort_tokens.size() - 1].const_value != CMD_JL) {
            Table_of_Labels_With_Adress[it->second] = sort_tokens.size() + 1;
            sort_tokens.push_back({T_LABEL, it->second});
            return true;
        }
        sort_tokens.push_back({T_LABEL, it->second});
        return true;
    } else if(token[0] == ':'){
        Table_Label.insert(make_pair(token, Label_P));
        if (sort_tokens.empty()) {
            Table_of_Labels_With_Adress[Label_P] = 0;
            sort_tokens.push_back({T_LABEL, Label_P});
            Label_P++;
            return true;
        }
        if (sort_tokens[sort_tokens.size() - 1].const_value == CMD_JMP
            || sort_tokens[sort_tokens.size() - 1].const_value == CMD_JE
            || sort_tokens[sort_tokens.size() - 1].const_value == CMD_JNZ
            || sort_tokens[sort_tokens.size() - 1].const_value == CMD_JL) {
            Table_of_Labels_With_Adress[Label_P] = FIND;
            sort_tokens.push_back({T_LABEL, Label_P});
            Label_P++;
            return true;
        } else {
            Table_of_Labels_With_Adress[Label_P] = i;
            sort_tokens.push_back({T_LABEL, Label_P});
            Label_P++;
            return true;
        }
    }
    return false;
}

int My_Assembly::Str_Int(const string& str)
{
    const int max_val = 58;
    const int min_val = 47;
    const int value_nol = 48;

    int len = str.length();
    for(int ip = 0; ip < len; ++ip)
    {
        if((int)str[ip] > max_val || (int)str[ip] < min_val)
            return NO_INT;
    }
    int value = 0;
    double ip = pow(10, len - 1);
    for (int i = 0; i < len; i++) {
        value = value + (((int)str[i] - value_nol) * ip);
        ip = ip / 10;
    }
    return value;
}

My_Assembly::~My_Assembly()
{
    tokens.clear();
    tokens.shrink_to_fit();
    sort_tokens.clear();
    sort_tokens.shrink_to_fit();
    final_array.clear();
    final_array.shrink_to_fit();
}
