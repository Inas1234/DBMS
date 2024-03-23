#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "./src/Tokenizer.h"
#include "./src/Parser.h"
#include "./src/Generator.h"
#include "./src/Menu.h"
#include "./src/User.h"
#include <unistd.h> 

int main(int argc, char* argv[]) {

    bool usingFile = false;
    std::string input;

    Menu menu;
    menu.showMenu();

    while (menu.authenticated) {  
        if (argv[1] != NULL) {
            std::string buffer;
            {
                std::ifstream file("../../" + std::string(argv[1]));
                if (file.is_open()) {
                    std::stringstream ss;
                    ss << file.rdbuf();
                    buffer = ss.str();
                } else {
                    std::cerr << "Error: Could not open file" << std::endl;
                    return 1;
                }
            }            

            usingFile = true;
            try {
                Tokenizer tokenizer(buffer);
                std::vector<Token> tokens = tokenizer.tokenize();
                Parser parser(tokens);
                std::unique_ptr<NodeProgram> program = parser.parseProgram();
                Generator generator(std::move(program));
                generator.Generate();
                if (generator.isLoggedOut()) {
                    menu.authenticated = false;
                    menu.showMenu();
                }
                break;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        else{

            std::cout << "mlinql > ";
            if (!std::getline(std::cin, input)) {
                break;
            }
            try {
                Tokenizer tokenizer(input);
                std::vector<Token> tokens = tokenizer.tokenize();
                Parser parser(tokens);
                std::unique_ptr<NodeProgram> program = parser.parseProgram();
                Generator generator(std::move(program));
                generator.Generate();
                if (generator.isLoggedOut()) {
                    menu.authenticated = false;
                    menu.showMenu();
                }

            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

    }


    return 0;
}

