#ifndef LOGIN_SYSTEM_H
#define LOGIN_SYSTEM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

class LoginSystem {
public:
    static bool authenticateAdmin(const std::string& inputUsername, const std::string& inputPassword);
    static bool authenticateUser(const std::string& inputUsername, const std::string& inputPassword);

    static void afterSuccessfulAdminLogin();
    static void afterFailedAdminLogin();
    static void afterSuccessfulUserLogin();
    static void afterFailedUserLogin();
};
