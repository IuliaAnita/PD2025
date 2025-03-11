// ServiciiWindows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include<winreg.h>




int main() {
	HKEY hKey;
	const wchar_t* servicesPath = L"SYSTEM\\CurrentControlSet\\Services";

	// Deschidem cheia Services
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, servicesPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		std::wcerr << L"Eroare la deschiderea cheii Services\n";
		return 1;
	}

	DWORD index = 0;
	WCHAR subkeyName[256];
	DWORD subkeyNameSize;

	while (true) {
		subkeyNameSize = 256;
		if (RegEnumKeyEx(hKey, index, subkeyName, &subkeyNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
			break;
		}

		// Construim cheia completă pentru fiecare subcheie
		HKEY hSubKey;
		std::wstring fullSubkeyPath = std::wstring(servicesPath) + L"\\" + subkeyName;

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, fullSubkeyPath.c_str(), 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
			WCHAR imagePath[1024];
			DWORD size = sizeof(imagePath);
			DWORD type = 0;

			if (RegQueryValueEx(hSubKey, L"ImagePath", NULL, &type, (LPBYTE)imagePath, &size) == ERROR_SUCCESS) {
				if (type == REG_EXPAND_SZ || type == REG_SZ) {
					std::wcout << subkeyName << L" => " << imagePath << std::endl;
				}
			}

			RegCloseKey(hSubKey);
		}

		index++;
	}

	RegCloseKey(hKey);
	return 0;
}