#include "main.h"


int main()
{
    
    int pid = findProcID(L"notepad.exe");
    if (pid == -1 && debuggingMode == true) {
        std::cerr << "\033[31m"<< currentTime() << "[ERROR] Can't find the process, exiting \033[0m" << std::endl;
        std::cin.get();
        return -1;
    }
    else if(debuggingMode == true)
        std::cerr << "\033[34m" << currentTime() << "[INFO] PID = "<< pid << "\033[0m" << std::endl;


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
