#pragma once

#include <windows.h>
#include <iostream>


int findProcID(const wchar_t* processName);
int injectorMain(int pid);
