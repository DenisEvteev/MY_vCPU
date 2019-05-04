#include "Translator.h"

int main(){
    setlocale(LC_ALL, "Rus");

    Translator* transl = new Translator;

    transl->Filling_Tables();

    transl->Find_Labels_In_Exe();

    transl->Generate_Code();

    delete transl;

    return 0;
}