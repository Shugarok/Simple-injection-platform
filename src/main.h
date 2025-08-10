#pragma once

#include <iomanip>
#include <chrono>
#include <sstream>
#include <windows.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <set>


const bool debuggingMode = true;   //Change to disable error,info,warn(log) information
const bool UseCreateRemoteThreadAfterNT = true; // Change to disable using CreateRemoteThread if NtCreateThreadEx crash!!!!


//AntiVM

static constexpr const wchar_t* szDlls[] = { L"avghookx.dll", L"avghooka.dll", L"snxhk.dll", L"sbiedll.dll", L"dbghelp.dll", L"api_log.dll", L"dir_watch.dll", L"pstorec.dll", L"vmcheck.dll", L"wpespy.dll", L"cmdvrt64.dll", L"cmdvrt32.dll" };


//AntiVM
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;


typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID      DllBase;
    PVOID      EntryPoint;
    ULONG      SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


typedef struct _PEB_LDR_DATA {
    ULONG      Length;
    BOOLEAN    Initialized;
    PVOID      SsHandle;
    LIST_ENTRY InLoadOrderModuleList;

} PEB_LDR_DATA, * PPEB_LDR_DATA;


typedef struct _PEB {
    BYTE Padding1[0x18];
    PPEB_LDR_DATA Ldr;
} PEB, * PPEB;
// -------------------------------------------------------------


//For NtCreateThreadEx
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

// -----------------------------------------------------


//For logs
enum class logLevel {
    INFO,
    WARN,
    ERRORR,
    SUCCESS

};

enum class injectionStage {
    INIT = 0,
    ANTI_VM,
    ANTI_VM_FINISH,
    ANTI_DEBUG,
    ANTI_DEBUG_FINISH,
    FIND_PID,
    LOAD_LIBRARY_ADDR,
    OPEN_PROCESS,
    ALLOC_MEMORY,
    WRITE_MEMORY,
    NTDLL_FIND,
    NTDLL_LOAD,
    NTDLL_CREATE_THREAD,
    CREATE_REMOTE_THREAD,
    CLEANUP,
    COMPLETE
};
// --------------------------------------------------------------------------




void log(logLevel level, injectionStage stage, const std::string& msg);
int findProcID(const wchar_t* processName);
int injectorMain(int pid);
std::string currentTime();
int AntiVMMain();
void mainAntiDebug();
int openProc(int pid);
int alocForProc(HANDLE hProcess);
int writeDLL(HANDLE hProcess, LPVOID pRemoteMemory);
int ntCTE(HANDLE hProcess, LPVOID pRemoteMemory);
int ntCTEFall(HANDLE hProcess, LPVOID pRemoteMemory, bool ntSuccess, HANDLE hThread);
int closeALL(HANDLE hProcess, LPVOID pRemoteMemory, HANDLE hThread);