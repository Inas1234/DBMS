#include "Menu.h"
#include "User.h"
#include <iostream>
#include <memory>

void Menu::showMenu() {
    std::cout << "1. Login\n";
    std::cout << "2. Add user\n";       
    std::cout << "3. Help [COMMANDS]\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter your option: ";

    int opt;
    std::cin >> opt;
    std::cin.ignore();
    std::string username;
    std::string password;
    User user = User(username, password, "user");
    std::string currentId;
    switch (opt)
    {
    case 1:
    {
        std::cout << "username: ";
        std::getline(std::cin, username);
        std::cout << "password: ";
        std::getline(std::cin, password);

        currentId = user.authenticateFromFile("users.json", username, password);
        authenticated=true; 
        user.setWorkDir(currentId);

        break;
    }
    case 2:
    {
        std::cout << "username: ";
        std::getline(std::cin, username);
        std::cout << "password: ";
        std::getline(std::cin, password);
        std::unique_ptr<User> user = std::make_unique<User>(username, password, "user");
        user->saveToFile("users.json");
        std::cout << "User added!\n";
        showMenu();
        break;
    }
    case 3:
        showHelp();
        showMenu();
        break;
    case 4:
        std::cout << "Exiting...\n";
        exit(0);
        break;

    default:
        break;
    }
}

void Menu::showHelp() {
    system("clear");
    std::cout << "Available commands:\n";

    std::cout << "* CREATE DATABASE [database_name]\n";
    std::cout << "* CREATE TABLE [table_name] {\n";
    std::cout << "    [column_name],[column_name],\n";
    std::cout << "    ...\n";
    std::cout << "    }\n\n";

    std::cout << "~ DELETE DATABASE [database_name]\n";
    std::cout << "~ DELETE TABLE [table_name]\n\n";

    std::cout << "~ INSERT INTO [table_name] {\n";
    std::cout << "    [column_name], [column_name], ...\n";
    std::cout << "   } VALUES {\n";
    std::cout << "    [value1], [value2], ...\n";
    std::cout << "   }\n\n";

    std::cout << "~ SELECT [* or column_name(s)] FROM [table_name]\n";
    std::cout << "~ SELECT * FROM [table_name] WHERE [column_name] = [value]\n\n"; //provjerit

    std::cout << "~ ALTER TABLE [table_name] ADD [column_name]\n";
    std::cout << "~ ALTER TABLE [table_name] DROP [column_name]\n\n";

    std::cout << "~ UPDATE [table_name] SET [column_name] = [value] WHERE [column_name] = [value]\n\n";

    std::cout << "~ SHOW DATABASES\n";
    std::cout << "~ SHOW TABLES (for the current database)\n\n";
}

