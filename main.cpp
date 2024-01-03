#include <iostream>
#include "./src/Tokenizer.h"
#include "./src/Parser.h"

int main() {
    std::string input;


    while (true) {
        std::cout << "mlinql > ";
        if (!std::getline(std::cin, input)) {
            break;
        }
        try {
            Tokenizer tokenizer(input);
            std::vector<Token> tokens = tokenizer.tokenize();
            Parser parser(tokens);
            std::unique_ptr<NodeProgram> program = parser.parseProgram();
            std::cout << "Parsed program" << std::endl;
            for (auto& stmt : program->statements){
                stmt->print();
            }

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }


    return 0;
}

