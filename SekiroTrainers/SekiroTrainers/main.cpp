#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <cwchar>
#include <iostream>
#include <vector>

using namespace std;

// function declaration
uintptr_t GetBaseMemoryAddrByProcID(DWORD processId);
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

int main() {

    DWORD processId{};

    // snapshot processes
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cout << "[!] ERROR: CreateToolhelp32Snapshot returns INVALID_HANDLE_VALUE\n";
    }
    else {
        cout << "[+] CreateToolhelp32Snapshot works correctly!\n";
    }

    // init dwSize to prep for Process32First
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    // init Process32First
    BOOL Process32First(HANDLE hProcessSnap, LPPROCESSENTRY32 pe32);

    // call Process32First
    if (!Process32First(hProcessSnap, &pe32))
    {
        cout << "[!] ERROR: Process32First returns false (0).\n";
    }
    else {

        do {

            // wprintf(L"%s\n", pe32.szExeFile);

            if (wcscmp(pe32.szExeFile, L"sekiro.exe") == 0) {

                // get Sekiro PID
                processId = pe32.th32ProcessID;

                break;
            }

        } while (Process32Next(hProcessSnap, &pe32));

    }

    if (processId == 0) {

        printf("[!] Sekiro.exe not found! \n");
        printf("[!] Exit... \n");
    }
    else {
        printf("[*] Sekiro processID is: %u \n", processId);

        // get base memory addrss of sekiro.exe
        uintptr_t SekiroBasePointer = GetBaseMemoryAddrByProcID(processId);

        if (SekiroBasePointer == 0) {

            cout << "[!] GetBaseMemoryAddrByProcID returns 0!";
        }

        // get sekiro handle
        HANDLE hSekiro = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId);
        
        if (hSekiro == NULL) {

            cout << "[!] Couldn't get Sekiro handle! \n";
        }
        else {

            cout << "[+] Succesfully retrieved Sekiro process handle. \n";
        }

        // offsets from Cheat Engine pointermap scanning
        vector<unsigned int> ammoOffsets = { 0x8, 0xC78 };

        // get ammo pointer address
        uintptr_t ammoCheat = FindDMAAddy(hSekiro, SekiroBasePointer + 0x03D5AAC0, ammoOffsets);

        if (ammoCheat == 0) {

            cout << "[!] Couldn't retrieve ammo offset! \n";

        }

        else {
            
            cout << "[+] Ammo offset address: " << hex << ammoCheat << "\n";
        
        }

    }

    return 0;
}


// use PID in CreateToolhelp32Snapshot
uintptr_t GetBaseMemoryAddrByProcID(DWORD processId) {

    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

    if (hModuleSnap == INVALID_HANDLE_VALUE) {

        cout << "[!] ERROR: CreateToolhelp32Snapshot returns INVALID_HANDLE_VALUE\n";
    }
    else {
        cout << "[*] Now finding base memory address of PID: " << processId << "\n";
    }

    // init dwSize to prep for Module32First
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(me32);

    if (!Module32First(hModuleSnap, &me32))
    {
        cout << "[!] ERROR: Module32First returns FALSE\n";
    }
    else {

        printf("[+] Sekiro.exe base address is: %p\n", me32.modBaseAddr);

        uintptr_t baseAddr = reinterpret_cast<uintptr_t>(me32.modBaseAddr);

        return baseAddr;
    
    }

    return 0;
}

// locate multi-level pointers
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}
