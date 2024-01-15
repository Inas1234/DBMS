#include "Menu.h"
#include "User.h"
#include <iostream>
#include <memory>

void Menu::showMenu() {
    std::cout << "\n\n\n\n";
    std::cout << "Welcome to mlinql!\n";
    std::cout << "------------------\n";
    std::cout << "MAIN MENU:\n";
    std::cout << "1. Login\n";
    std::cout << "2. Help [COMMANDS]\n";
    std::cout << "3. Exit\n";
    std::cout << "------------------\n";
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
        
        if(!currentId.empty()){
            authenticated=true; 
            user.setWorkDir(currentId);
            system("clear");
        } else{
            authenticated=false;
            system("clear");
            std::cout << "Wrong username or password!\n";
            showMenu();
        }
        break;
    }
    case 2:
        showHelp();
        break;
    case 3:
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

    std::cout << "~ CREATE DATABASE [database_name]\n";
    std::cout << "~ CREATE TABLE [table_name] {\n";
    std::cout << "    [column_name column_type],[column_name column_type],\n";
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

    std::cout << "~ ALTER TABLE [table_name] ADD [column_name column_type]\n";
    std::cout << "~ ALTER TABLE [table_name] DROP COLUMN [column_name]\n\n";

    std::cout << "~ UPDATE [table_name] SET [column_name] = [value] WHERE [column_name] = [value]\n\n";

    std::cout << "~ SHOW DATABASES\n";
    std::cout << "~ SHOW TABLES (for the current database)\n\n";
   
    std::cout << "~ LOGOUT\n";
   
    std::cout << "  admin commands:\n";
    std::cout << "~ CREATE USER [username] IDENTIFIED BY [password] ROLE [role]\n";
    std::cout << "~ DELETE USER [username]\n\n";
    
    std::cout << "press any key to close Help\n";
    std::cin.ignore();
    showMenu();

}

