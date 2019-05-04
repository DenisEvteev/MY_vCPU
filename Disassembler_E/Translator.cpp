//
// Created by denis on 21.04.19.
//

#include "Translator.h"

Translator::Translator() {

    size_code = 0;
    print_type[0] = &Translator::Print_Func;
    print_type[1] = &Translator::Print_Value;
    print_type[2] = &Translator::Print_Reg;
    print_type[3] = &Translator::Print_Label;
    print_type[4] = &Translator::Print_Reg_And_Value;
}

int Translator::Filling_Tables() {

    FILE *file_read = fopen("/home/denis/CLionProjects/Compilator/result.txt", "rb");

    if (!file_read) {
        cout << "Can not open RAM.bin" << endl;
        return ERROR_IN_OPENNING_FILE;
    }

    int size = 0;

    fread(&size, sizeof(int), 1, file_read);

    size_code = size / sizeof(int);

    exe.resize(size_code);

    fread(&exe[0], sizeof(int), size_code, file_read);

//--------------------------------filling the Table for disasm-----------------------//

    int size_table = 0;   // read tabel_labels and insert it in map (code, name)

    fread(&size_table, sizeof(int), 1, file_read);

    int ip = 0;

    int code = 0;
    int size_name = 0;
    string name;

    while(ip != size_table){
        fread(&code, sizeof(int), 1, file_read);
        fread(&size_name, sizeof(int), 1, file_read);
        name.resize(size_name, '\0');
        fread(&name[0], sizeof(char), size_name, file_read);
        Table.insert(std::make_pair(code, name));
        ++ip;
    }

    int size_table_adr = 0;      // read tabel_label_addresses and insert it in map(code, adr)

    fread(&size_table_adr, sizeof(int), 1, file_read);   // adr it is a num of 2 in file_exe

    int j = 0;
    int code_l = 0;
    int adr_l = 0;

    while( j != size_table_adr){
        fread(&adr_l, sizeof(int), 1, file_read);
        fread(&code_l, sizeof(int), 1, file_read);
        Table_With_Adr_Labels.insert(std::make_pair(adr_l, code_l));
        j++;
    }

    return SUCCESS;

}

int Translator::Find_Labels_In_Exe(){


    ofstream out_file;

    out_file.open("ishodnik.txt");

    if(!out_file){
        cout << "ishodnik.txt can not be opened!!!" << endl;
        return ERROR_IN_OPENNING_FILE;
    }

    int ip = 0;
    map<int, int>::iterator it;
    map<int, string>::iterator it_l;

    while(ip != size_code){
        if(exe[ip] == T_LABEL){
            it = Table_With_Adr_Labels.find(exe[ip + 1]);
            it_l = Table.find(it->second);
            Table_Code_Name_Label.insert(std::make_pair(exe[ip + 1] * CORRECT_VALUE, it_l->second));
            if(exe[ip + 1] != THE_BEGIN_OF_PROG && it->second > FUNC_LABEL_CODE)
            {
                out_file << "func_a " << it_l->second << ';' << endl;
            }
            ip += 2;
        }
        else{
            ip += 2;
        }

    }
    return SUCCESS;
}

void Translator::Generate_Code(){


    ofstream file_for_code;
    file_for_code.open("ishodnik.txt", std::ios_base::app);

    if(!file_for_code){
        cout << "ishodnik.txt can not be opened!!!" << endl;
        return;
    }

    int ip = 0;


    while(ip != size_code)
    {
        Write_Label(ip, file_for_code);
        if(exe[ip] != NO_PARAM){
            (this->*print_type[exe[ip] - T_NOL])(ip + 1, file_for_code);
            ip += 2;
        }
        else
            ip += 2;
    }

    file_for_code.close();
}

void Translator::Print_Func(int i, ofstream& file_for_code){
    auto it = Table_Functions.begin();
    while(it != Table_Functions.end()){
        if(it->second == exe[i])
            break;
        it++;
    }
    if (it != Table_Functions.end()) {
        if(it->second == CMD_POP_RAM || it->second == CMD_PUSH_RAM)
            file_for_code << it->first;

        else if (exe[i + 1] != NO_PARAM)
            file_for_code << it->first << ' ';

        else
            file_for_code << it->first << ';' << endl;
    }
}

void Translator::Print_Reg(int i, ofstream& file_for_code){
    auto it = Table_Register.begin();
    while(it != Table_Register.end()){
        if(it->second == exe[i])
            break;
        it++;
    }
    if(it != Table_Register.end()){
        if(exe[i - 1] == CMD_PUSH_RAM || exe[i - 1] == CMD_POP_RAM)
            file_for_code << it->first << "];" << endl;
        else
            file_for_code << it->first << ';' << endl;
    }
    else
        cout << "THE GREAT MISTAKE IN PRINT REGISTOR HAS HAPPENED" << endl;
}

void Translator::Print_Value(int i, ofstream& file_for_code){
    if(exe[i - 2] == CMD_POP_RAM || exe[i - 2] == CMD_PUSH_RAM)
        file_for_code << exe[i] << "];" << endl;
    else
        file_for_code << exe[i] << ';' << endl;
}

void Translator::Print_Label(int i, ofstream& file_for_code){
    auto it = Table_Code_Name_Label.find(exe[i] * CORRECT_VALUE);
    if(it != Table_Code_Name_Label.end())
        file_for_code << it->second << ';' << endl;

    else
        cout << "THE GREAT MISTAKE IN PRINT LABEL HAS HAPPENED" << endl;
}

void Translator::Print_Reg_And_Value(int i, ofstream& file_for_code){
    int reg = *((short*)(&exe[i]));
    int value = *((short*)(&exe[i]) + 1);
    auto it = Table_Register.begin();
    while(it != Table_Register.end()){
        if(it->second == reg)
            break;
        it++;
    }
    file_for_code << it->first << ' ';
    if(value > 0){
        file_for_code << "+ " << value << "];" << endl;
    }
    else{
        file_for_code << "- " << -value << "];" << endl;
    }

}

void Translator::Write_Label(int i, std::ofstream& file_for_code){
    auto it = Table_Code_Name_Label.find(i);
    if(it != Table_Code_Name_Label.end()){
        if(it->second[0] == ':')
        {
            file_for_code << it->second << ';' << endl;
        }
        else{
            file_for_code << "func " << it->second << ';' << endl;
        }
    }
}

Translator::~Translator(){
    size_code = 0;
    exe.clear();
    exe.shrink_to_fit();
}