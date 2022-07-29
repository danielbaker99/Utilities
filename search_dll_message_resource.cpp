#include <iostream>
#include <windows.h>
#include <string.h>
#include <filesystem>
/*
#include <dsound.h>
*/
#include <winerror.h>
using std::wcout, std::endl;
int IterateDLLs(std::filesystem::path Path);
void HasMesssage(LPCWSTR filename);
int RecursiveIterateDLLs(std::filesystem::path Path);

constexpr DWORD err = CO_E_NOTINITIALIZED;
int IterateDLLs() {
    IterateDLLs(L"C:\\WINDOWS\\System32");
    IterateDLLs(L"C:\\WINDOWS\\syswow64");
    RecursiveIterateDLLs(L"C:\\Program Files (x86)\\Windows Kits");//\\10
    return 0;
}

int IterateDLLs(std::filesystem::path Path) {
    for (auto dirEntry : std::filesystem::directory_iterator(Path)) {
        if (dirEntry.path().extension() == L".dll") {
            HasMesssage(dirEntry.path().c_str());
        }
    }
    return 0;
}

int RecursiveIterateDLLs(std::filesystem::path Path) {
    for (auto dirEntry : std::filesystem::recursive_directory_iterator(Path)) {
        if (dirEntry.path().extension() == L".dll") {
            HasMesssage(dirEntry.path().c_str());
        }
    }
    return 0;
}

void HasMesssage(LPCWSTR filename) {
    HMODULE hMod = LoadLibraryExW(filename, NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);
    LPTSTR Buffer = NULL;
    DWORD ret = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
        (LPCVOID)hMod,
        err,
        NULL,
        reinterpret_cast<LPTSTR>(&Buffer),
        NULL,
        NULL
    );
    if (ret)
        wcout << filename << "\t" << Buffer << endl;
    LocalFree(Buffer);
}
