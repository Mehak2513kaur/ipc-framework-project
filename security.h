#pragma once
#include <string>

bool authenticate();
std::string encryptData(const std::string &msg, char key='K');
std::string decryptData(const std::string &msg, char key='K');
