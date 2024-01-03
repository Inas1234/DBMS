#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include "Tokenizer.h"


class NodeExpr {
public:
    virtual ~NodeExpr() {}
    // Additional virtual functions can be added here if needed
};

class NodeExprIdentifier : public NodeExpr {
public:
    std::string name;

    NodeExprIdentifier(const std::string& name) : name(name) {}
};



class NodeStmt {
public:
    virtual ~NodeStmt() {}
    virtual void print(){
        std::cout << "NodeStmt" << std::endl;
    }
};

class NodeStmtCreateDatabase : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> database_name;
    std::vector<std::unique_ptr<NodeExpr>> columns;
    // Use the print function to print the contents of the class
    void print() override{
        std::cout << "NodeStmtCreateDatabase" << std::endl;
        std::cout << "Database name: " << static_cast<NodeExprIdentifier*>(database_name.get())->name << std::endl;
        std::cout << "Columns: " << std::endl;
        for (auto& column : columns){
            std::cout << static_cast<NodeExprIdentifier*>(column.get())->name << std::endl;
        }
    } 
};

struct NodeProgram {
    std::vector<std::unique_ptr<NodeStmt>> statements;
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : m_tokens(tokens){}

    std::unique_ptr<NodeStmt> parseStmt();
    std::unique_ptr<NodeExpr> parseExpression();
    std::unique_ptr<NodeProgram> parseProgram();

    
private:
    const std::vector<Token>& m_tokens;
    size_t m_index = 0;

    std::optional<Token> peak(int ahead = 0){
        if (m_index + ahead >= m_tokens.size()){
            return {};
        }
        return m_tokens[m_index + ahead];
    }
        
    Token consume(){
        return m_tokens[m_index++];
    }

};

// Define other NodeStmt derived classes...

