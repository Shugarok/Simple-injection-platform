#include "main.h"


int main()
{
    
    int pid = findProcID(L"notepad.exe");
    if (pid == -1) {
        std::cerr << "\033[31mError: can't find the process, error code: \033[0m" << GetLastError() << std::endl;
        return -1;
    }
    else
        std::cerr << "\033[33mInfo: PID = "<< pid << "\033[0m" << std::endl;


    injectorMain(pid);

    return 0;
}