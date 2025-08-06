#include "main.h"
#include <tlhelp32.h>


int findProcID(const wchar_t* procName) {

	int procID = -1;
	HANDLE snapshot;
	PROCESSENTRY32 pe;

	pe.dwSize = sizeof(PROCESSENTRY32);


	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) return 1;


	Process32First(snapshot, &pe);

	if (wcscmp(pe.szExeFile, procName) == 0) {
		procID = pe.th32ProcessID;
		CloseHandle(snapshot);

		return procID;

	}
	
	else
		while (Process32Next(snapshot, &pe)) {
			if (wcscmp(pe.szExeFile, procName) == 0) {
				procID = pe.th32ProcessID;
				break;
			}
		}



	CloseHandle(snapshot);
	return procID;
}
