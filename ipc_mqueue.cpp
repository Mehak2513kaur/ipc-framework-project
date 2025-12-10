#include "ipc_mqueue.h"
#include "security.h"
#include "logger.h"
#include <windows.h>
#include <iostream>

#define PIPE_NAME L"\\\\.\\pipe\\MyMsgQueue"

//
// =======================
//   MESSAGE QUEUE SEND
// =======================
//
void mqSend() {

    HANDLE pipe;

    // Try connecting until receiver is ready
    while (true) {
        pipe = CreateFileW(
            PIPE_NAME,
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (pipe != INVALID_HANDLE_VALUE)
            break;  // Connected successfully

        Sleep(200);  // Wait for receiver
    }

    std::string msg;
    std::cout << "Enter message: ";
    std::cin.ignore();
    std::getline(std::cin, msg);

    msg = encryptData(msg);

    DWORD written;
    BOOL ok = WriteFile(pipe, msg.c_str(), msg.size(), &written, NULL);

    if (!ok) {
        std::cout << "Failed to send message.\n";
    } else {
        std::cout << "Message sent.\n";
        logEvent("[MSG QUEUE] Sent encrypted message.");
    }

    CloseHandle(pipe);
}

//
// =======================
//  MESSAGE QUEUE RECEIVE
// =======================
//
void mqReceive() {

    HANDLE pipe = CreateNamedPipeW(
        PIPE_NAME,
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1, 256, 256, 0, NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create queue.\n";
        return;
    }

    std::cout << "Waiting for sender...\n";

    BOOL connected = ConnectNamedPipe(pipe, NULL);

    if (!connected) {
        if (GetLastError() != ERROR_PIPE_CONNECTED) {
            std::cout << "Failed to connect.\n";
            CloseHandle(pipe);
            return;
        }
    }

    char buffer[256] = {0};
    DWORD bytesRead = 0;

    BOOL ok = ReadFile(pipe, buffer, sizeof(buffer), &bytesRead, NULL);

    if (!ok || bytesRead == 0) {
        std::cout << "No data received.\n";
        CloseHandle(pipe);
        return;
    }

    std::string decrypted = decryptData(std::string(buffer, bytesRead));
    std::cout << "Received: " << decrypted << "\n";

    logEvent("[MSG QUEUE] Received & decrypted message.");

    CloseHandle(pipe);
}

