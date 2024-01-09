#include "User.h"
#include <fstream>
#include <json/json.h>
#include <random>
#include <sstream>
#include <filesystem>  
#include <iostream>

const std::string User::charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

User::User(const std::string& username, const std::string& password, const std::string& role)
    : id(generateId()), username(username), password(encrypt(password)), role(role) {}

std::string User::generateId() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, charset.size() - 1);

    std::stringstream ss;
    for (int i = 0; i < 4; ++i) {
        ss << charset[distribution(generator)];
    }

    return ss.str();
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

std::string User::getRole() const {
    return role;
}

std::string User::getId() const {
    return id;
}

void User::saveToFile(const std::string& filename) const {
    Json::Value userJson;
    userJson["id"] = id;
    userJson["username"] = username;
    userJson["password"] = password;  
    userJson["role"] = role;

    std::ifstream ifs(filename, std::ifstream::binary);
    Json::Value usersJson;

    if (ifs.good()) {
        ifs >> usersJson;
        ifs.close();
    }

    usersJson[id] = userJson;

    std::ofstream ofs(filename, std::ofstream::trunc);
    ofs << usersJson;
    ofs.close();


    createFolder();
}

void User::createFolder() const {

    std::filesystem::create_directory(id);
    
}

std::string User::encrypt(const std::string& input) {
    
    std::string result = input;
    for (char& c : result) {
        c = charset[(c + 1) % charset.size()];
    }
    return result;
}

std::string User::decrypt(const std::string& input) {
    
    std::string result = input;
    for (char& c : result) {
        c = charset[(c - 1 + charset.size()) % charset.size()];
    }
    return result;
}

bool User::authenticateFromFile(const std::string& filename, const std::string& inputUsername, const std::string& inputPassword) {
    std::vector<User> users = loadUsersFromFile(filename);

    for (const User& user : users) {
        if (user.getUsername() == inputUsername && user.decrypt(user.getPassword()) == inputPassword) {
            std::cout << "Uspešna prijava!\n";
            return true;
        }
    }

    std::cout << "Neuspešna prijava.\n";
    return false;
}

std::vector<User> User::loadUsersFromFile(const std::string& filename) {
    std::ifstream ifs(filename, std::ifstream::binary);
    Json::Value usersJson;

    if (ifs.good()) {
        ifs >> usersJson;
        ifs.close();
    }

    std::vector<User> users;

    for (const auto& id : usersJson.getMemberNames()) {
        const Json::Value& userJson = usersJson[id];
        users.emplace_back(userJson["username"].asString(), userJson["password"].asString(), userJson["role"].asString());
    }

    return users;
}
