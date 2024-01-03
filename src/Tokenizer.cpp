#include "Tokenizer.h"
#include <stdexcept>  

std::vector<Token> Tokenizer::tokenize(){
    std::vector<Token> tokens;
    std::string buffer = "";
    try{
        while (peak().has_value())
        {
            if (isalpha(peak().value())){
                buffer.push_back(consume());
                while (isalnum(peak().value()))
                {
                    buffer.push_back(consume());
                }
                if (buffer == "CREATE"){
                    std::cout << "CREATE" << std::endl;
                    tokens.push_back({TokenType::CREATE});
                    buffer.clear();
                }
                else if (buffer == "DATABASE"){
                    std::cout << "DATABASE" << std::endl;
                    tokens.push_back({TokenType::DATABASE});
                    buffer.clear();
                }
                else{
                    std::cout << "IDENTIFIER: " << buffer << std::endl;
                    tokens.push_back({TokenType::IDENTIFIER, buffer});
                    buffer.clear();
                }
                
            }
            else if (peak().value() == ','){
                consume();
                std::cout << "COMMA" << std::endl;
                tokens.push_back({TokenType::COMMA});
            }
            else if (peak().value() == '{'){
                consume();
                std::cout << "LBRACE" << std::endl;
                tokens.push_back({TokenType::LBRACE});
            }
            else if (peak().value() == '}'){
                consume();
                std::cout << "RBRACE" << std::endl;
                tokens.push_back({TokenType::RBRACE});
            }
            else if (isspace(peak().value())){
                consume();
                continue;
            }
            else{
                throw std::runtime_error("Invalid character: "  + std::string(1, peak().value()));
            }
        }

    }catch (const std::runtime_error& e){
        std::cout << "Error: " << e.what() << std::endl;
        exit(1);
    }
    
    
    m_index = 0;
    return tokens;
}