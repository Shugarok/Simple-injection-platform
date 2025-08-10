#include "main.h"


int main()
{
    AntiVMMain();
    log(logLevel::INFO, injectionStage::ANTI_VM_FINISH, "No VM detected, continue...");
    mainAntiDebug();
    log(logLevel::INFO, injectionStage::ANTI_DEBUG, "No debugger detected, continue...");
    //Process name with .exe(example: notepad.exe)
    int pid = findProcID(L"");
    if (pid == -1) {
        log(logLevel::ERRORR, injectionStage::FIND_PID, "Can't find the process, exiting");
        std::cin.get();
        return -1;
    }
    else 
        log(logLevel::INFO, injectionStage::FIND_PID, "PID = " + std::to_string(pid));
        

    injectorMain(pid);

    return 0;
}



std::string currentTime() {
   
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);


    std::tm tm_buffer;
    localtime_s(&tm_buffer, &time);  // linux (localtime_r)
 

  
    std::ostringstream oss;
    oss << std::put_time(&tm_buffer, "[%T.");  // [HH:MM:SS.

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    oss << std::setfill('0') << std::setw(3) << ms.count() << "] ";

    return oss.str();
}
