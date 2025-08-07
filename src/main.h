#pragma once

#include <iomanip>
#include <chrono>
#include <sstream>
#include <windows.h>
#include <iostream>






//FOR NtCreateThreadEx
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



//For log.cpp
enum class logLevel {
    INFO,
    WARN,
    ERRORR,
    SUCCESS

};

enum class injectionStage {
    INIT = 0,
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






void log(logLevel level, injectionStage stage, const std::string& msg);
int findProcID(const wchar_t* processName);
int injectorMain(int pid);
std::string currentTime();


int openProc(int pid);
int alocForProc(HANDLE hProcess);
int writeDLL(HANDLE hProcess, LPVOID pRemoteMemory);
int ntCTE(HANDLE hProcess, LPVOID pRemoteMemory);
int ntCTEFall(HANDLE hProcess, LPVOID pRemoteMemory, bool ntSuccess, HANDLE hThread);
int closeALL(HANDLE hProcess, LPVOID pRemoteMemory, HANDLE hThread);

const bool debuggingMode = true;   //Change to disable error,info,warn(log) information
const bool UseCreateRemoteThreadAfterNT = true; // Change to disable using CreateRemoteThread if NtCreateThreadEx crash!!!!