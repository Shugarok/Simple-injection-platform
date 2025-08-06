#pragma once

#include <iomanip>
#include <chrono>
#include <sstream>
#include <windows.h>
#include <iostream>



int findProcID(const wchar_t* processName);
int injectorMain(int pid);
std::string currentTime();




const bool debuggingMode = true;   //Change to disable error,info,warn(log) information
const bool UseCreateRemoteThreadAfterNT = true; // Change to disable using CreateRemoteThread if NtCreateThreadEx crash