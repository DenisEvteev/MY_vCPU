//
// Created by denis on 18.04.19.
//
#pragma once


#include "Table_And_Const.h"

#define SUCCESS 1

#define NO_INT -111

#define FUNC(str)\
cout << "Function " << #str << " cannot take such a parametr!!!" << endl;\
cout << "Look carefully in your code!" << endl;\
//--------------------------------------------------------------------

#define FUNC_WITHOUT_PARAM(str, error)\
if(next.num_of_type == T_FUNC || next.num_of_type == T_LABEL){\
    final_array.push_back({func, {0, 0}});\
    return SUCCESS;\
} else{\
    cout << "Function " << #str << " cannot take such a parametr!!!" << endl;\
    cout << "Look carefully in your code!" << endl;\
    return error;\
}\
//-------------------------------------------------------------------------

#define FWRITE_TABELS(it)\
fwrite(&(it->second), sizeof(int), 1, file_exe);\
int size_name = (it->first).size();\
fwrite(&size_name, sizeof(int), 1, file_exe);\
fwrite((it->first).data(), sizeof(char), size_name, file_exe);\

#define FIND -333


#define F "func"  // the defenition of the function

#define F_A "func_a"  // the announcment of the function

#pragma pack(push, 1)
struct type_of_element{
    int num_of_type;
    int const_value;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct logical_
{
    type_of_element function;
    type_of_element variable;
};
#pragma pack(pop)

struct token_and_line{    // this struct is for finding errors in tokens during translation
    string token;
    int line;
};

//---------------------------------------------

class My_Assembly {
public:
    vector<token_and_line> tokens;

    vector<type_of_element> sort_tokens;

    map<int, int> Table_of_Labels_With_Adress;   //This table of labels contain 1) key -- code of label and 2) value -- their adress

    vector<logical_> final_array;

    Table Table_Label; // This table of labels contain 1) key -- names of labels and 2) value -- their code

    int current_line;

    int Log_Main(type_of_element func, type_of_element next);

    int Log_Call(type_of_element func, type_of_element next);

    int Log_Ret(type_of_element func, type_of_element next);

    int Log_Push(type_of_element func, type_of_element next);

    int Log_Pop(type_of_element func, type_of_element next);

    int Log_In(type_of_element func, type_of_element next);

    int Log_Mul(type_of_element func, type_of_element next);

    int Log_Out(type_of_element func, type_of_element next);

    int Log_Add(type_of_element func, type_of_element next);

    int Log_Jmp(type_of_element func, type_of_element next);

    int Log_Dec(type_of_element func, type_of_element next);

    int Log_Sub(type_of_element func, type_of_element next);

    int Log_Div(type_of_element func, type_of_element next);

    int Log_Sqrt(type_of_element func, type_of_element next);

    int Log_End(type_of_element func, type_of_element next);

    int Log_Push_R(type_of_element func, type_of_element next);

    int Log_Pop_R(type_of_element func, type_of_element next);

    bool Func_For_A_I_M_O(type_of_element func, type_of_element next);


    My_Assembly();
    ~My_Assembly();


    bool Find_Functions(string token);

    bool Find_Register(string token);

    bool Find_Number(string token);

    bool Find_Label(string token, int i);

    bool Find_Adr_For_Ram(const string& str);


    int Str_Int(const string& str);

    void Change_Adr_Label(int value);

    int Tokenization();
    void Translation();
    void Logics();

    int Making_Object_File();

};

#include "My_Asm.cpp"