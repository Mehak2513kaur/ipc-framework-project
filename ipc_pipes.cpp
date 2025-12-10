// Revision 2: Pipes module updated
#include "ipc_pipes.h"
#include "security.h"
#include "logger.h"
#include <windows.h>
#include <iostream>

void pipeSend() {
    HANDLE hRead, hWrite;
    CreatePipe(&hRead, &hWrite, NULL, 0);

    std::string msg;
    std::cout << "Enter message: ";
    std::cin.ignore();
    std::getline(std::cin, msg);

    msg = encryptData(msg);
    DWORD written;
    WriteFile(hWrite, msg.c_str(), msg.size(), &written, NULL);

    logEvent("[PIPE] Sent encrypted message.");

    CloseHandle(hWrite);
    CloseHandle(hRead);
}

void pipeReceive() {
    HANDLE hRead, hWrite;
    CreatePipe(&hRead, &hWrite, NULL, 0);

    char buffer[256];
    DWORD read;
    ReadFile(hRead, buffer, sizeof(buffer), &read, NULL);

    std::string decrypted = decryptData(std::string(buffer, read));
    std::cout << "Received: " << decrypted << std::endl;

    logEvent("[PIPE] Received & decrypted message.");

    CloseHandle(hWrite);
    CloseHandle(hRead);
}

