#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <optional>


enum class TokenType{
    CREATE,
    DATABASE,
    IDENTIFIER,
    LBRACE,
    RBRACE,
    COMMA,
    DELETE,
    TABLE,
    USE,
    INSERT,
    INTO,
    SHOW,
    DATABASES,
    TABLES,
    VALUES,
    ALTER,
    ADD,
    DROP,
    COLUMN,
    STRING,
    INTEGER,
    INT_DATA_TYPE,
    STRING_DATA_TYPE,
    SELECT,
    FROM,
    WHERE,
    EQUALS,
    AND,
    NOT_EQUAL,
    USER,
    IDENTIFIED,
    BY,
    ROLE,
    LOGOUT,
    UPDATE,
    SET,
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};


class Tokenizer {
public:
    Tokenizer(std::string str) : m_string(str){}
    ~Tokenizer(){}
    std::vector<Token> tokenize();



private:
    std::string m_string;
    int m_index = 0;

    std::optional<char> peak(int ahead = 0){
        if (m_index + ahead >= m_string.length()){
            return {};
        }else{
            return m_string.at(m_index + ahead);   
        }
    }

    char consume(){
        return m_string.at(m_index++);
    }
};
