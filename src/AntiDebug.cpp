#include "main.h"


BOOL bDebuggerPresent;


void mainAntiDebug() {
	if (IsDebuggerPresent()){
		log(logLevel::WARN, injectionStage::ANTI_DEBUG, "Find debugger");
		ExitProcess(-1);

	}

	if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &bDebuggerPresent) == TRUE && bDebuggerPresent == TRUE ){
		log(logLevel::WARN, injectionStage::ANTI_DEBUG, "Find debugger");
		ExitProcess(-1);

	}

}	
