#pragma once
#include <string>
#include <iostream>
#include <windows.h>

namespace FastFunctions
{
    static std::string WStringToUTF8(const std::wstring& wstr)
    {
        if (wstr.empty()) return {};
        int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        if (size <= 0) return {};
        std::string str(size, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), str.data(), size, nullptr, nullptr);
        return str;
    }

    static std::wstring UTF8ToWString(const std::string& str)
    {
        if (str.empty()) return {};
        int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
        if (size <= 0) return {};
        std::wstring wstr(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), wstr.data(), size);
        return wstr;
    }
}