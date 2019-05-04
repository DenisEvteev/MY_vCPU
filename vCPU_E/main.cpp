#include "Stack.h"

#include "MY_vCPU.h"

int main()
{
    setlocale(LC_ALL, "Rus");

    MY_vCPU* proc = new MY_vCPU;

    proc->Read_Bin_File();

    proc->Run();

    delete proc;

    return 0;
}
