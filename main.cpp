#include <iostream>
#include "./src/Tokenizer.h"

int main(){
    std::string input = "CREATE DATABASE {test, test2}";
    Tokenizer tokenizer(input);
    std::vector<Token> tokens = tokenizer.tokenize();

    for (const Token& token : tokens) {
        std::cout << "Token: " << (int)token.type;
        if (token.value.has_value()) {
            std::cout << token.value.value();
        }
        std::cout << std::endl;
    }




    return 0;
}