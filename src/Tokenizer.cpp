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
                    tokens.push_back({TokenType::CREATE});
                    buffer.clear();
                }
                else if (buffer == "DATABASE"){
                    tokens.push_back({TokenType::DATABASE});
                    buffer.clear();
                }
                else if (buffer == "DELETE"){
                    tokens.push_back({TokenType::DELETE});
                    buffer.clear();
                }
                else if (buffer == "TABLE"){
                    tokens.push_back({TokenType::TABLE});
                    buffer.clear();
                }
                else if (buffer == "USE"){
                    tokens.push_back({TokenType::USE});
                    buffer.clear();
                }
                else if (buffer == "INSERT"){
                    tokens.push_back({TokenType::INSERT});
                    buffer.clear();
                }
                else if (buffer == "INTO"){
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
                else if (buffer == "ALTER"){
                    tokens.push_back({TokenType::ALTER});
                    buffer.clear();
                }
                else if (buffer == "ADD"){
                    tokens.push_back({TokenType::ADD});
                    buffer.clear();
                }
                else if (buffer == "DROP"){
                    tokens.push_back({TokenType::DROP});
                    buffer.clear();
                }
                else if (buffer == "COLUMN"){
                    tokens.push_back({TokenType::COLUMN});
                    buffer.clear();
                }
                else if (buffer == "int"){
                    tokens.push_back({TokenType::INT_DATA_TYPE});
                    buffer.clear();
                }
                else if (buffer == "String"){
                    tokens.push_back({TokenType::STRING_DATA_TYPE});
                    buffer.clear();
                }
                else if (buffer == "SELECT"){
                    tokens.push_back({TokenType::SELECT});
                    buffer.clear();
                }
                else if (buffer == "FROM"){
                    tokens.push_back({TokenType::FROM});
                    buffer.clear();
                }
                else if (buffer == "WHERE"){
                    tokens.push_back({TokenType::WHERE});
                    buffer.clear();
                }
                else if (buffer == "AND"){
                    tokens.push_back({TokenType::AND});
                    buffer.clear();
                }
                else if (buffer == "USER"){
                    tokens.push_back({TokenType::USER});
                    buffer.clear();
                }
                else if (buffer == "IDENTIFIED"){
                    tokens.push_back({TokenType::IDENTIFIED});
                    buffer.clear();
                }
                else if (buffer == "BY"){
                    tokens.push_back({TokenType::BY});
                    buffer.clear();
                }
                else if (buffer == "ROLE"){
                    tokens.push_back({TokenType::ROLE});
                    buffer.clear();
                }
                else if (buffer == "LOGOUT"){
                    tokens.push_back({TokenType::LOGOUT});
                    buffer.clear();
                }
                else if (buffer == "UPDATE"){
                    tokens.push_back({TokenType::UPDATE});
                    buffer.clear();
                }
                else if (buffer == "SET"){
                    tokens.push_back({TokenType::SET});
                    buffer.clear();
                }
                else{
                    tokens.push_back({TokenType::IDENTIFIER, buffer});
                    buffer.clear();
                }

                
            }
            else if (isdigit(peak().value())){
                buffer.push_back(consume());
                while (peak().has_value() && isdigit(peak().value()))
                {
                    buffer.push_back(consume());
                }
                tokens.push_back({TokenType::INTEGER, buffer});
                buffer.clear();
            }
            else if (peak().value() == '\''){
                consume();
                while (peak().has_value() && peak().value() != '\'')
                {
                    buffer.push_back(consume());
                }
                if (peak().has_value()){
                    consume();
                }
                else{
                    throw std::runtime_error("Expected '");
                }
                std::cout << "STRING: " << buffer << std::endl;
                tokens.push_back({TokenType::STRING, buffer});
                buffer.clear();
            }
            else if (peak().value() == ','){
                consume();
                tokens.push_back({TokenType::COMMA});
            }
            else if (peak().value() == '{'){
                consume();
                tokens.push_back({TokenType::LBRACE});
            }
            else if (peak().value() == '}'){
                consume();
                tokens.push_back({TokenType::RBRACE});
            }
            else if (peak().value() == '*'){
                consume();
                tokens.push_back({TokenType::IDENTIFIER, "*"});
            }
            else if (peak().value() == '='){
                consume();
                tokens.push_back({TokenType::EQUALS});
            }
            else if (peak().value() == '!' && peak(1).value() == '='){
                consume();
                consume();
                tokens.push_back({TokenType::NOT_EQUAL});
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