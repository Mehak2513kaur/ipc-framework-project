// Revision 5: Security module updated
#include "security.h"
#include <iostream>

bool authenticate() {
    std::string pass;
    std::cout << "Enter Password: ";
    std::cin >> pass;

    return pass == "admin123";
}

std::string encryptData(const std::string &msg, char key) {
    std::string out = msg;
    for (char &c : out) c ^= key;
    return out;
}

std::string decryptData(const std::string &msg, char key) {
    return encryptData(msg, key);
}

