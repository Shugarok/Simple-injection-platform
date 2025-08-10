#include "main.h"


#ifdef _M_X64
#define PPEB_CURRENT ((PPEB)__readgsqword(0x60))
#else
#define PPEB_CURRENT ((PPEB)__readfsdword(0x30))
#endif



bool CheckDllLoaded(const wchar_t* dllName) {
    PPEB peb = PPEB_CURRENT;
    PPEB_LDR_DATA ldr = peb->Ldr;
    LIST_ENTRY* list = &ldr->InLoadOrderModuleList;
    LIST_ENTRY* current = list->Flink;

    while (current != list) {
        PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(current, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        if (_wcsicmp(entry->BaseDllName.Buffer, dllName) == 0) {
            return true;
        }
        current = current->Flink;
    }

    return false;
}



int AntiVMMain() {
    SYSTEM_INFO si;
    MEMORYSTATUSEX msx{ sizeof(msx) };

    GetSystemInfo(&si);
    GlobalMemoryStatusEx(&msx);

    if (GetTickCount() < 60000) {
        log(logLevel::WARN, injectionStage::ANTI_VM, "System work time < 1min, exiting");
        ExitProcess(-1);
    }

    
    if (si.dwNumberOfProcessors <= 2){
        log(logLevel::WARN, injectionStage::ANTI_VM, "NumberOfProcessors  <= 2, exiting");
        ExitProcess(-1);
    }

        if (msx.ullTotalPhys < (2ULL << 30)) {          
            log(logLevel::WARN, injectionStage::ANTI_VM, "RAM < 2gb");
            ExitProcess(-1);

    }

    for (const auto& dllName : szDlls) {
        if (CheckDllLoaded(dllName)) {
            log(logLevel::WARN, injectionStage::ANTI_VM, "Loaded suspect DLL, exiting \n(You can change the order of actions in main.h)");
            ExitProcess(-1);

        }

    }

    log(logLevel::INFO, injectionStage::ANTI_VM, "Suspect DLL not found, continue...");
    return 0;
}
