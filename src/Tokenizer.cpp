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
                while (peak().has_value() && isalnum(peak().value()))
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
                else if (buffer == "DELETE"){
                    std::cout << "DELETE" << std::endl;
                    tokens.push_back({TokenType::DELETE});
                    buffer.clear();
                }
                else if (buffer == "TABLE"){
                    std::cout << "TABLE" << std::endl;
                    tokens.push_back({TokenType::TABLE});
                    buffer.clear();
                }
                else if (buffer == "USE"){
                    std::cout << "USE" << std::endl;
                    tokens.push_back({TokenType::USE});
                    buffer.clear();
                }
                else if (buffer == "INSERT"){
                    std::cout <<"INSERT" << std::endl;
                    tokens.push_back({TokenType::INSERT});
                    buffer.clear();
                }
                else if (buffer == "INTO"){
                    std::cout << "INTO" << std::endl;
                    tokens.push_back({TokenType::INTO});
                    buffer.clear();
                }
                else if (buffer == "SHOW"){
                    tokens.push_back({TokenType::SHOW});
                    buffer.clear();
                }
                else if (buffer == "DATABASES"){
                    tokens.push_back({TokenType::DATABASES});
                    buffer.clear();
                }
                else if (buffer == "TABLES"){
                    tokens.push_back({TokenType::TABLES});
                    buffer.clear();
                }
                else if (buffer == "VALUES"){
                    tokens.push_back({TokenType::VALUES});
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
    }
    
    
    m_index = 0;
    return tokens;
}