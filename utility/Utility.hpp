#ifndef Utility_hpp
#define Utility_hpp
#define RUNNING_WINDOWS

#include <iostream>
#include <string>
#include <algorithm>

namespace Utility {
    #ifdef RUNNING_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <Shlwapi.h>

    inline std::string wideStringToString(const std::wstring& wstr) {
        std::string str(wstr.begin(), wstr.end());
        return str;
    }

    inline std::string workingDirectory() {
        HMODULE hModule = GetModuleHandle(nullptr);
        if(hModule) {
            wchar_t path[256];
            GetModuleFileNameW(hModule, path, sizeof(path) / sizeof(wchar_t));
            PathRemoveFileSpecW(path);
            wcscat(path, L"\\"); // new
            return wideStringToString(path); // new
        }
        return "";
    }
    #elif defined RUNNING_LINUX
    #include <unistd.h>
    #include <unistd.h>
    inline std::string GetWorkingDirectory() {
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) != nullptr){
            return std::string(cwd) + std::string("/");
        }
        return "";
    }
    #endif
}

#endif