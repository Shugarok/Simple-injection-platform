#include "main.h"


const char* to_string(injectionStage stage) {
    switch (stage) {
    case injectionStage::INIT: return "INIT";
    case injectionStage::ANTI_VM: return "ANTI_VM";
    case injectionStage::ANTI_VM_FINISH: return "ANTI_VM_FINISH";
    case injectionStage::ANTI_DEBUG: return "ANTI_DEBUG";
    case injectionStage::ANTI_DEBUG_FINISH: return "ANTI_DEBUG_FINISH";
    case injectionStage::FIND_PID: return "FIND_PID";
    case injectionStage::LOAD_LIBRARY_ADDR: return "LOAD_LIBRARY_ADDR";
    case injectionStage::OPEN_PROCESS: return "OPEN_PROCESS";
    case injectionStage::ALLOC_MEMORY: return "ALLOC_MEMORY";
    case injectionStage::WRITE_MEMORY: return "WRITE_MEMORY";
    case injectionStage::NTDLL_FIND: return "NTDLL_FIND";
    case injectionStage::NTDLL_LOAD: return "NTDLL_LOAD";
    case injectionStage::NTDLL_CREATE_THREAD: return "NTDLL_CREATE_THREAD";
    case injectionStage::CREATE_REMOTE_THREAD: return "CREATE_REMOTE_THREAD";
    case injectionStage::CLEANUP: return "CLEANUP";
    case injectionStage::COMPLETE: return "COMPLETE";
    default: return "UNKNOWN_STAGE";
    }
}

const char* to_string(logLevel level) {
    switch (level) {
    case logLevel::INFO: return "[INFO] ";
    case logLevel::WARN: return "[WARN] ";
    case logLevel::ERRORR: return "[ERROR] ";
    case logLevel::SUCCESS: return "[SUCCESS] ";
    default: return "[UNKNOWN] ";
    }
}

const char* get_color(logLevel level) {
    switch (level) {
    case logLevel::INFO: return "\033[34m";     
    case logLevel::WARN: return "\033[33m";     
    case logLevel::ERRORR: return "\033[31m";   
    case logLevel::SUCCESS: return "\033[32m";  
    default: return "\033[0m";
    }
}

void log(logLevel level, injectionStage stage, const std::string& msg) {
    if (!debuggingMode) return;

    const char* color = get_color(level);
    const char* levelStr = to_string(level);
    const char* stageStr = to_string(stage);

    std::cerr << color  << currentTime() << levelStr << " (" << stageStr << ") "<< msg<< "\033[0m" << std::endl;
}
