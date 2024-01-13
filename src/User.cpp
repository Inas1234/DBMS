#include <fstream>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <filesystem>  
#include <iostream>
#include "User.h"

const std::string User::charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

User::User(const std::string& username, const std::string& password, const std::string& role)
    : id(generateId()), username(username), password(password), role(role) {}

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
    return this->id;
}

void User::saveToFile(const std::string& filename) const {
    nlohmann::json userJson;
    userJson["id"] = id;
    userJson["username"] = username;
    userJson["password"] = encrypt(password);  
    userJson["role"] = role;

    std::ifstream ifs(filename, std::ifstream::binary);
    nlohmann::json usersJson;

    if (ifs.good()) {
        ifs >> usersJson;
        ifs.close();
    }

    usersJson[id] = userJson;

    std::ofstream ofs(filename, std::ofstream::trunc);
    ofs << usersJson;
    ofs.close();


    createFolder("data");
}

void User::createFolder(const std::filesystem::path& p) const {

    std::filesystem::path folderPath = p / id;
    std::filesystem::create_directory(folderPath);
    
}

std::string User::encrypt(const std::string& input) {
    static const int shift = 1; // Add a static member variable shift
    
    std::string result = input;
    for (char& c : result) {
        size_t pos = charset.find(c);
        if (pos != std::string::npos) {
            c = charset[(pos + shift) % charset.size()]; // Use the static member variable shift
        }
    }
    return result;
}

std::string User::decrypt(const std::string& input) {
    static const int shift = 1; // Add a static member variable shift

    std::string result = input;
        for (char& c : result) {
            size_t pos = charset.find(c);
            if (pos != std::string::npos) {
                c = charset[(pos - shift + charset.size()) % charset.size()];
            }
        }
    return result;
}

std::string User::authenticateFromFile(const std::string& filename, const std::string& inputUsername, const std::string& inputPassword) {
    std::vector<std::pair<std::string, User>> users = loadUsersFromFile(filename);

    for (const auto& userPair : users) {
        const std::string& userId = userPair.first;
        const User& user = userPair.second;

        //std::cout << user.getUsername() << " " << user.decrypt(user.getPassword()) << std::endl;

        if (user.getUsername() == inputUsername && user.decrypt(user.getPassword()) == inputPassword) {
            std::cout << "Login successful!\n";
            return userId;
        }
    }

    std::cout << "Login failed.\n";
    return "";  // Return an empty string to indicate failure
}



std::vector<std::pair<std::string, User>> User::loadUsersFromFile(const std::string& filename) {
    std::ifstream ifs(filename, std::ifstream::binary);
    nlohmann::json usersJson;

    if (ifs.good()) {
        ifs >> usersJson;
        ifs.close();
    }

    std::vector<std::pair<std::string, User>> users;

    for (const auto& entry : usersJson.items()) {
        const std::string& userId = entry.key();
        const nlohmann::json& userJson = entry.value();

        users.emplace_back(userId,
                           User(userJson["username"].get<std::string>(),
                                userJson["password"].get<std::string>(),
                                userJson["role"].get<std::string>()));
    }

    return users;
}

void User::setWorkDir(const std::string& foldername){
    std::filesystem::current_path("./data/"+foldername+"/");
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout<<"current path: "<<currentPath<<std::endl;
}

