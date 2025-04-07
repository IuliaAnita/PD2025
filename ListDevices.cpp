#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

int main() {
    // Obține informații despre dispozitivele USB active
    GUID usbGuid = GUID_DEVCLASS_USB;
    HDEVINFO listaDispozitive = SetupDiGetClassDevs(
        &usbGuid,
        nullptr,
        nullptr,
        DIGCF_PRESENT | DIGCF_PROFILE
    );

    if (listaDispozitive == INVALID_HANDLE_VALUE) {
        std::cerr << "Nu s-au putut accesa dispozitivele conectate prin USB." << std::endl;
        return -1;
    }

    SP_DEVINFO_DATA infoDispozitiv;
    infoDispozitiv.cbSize = sizeof(SP_DEVINFO_DATA);

    DWORD index = 0;
    while (SetupDiEnumDeviceInfo(listaDispozitive, index, &infoDispozitiv)) {
        WCHAR descriere[1024];
        DWORD dimensiuneDescriere = 0;

        BOOL reusit = SetupDiGetDeviceRegistryPropertyW(
            listaDispozitive,
            &infoDispozitiv,
            SPDRP_DEVICEDESC,
            nullptr,
            reinterpret_cast<PBYTE>(descriere),
            sizeof(descriere),
            &dimensiuneDescriere
        );

        if (reusit) {
            std::wcout << L"[USB] -> " << descriere << std::endl;
        }

        ++index;
    }

    SetupDiDestroyDeviceInfoList(listaDispozitive);
    return 0;
}
