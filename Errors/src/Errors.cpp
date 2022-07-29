#include "errors_export.h"
#include <string>
/*
#include <Audioclient.h>
#include <dsound.h>
xcopy .\x64\Release\Errors.dll D:\Dev\test\wasapi /y
AUDCLNT_E_BUFFER_TOO_LARGE
0x20000001
CO_E_NOTINITIALIZED _HRESULT_TYPEDEF_(0x800401F0L)
#include <winerror.h>
using std::wcout, std::endl;
#include <iostream>
int main()
{
	Error error;// (&callback)
	//DWORD err = 0x80090328;
	SetLastError(1);
	std::wcout << error.Display();
	return 0;
}

const wchar_t* const Error::DisplayWin32Error() {
	HMODULE hMod = GetModuleHandleW(NULL);
	return DisplayMain(hMod, GetLastError());
}
*/
const wchar_t* const Error::DisplayWin32Error(DWORD err) {
	HMODULE hMod = GetModuleHandleW(NULL);
	return DisplayMain(hMod, err ? err : GetLastError());
}

const wchar_t* const Error::DisplayComError(HRESULT err) {
	HMODULE hMod = LoadLibraryExW(L"kernel32.dll", NULL,
		LOAD_LIBRARY_AS_IMAGE_RESOURCE | LOAD_LIBRARY_AS_DATAFILE);
	return DisplayMain(hMod,err);
}

const wchar_t* const Error::DisplayMain(HMODULE hMod,DWORD err) {
static wchar_t str[100];

if (!err) return NULL;

if (!(err >> 16)) 
	err = HRESULT_FROM_WIN32(err);
if ((err >> 16 & 0x7ff) == FACILITY_WIN32) {
	DWORD ret = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, (DWORD)err, NULL, reinterpret_cast<LPWSTR>(&Buffer), NULL, NULL);
	if (ret) return Buffer; else 
		return L"error in system message";
}
	else {
		if (err & 0x20000000) {
			if (callback) return callback(err); else return L"callback function not defined";
		}
		else {


			DWORD ret = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
				(LPCVOID)hMod, (DWORD)err, NULL, reinterpret_cast<LPWSTR>(&Buffer), NULL, NULL);
			if (ret) return Buffer; 
				else if 
					(GetLastError() == 317 || GetLastError()==1813)
				{
					swprintf(str, 100, L"Facility %d\nError code 0x%x\n", err >> 16 & 0x8ff, err);
					return str;
				}
				else
				return DisplayWin32Error();
		}
	}
	return static_cast<const wchar_t*>(str);
}
