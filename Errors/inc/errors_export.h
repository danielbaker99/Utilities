#include <Windows.h>
class __declspec(dllexport) Error {
private:
	LPCWSTR Buffer = NULL;
	typedef const wchar_t*( * func_ptr )(HRESULT err);
	func_ptr callback{};
	const wchar_t* const DisplayMain(HMODULE hMod, DWORD err);
public:
	const wchar_t* const DisplayWin32Error(DWORD err = NULL);
	const wchar_t* const DisplayComError(HRESULT err);

	explicit Error(func_ptr _callback) : callback{ _callback } {}
	Error() = default;
	Error& operator=(const Error&) = default;
	~Error() {
		//if (Buffer) 
		//	LocalFree(&Buffer);
	}

};
/*
const wchar_t* callback(HRESULT err) {
	const wchar_t* ret;
	switch (err) {
	case 0x20000001:
		ret = L"custom error 1\n";
		break;
	default:
		ret = L"undefined custom error\n";
	}
	return ret;
}
*/