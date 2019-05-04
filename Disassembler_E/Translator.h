//
// Created by denis on 21.04.19.
//

#pragma once

#include "Table_And_Const.h"

#define ERROR_IN_OPENNING_FILE -219
#define SUCCESS 219
#define FUNC_LABEL_CODE 99
#define THE_BEGIN_OF_PROG 0
#define NO_PARAM 0
#define CORRECT_VALUE 4  // value for correct addresses in exe of labels
#define SIZE 5



class Translator {
    vector<int> exe;

    int size_code;

    map<int, string> Table;

    map<int, int> Table_With_Adr_Labels;

    map<int, string> Table_Code_Name_Label;

    void Write_Label(int i, std::ofstream& file_for_code);

    void Print_Func(int i, ofstream& file_for_code);

    void Print_Label(int i, ofstream& file_for_code);

    void Print_Reg(int i, ofstream& file_for_code);

    void Print_Value(int i, ofstream& file_for_code);

    void Print_Reg_And_Value(int i, ofstream& file_for_code);
public:

    typedef void (Translator::*Fprintf_Code)(int, ofstream&);

    Translator();
    ~Translator();

    int Find_Labels_In_Exe();

    void Generate_Code();

    int Filling_Tables();

    Fprintf_Code print_type[SIZE];

};

#include "Translator.cpp"