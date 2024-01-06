#include <iostream>
#include "./src/Tokenizer.h"
#include "./src/Parser.h"
#include "./src/Generator.h"
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
            Generator generator(std::move(program));
            generator.Generate();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }


    return 0;
}

