#include "main.h"


//Inject DLL path
wchar_t dllPath[] = L"";



unsigned int dllSiz = static_cast<unsigned int>((wcslen(dllPath) + 1) * sizeof(wchar_t));





LPVOID getLoadLibraryAddress() {
    LPVOID lb = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
    if (lb == NULL) {
        log(logLevel::ERRORR, injectionStage::LOAD_LIBRARY_ADDR, "Can't get LoadLibraryW address");
        return NULL;
    }
    return lb;

}



int openProc(int pid) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
    if (hProcess == NULL) {
        log(logLevel::ERRORR, injectionStage::OPEN_PROCESS, "Can't open process");
        return -1;
    }
    else {
        log(logLevel::INFO, injectionStage::OPEN_PROCESS, "Process opened successfully");
        return alocForProc(hProcess);
    }

}



int alocForProc(HANDLE hProcess) {

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllSiz, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteMemory == NULL) {
        log(logLevel::ERRORR, injectionStage::ALLOC_MEMORY, "Can't allocate memory");
        CloseHandle(hProcess);
        return -1;
    }
    else {
        log(logLevel::INFO, injectionStage::ALLOC_MEMORY, "Memory allocated");
        return writeDLL(hProcess, pRemoteMemory);
    }
}






int writeDLL(HANDLE hProcess, LPVOID pRemoteMemory) {

    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath, dllSiz, NULL)) {
        log(logLevel::ERRORR, injectionStage::WRITE_MEMORY, "Can't write memory, exiting");
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

    else {
        log(logLevel::INFO, injectionStage::WRITE_MEMORY, "DLL path written");
        ntCTE(hProcess, pRemoteMemory);
        return 0;
    }
}





int ntCTE(HANDLE hProcess, LPVOID pRemoteMemory) {
    HANDLE hThread = NULL;
    NTSTATUS status = 0;

    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (!hNtdll) {
        log(logLevel::WARN, injectionStage::NTDLL_LOAD, "Can't load ntdll.dll");
        return ntCTEFall(hProcess, pRemoteMemory, false, NULL);
    }

    pNtCreateThreadEx NtCTE = (pNtCreateThreadEx)GetProcAddress(hNtdll, "NtCreateThreadEx");
    if (!NtCTE) {
        log(logLevel::WARN, injectionStage::NTDLL_FIND, "Can't find NtCreateThreadEx");
        return ntCTEFall(hProcess, pRemoteMemory, false, NULL);
    }

    status = NtCTE(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, getLoadLibraryAddress(), pRemoteMemory, 0, 0, 0, 0, NULL);

    if (status >= 0 && hThread) {
        log(logLevel::INFO, injectionStage::NTDLL_CREATE_THREAD, "NtCreateThreadEx succeeded");
        WaitForSingleObject(hThread, INFINITE);
        log(logLevel::SUCCESS, injectionStage::COMPLETE, "Injected!!!");
        return closeALL(hProcess, pRemoteMemory, hThread);
    }
    else {
        log(logLevel::WARN, injectionStage::NTDLL_CREATE_THREAD, "NtCreateThreadEx failed");
        return ntCTEFall(hProcess, pRemoteMemory, false, hThread);
    }
}






int ntCTEFall(HANDLE hProcess, LPVOID pRemoteMemory, bool ntSuccess, HANDLE hThread) {



    if (UseCreateRemoteThreadAfterNT && ntSuccess != NULL) {
        hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)getLoadLibraryAddress(), pRemoteMemory, 0, NULL);

        if (hThread == NULL) {
            log(logLevel::ERRORR, injectionStage::CREATE_REMOTE_THREAD, "CreateRemoteThread failed, exiting");
            VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return -1;
        }

        log(logLevel::INFO, injectionStage::CREATE_REMOTE_THREAD, "Using CreateRemoteThread");

        WaitForSingleObject(hThread, INFINITE);


        log(logLevel::SUCCESS, injectionStage::COMPLETE, "Injected!!!");
        return closeALL(hProcess, pRemoteMemory, hThread);

    }
    log(logLevel::ERRORR, injectionStage::CREATE_REMOTE_THREAD, "UseCreateRemoteThreadAfterNT disabled, exiting \n(You can change the order of actions in main.h)");
    return -1;
}



int closeALL(HANDLE hProcess, LPVOID pRemoteMemory, HANDLE hThread) {
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    ExitProcess(0);
    return 0;
}



int injectorMain(int pid) {

    return openProc(pid);


}
