// message resource.cpp : This file contains the 'main' function. Program execution begins and ends there.
//#include "sample.h"
//STRSAFE_E_INVALID_PARAMETER
//#include <winerror.h>//definitions
#include <windows.h>
#include <iostream>
#include <strsafe.h>
//#include "message.h"
#include "nserror.h"
using std::wcout, std::endl;
BOOL EnumTypesFunc(
    HMODULE hModule,  // module handle
    LPTSTR lpType,    // address of resource type
    LONG lParam); // extra parameter, could be
BOOL EnumNamesFunc(
    HMODULE hModule,
    LPCTSTR lpType,
    LPTSTR lpName,
    LONG lParam);
BOOL EnumLangsFunc(
    HMODULE hModule, // module handle
    LPCTSTR lpType,  // address of resource type
    LPCTSTR lpName,  // address of resource name
    WORD wLang,      // resource language
    LONG lParam);    // extra parameter, could be

int DisplayError(DWORD err);
int DisplaySystemError(DWORD err);

int enumerateResources() {
    HMODULE hExe = NULL;
    BOOL dwBool;
/* 
    hExe = LoadLibraryW(L"D:\\Dev\\utilities\\message resource\\Release\\message resource.exe");
    //kernel32.dll
    if (hExe == NULL)
    {
        wcout << L"load library error " << GetLastError() << endl;
        DisplayError(GetLastError());
        return 1;
    }
*/    
    dwBool = EnumResourceTypes(hExe,(ENUMRESTYPEPROC)EnumTypesFunc,0);
    if (!dwBool) DisplaySystemError(GetLastError());
    if (hExe) FreeLibrary(hExe);
    return 0;
}
int DisplaySystemError(DWORD err) {//D:\\Dev\\utilities\\message resource\\onelang.res"
   // const WCHAR* filename{ L"kernel32.dll" };
    HMODULE hMod = GetModuleHandleW(NULL);
    //LoadLibraryExW(filename, NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);
    LPTSTR Buffer = NULL;
    DWORD ret = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        (LPCVOID)NULL,
        err,
        NULL,
        reinterpret_cast<LPTSTR>(&Buffer),
        NULL,
        NULL
    );
    if (ret) wcout << Buffer << endl; 
    else
    {
        wcout << "error function error code" << GetLastError() << endl;
        exit(1);
    }
    return 0;
}
int DisplayError(DWORD err) {
    //L"D:\\Dev\\utilities\\message resource\\onelang.res"
   //L"kernel32.dll"
    HMODULE hMod = GetModuleHandleW(NULL);
    LPTSTR Buffer = NULL;
    DWORD ret = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
        (LPCVOID)hMod,
        (DWORD)err,
        NULL,
        reinterpret_cast<LPTSTR>(&Buffer),
        NULL,
        NULL
    );

    if (ret) wcout << Buffer << endl; else {
        wcout << "original error " << err << endl;
        DisplaySystemError(GetLastError());
    }
    return 0;
}

BOOL EnumTypesFunc(HMODULE hModule,LPTSTR lpType,LONG lParam){
    TCHAR szBuffer[80];
    DWORD cbWritten;
    size_t cbString;
    HRESULT hResult;
    if (!IS_INTRESOURCE(lpType))
    {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("Type: %s\r\n"), lpType);
        if (FAILED(hResult))
        {
            wcout << "error";
            return -1;
            return FALSE;
        }
    }
    else
    {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("Type: %u\r\n"), (USHORT)lpType);
        if (FAILED(hResult))
        {
            wcout << "error";
            return -2;
        }
    }
    wcout << szBuffer;
    hResult = StringCchLength(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), &cbString);
    if (FAILED(hResult))
    {
        wcout << "error";
        return -3;
    }
    /*
    WriteFile(g_hFile, szBuffer, (DWORD)cbString, &cbWritten, NULL);
    // Find the names of all resources of type lpType.
*/    
    EnumResourceNames(hModule,
        lpType,
        (ENUMRESNAMEPROC)EnumNamesFunc,
        0);

    return TRUE;
}

BOOL EnumNamesFunc(
    HMODULE hModule,  // module handle
    LPCTSTR lpType,   // address of resource type
    LPTSTR lpName,    // address of resource name
    LONG lParam)      // extra parameter, could be
                      // used for error checking
{
    TCHAR szBuffer[80];  // print buffer for info file
    DWORD cbWritten;     // number of bytes written to resource info file
    size_t cbString;
    HRESULT hResult;

    // Write the resource name to a resource information file.
    // The name may be a string or an unsigned decimal
    // integer, so test before printing.
    if (!IS_INTRESOURCE(lpName))
    {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("\tName: %s\r\n"), lpName);
        if (FAILED(hResult))
        {
            wcout << "error";
            return -4;
        }
    }
    else
    {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("\tName: %u\r\n"), (USHORT)lpName);
        if (FAILED(hResult))
        {
            wcout << "error";
            return -5;
        }
        wcout << szBuffer << endl;
        EnumResourceLanguages(hModule,
            lpType,
            lpName,
            (ENUMRESLANGPROC)EnumLangsFunc,
            0);
    }
}
BOOL EnumLangsFunc(
    HMODULE hModule, // module handle
    LPCTSTR lpType,  // address of resource type
    LPCTSTR lpName,  // address of resource name
    WORD wLang,      // resource language
    LONG lParam)     // extra parameter, could be
                     // used for error checking
{
    HRSRC hResInfo;
    TCHAR szBuffer[80];  // print buffer for info file
    DWORD cbWritten;     // number of bytes written to resource info file
    size_t cbString;
    HRESULT hResult;

    hResInfo = FindResourceEx(hModule, lpType, lpName, wLang);
    // Write the resource language to the resource information file.
    hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("\t\tLanguage: %u\r\n"), (USHORT)wLang);
    if (FAILED(hResult))
    {
        // Add code to fail as securely as possible.
        return FALSE;
    }
    wcout << szBuffer;
    hResult = StringCchLength(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), &cbString);
    if (FAILED(hResult))
    {
        // Add code to fail as securely as possible.
        return FALSE;
    }

    hResult = StringCchPrintf(szBuffer,
        sizeof(szBuffer) / sizeof(TCHAR),
        TEXT("\t\thResInfo == %lx,  Size == %lu\r\n\r\n"),
        hResInfo,
        SizeofResource(hModule, hResInfo));
    wcout << szBuffer << endl;
    HGLOBAL hResource;
    hResource = LoadResource(hModule, hResInfo);
    if (!hResource) {
        DisplaySystemError(GetLastError());
        return -1;
    }
    DWORD ResourceSize;
    ResourceSize = SizeofResource(hModule, hResInfo);
    if (!ResourceSize) {
        DisplaySystemError(GetLastError());
        return -1;
    }
    wcout << "resource size " << ResourceSize << endl;
    LPVOID pResource;
    pResource = LockResource(hResInfo);
    if (!pResource) {
        DisplaySystemError(GetLastError());
        return -1;
    }
    wcout << std::hex << pResource << endl;
    return 0;
}