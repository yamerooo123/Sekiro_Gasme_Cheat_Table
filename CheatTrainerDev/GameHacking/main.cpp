#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <cwchar>
#include <iostream>

using namespace std;


int GetProcID();



int main() {

    string GetProcIDByName;
    DWORD processId{};

    cout << "Enter target process name: ";
    cin >> GetProcIDByName;

    // snapshot processes
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cout << "[!] ERROR: CreateToolhelp32Snapshot returns INVALID_HANDLE_VALUE\n";
    }
    else {
        cout << "[*] CreateToolhelp32Snamshot works correctly!\n";
    }

    // init dwSize to prep for Process32First
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    // init Process32First
    BOOL Process32First(HANDLE hProcessSnap,LPPROCESSENTRY32 pe32);

    // call Process32First
    if (!Process32First(hProcessSnap, &pe32)) 
    {
        cout << "[!] ERROR: Process32First returns false (0).\n";
    }
    else {

        do {
            
            // wprintf(L"%s\n", pe32.szExeFile);

            if (wcscmp(pe32.szExeFile,L"sekiro.exe") == 0) {

                processId = pe32.th32ProcessID;

                break;
            }
           
        } while (Process32Next(hProcessSnap, &pe32));

    }
    
    if (processId == 0) {
        
        printf("[!] Sekiro.exe not found!");
    }
    else {
        printf("[*] Sekiro processID is: %u", processId);
    }

    return 0;
}

int GetProcID() {

    return 0;
}


