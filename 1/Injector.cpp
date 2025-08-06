#include "main.h"

wchar_t dllPath[] = L"";
unsigned int dllSiz = static_cast<unsigned int>((wcslen(dllPath) + 1) * sizeof(wchar_t));

int injectorMain(int pid) {
    

    LPVOID lb = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
    if (lb == NULL) {
        std::cerr << "\033[31mError: can't get handle for kernel32.dll, error code: \033[0m" << GetLastError() << std::endl;
        return -1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));

    if (hProcess == NULL) {
        std::cerr << "\033[31mError: can't open the file, error code: \033[0m" << GetLastError() << std::endl;
        return -1;
    }
    std::cerr << "\033[33mInfo: Process successfully opened...\033[0m" << std::endl;
        

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess,NULL,dllSiz,MEM_RESERVE | MEM_COMMIT,PAGE_READWRITE);
    if (pRemoteMemory == NULL){
        std::cerr << "\033[31mError: can't allocate memory, error code: \033[0m" << GetLastError() << std::endl;
        return -1;
    }
    std::cerr << "\033[33mInfo: Memory for the process is allocated...\033[0m" << std::endl;
        
    if (WriteProcessMemory(hProcess, pRemoteMemory, dllPath, dllSiz, NULL) == FALSE){
        std::cerr << "\033[31mError: can't write data, error code: \033[0m" << GetLastError() << std::endl;
        return -1;
    }
    std::cerr << "\033[33mInfo: Data written to memory...\033[0m" << std::endl;

    HANDLE hThread = CreateRemoteThread(hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lb,pRemoteMemory,0,NULL);
    if (hThread == NULL){
        std::cerr << "\033[31mError: descriptor error, error code: \033[0m" << GetLastError() << std::endl;
        return -1;
    }
    std::cerr << "\033[32mInfo: Injected!!\033[0m" << std::endl;

    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}