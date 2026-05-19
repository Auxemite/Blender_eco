#include <iostream>
#include "utils.hh"

#ifdef OS_WIN
#include <windows.h>
#endif

namespace Graphics {

#ifdef OS_WIN
bool running_in_debugger() {
    return IsDebuggerPresent();
}
#endif

void break_in_debugger() {
#ifdef OS_WIN
if(running_in_debugger()) {
    DebugBreak();
}
#endif
#ifdef OS_LINUX
    static bool handler_setup = false;
    if(!handler_setup) {
        std::signal(SIGTRAP, [](int) {});
        handler_setup = true;
    }
    std::raise(SIGTRAP);
#endif
}

void fatal(const char *msg, const char *file, int line) {
    std::cerr << msg;

    if (file) {
        std::cerr << " in file \"" << file << "\"";
    }
    if (line) {
        std::cerr << " at line " << line;
    }

    std::cerr << std::endl;

    break_in_debugger();
    std::terminate();
}

}