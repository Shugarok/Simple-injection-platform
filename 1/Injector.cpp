#include "main.h"

wchar_t dllPath[] = L"";
unsigned int dllSiz = static_cast<unsigned int>((wcslen(dllPath) + 1) * sizeof(wchar_t));


int injectorMain(int pid) {

    typedef NTSTATUS(NTAPI* pNtCreateThreadEx)(
        OUT PHANDLE hThread,
        IN ACCESS_MASK DesiredAccess,
        IN PVOID ObjectAttributes,
        IN HANDLE ProcessHandle,
        IN PVOID lpStartAddress,
        IN PVOID lpParameter,
        IN ULONG Flags,
        IN SIZE_T StackZeroBits,
        IN SIZE_T SizeOfStackCommit,
        IN SIZE_T SizeOfStackReserve,
        OUT PVOID lpBytesBuffer
        );

   
    LPVOID lb = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
    if (lb == NULL) {
        if (debuggingMode) {
            std::cerr << "\033[31m[ERROR] Can't get LoadLibraryW address, error: \033[0m"<< GetLastError() << std::endl;
        }
        return -1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
    if (hProcess == NULL) {
        if (debuggingMode) {
            std::cerr << "\033[31m" << currentTime()
                << " [ERROR] Can't open process, error: \033[0m"<< GetLastError() << std::endl;
        }
        return -1;
    }
    else if (debuggingMode) {
        std::cerr << "\033[34m" << currentTime() << " [INFO] Process opened successfully\033[0m" << std::endl;
    }


    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllSiz, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteMemory == NULL) {
        if (debuggingMode) {
            std::cerr << "\033[31m" << currentTime()<< " [ERROR] Can't allocate memory, error: \033[0m" << GetLastError() << std::endl;
        }
        CloseHandle(hProcess);
        return -1;
    }
    else if (debuggingMode) {
        std::cerr << "\033[34m" << currentTime() << " [INFO] Memory allocated\033[0m" << std::endl;
    }

  
    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath, dllSiz, NULL)) {
        if (debuggingMode) {
            std::cerr << "\033[31m" << currentTime()<< " [ERROR] Can't write memory, error: \033[0m"<< GetLastError() << std::endl;
        }
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }
    else if (debuggingMode) {
        std::cerr << "\033[34m" << currentTime()<< " [INFO] DLL path written\033[0m" << std::endl;
    }

    
    HANDLE hThread = NULL;
    NTSTATUS status = 0;
    bool ntSuccess = true;
   
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (hNtdll) {
        pNtCreateThreadEx NtCTE = (pNtCreateThreadEx)GetProcAddress(hNtdll, "NtCreateThreadEx");
        if (NtCTE) {
            status = NtCTE(&hThread,0x1FFFFF,NULL,hProcess,lb,pRemoteMemory,0, 0, 0, 0, NULL);

       
            if (status >= 0) {  
                if (debuggingMode) {
                    std::cerr << "\033[34m" << currentTime()<< " [INFO] NtCreateThreadEx succeeded (status: 0x"<< std::hex << status << ")\033[0m" << std::endl;
                }
               
            }
            else if (debuggingMode) {
                    std::cerr << "\033[33m" << currentTime()<< " [WARN] NtCreateThreadEx failed (status: 0x"<< std::hex << status << ")\033[0m" << std::endl;
                    ntSuccess = false;
               
            }
        }
        else if (debuggingMode) {
                std::cerr << "\033[33m" << currentTime()<< " [WARN] Can't find NtCreateThreadEx, error: \033[0m"<< GetLastError() << std::endl;
                ntSuccess = false;
           
        }
    }
    else if (debuggingMode) {
            std::cerr << "\033[33m" << currentTime()<< " [WARN] Can't load ntdll.dll, error: \033[0m"<< GetLastError() << std::endl;
            ntSuccess = false;
    }
    

   
    if (UseCreateRemoteThreadAfterNT && !ntSuccess) {
        hThread = CreateRemoteThread(hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lb,pRemoteMemory,0, NULL);

        if (hThread == NULL) {
            if (debuggingMode) {
                std::cerr << "\033[31m" << currentTime() << " [ERROR] CreateRemoteThread failed, error: \033[0m"<< GetLastError() << std::endl;
                
            }
            
            VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return -1;
        }

        if (debuggingMode) {
            std::cerr << "\033[34m" << currentTime()<< " [INFO] Using CreateRemoteThread\033[0m" << std::endl;
        }
    }

 
   /* WaitForSingleObject(hThread, INFINITE);*/

    if (debuggingMode) {
        std::cerr << "\033[32m" << currentTime()<< " [SUCCESS] Injection completed!\033[0m" << std::endl;
        std::cin.get();
    }

    
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}
