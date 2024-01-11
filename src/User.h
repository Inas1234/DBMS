#pragma once
#include <string>
#include <nlohmann/json.hpp>

class User {
public:
    User(const std::string& username, const std::string& password, const std::string& role);
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getRole() const;
    std::string getId() const;
    void saveToFile(const std::string& filename) const;
    void createFolder(const std::filesystem::path& p) const;
    static bool deleteUser(const std::string& filename, const std::string& userId);
    static std::string encrypt(const std::string& input);
    static std::string decrypt(const std::string& input);
    static bool authenticateFromFile(const std::string& filename, const std::string& inputUsername, const std::string& inputPassword);

private:
    std::string generateId(); 
    static const std::string charset;
    std::string id;
    std::string username;
    std::string password;
    std::string role;
    static std::vector<User> loadUsersFromFile(const std::string& filename);
    const int shift = 1;
};