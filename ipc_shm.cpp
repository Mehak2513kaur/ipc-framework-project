// Revision 4: Shared memory module updated
#include "ipc_shm.h"
#include "security.h"
#include "logger.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>

#define SHM_NAME L"MySharedMemory"   // Same for read + write
#define SHM_SIZE 1024                // Bigger size = safer

// ======================
//   SHARED MEMORY WRITE
// ======================
void shmWrite() {

    HANDLE hMap = CreateFileMappingW(
        INVALID_HANDLE_VALUE,     // Use RAM, not a file
        NULL,
        PAGE_READWRITE,
        0,
        SHM_SIZE,
        SHM_NAME                  // Must match in reader
    );

    if (hMap == NULL) {
        std::cout << "Failed to create shared memory. ERROR = " << GetLastError() << "\n";
        return;
    }

    void* mem = MapViewOfFile(
        hMap,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        SHM_SIZE
    );

    if (mem == NULL) {
        std::cout << "Failed to map shared memory. ERROR = " << GetLastError() << "\n";
        CloseHandle(hMap);
        return;
    }

    std::string msg;
    std::cout << "Enter message: ";
    std::cin.ignore();
    std::getline(std::cin, msg);

    // Encrypt message
    msg = encryptData(msg);

    // Clear block and copy data
    memset(mem, 0, SHM_SIZE);
    memcpy(mem, msg.c_str(), msg.size());

    std::cout << "Message written to shared memory.\n";
    logEvent("[SHM] Wrote encrypted message.");

    UnmapViewOfFile(mem);
    CloseHandle(hMap);
}


// ======================
//   SHARED MEMORY READ
// ======================
void shmRead() {

    HANDLE hMap = OpenFileMappingW(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        SHM_NAME
    );

    if (hMap == NULL) {
        std::cout << "No shared memory segment found. ERROR = " << GetLastError() << "\n";
        return;
    }

    void* mem = MapViewOfFile(
        hMap,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        SHM_SIZE
    );

    if (mem == NULL) {
        std::cout << "Failed to map shared memory (Read). ERROR = " << GetLastError() << "\n";
        CloseHandle(hMap);
        return;
    }

    // Convert memory buffer to string
    std::string encrypted((char*)mem);
    std::string decrypted = decryptData(encrypted);

    std::cout << "Shared Memory Contains: " << decrypted << "\n";
    logEvent("[SHM] Read & decrypted message.");

    UnmapViewOfFile(mem);
    CloseHandle(hMap);
}


