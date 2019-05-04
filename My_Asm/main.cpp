

#include "My_Asm.h"


int main()
{

    My_Assembly* compilator = new My_Assembly;

    compilator->Tokenization();

    compilator->Translation();


    compilator->Logics();

    compilator->Making_Object_File();


    delete compilator;


    return 0;
}