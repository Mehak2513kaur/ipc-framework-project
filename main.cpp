// Revision 6: Demo controller module updated
#include <iostream>
#include "security.h"
#include "ipc_pipes.h"
#include "ipc_mqueue.h"
#include "ipc_shm.h"

int main() {
    if (!authenticate()) {
        std::cout << "Authentication Failed!\n";
        return 0;
    }

    int choice;
    while (true) {
        std::cout << "\n==== IPC FRAMEWORK MENU ====\n";
        std::cout << "1. Pipe Send\n";
        std::cout << "2. Pipe Receive\n";
        std::cout << "3. Message Queue Send\n";
        std::cout << "4. Message Queue Receive\n";
        std::cout << "5. Shared Memory Write\n";
        std::cout << "6. Shared Memory Read\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: pipeSend(); break;
            case 2: pipeReceive(); break;
            case 3: mqSend(); break;
            case 4: mqReceive(); break;
            case 5: shmWrite(); break;
            case 6: shmRead(); break;
            case 0: return 0;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

