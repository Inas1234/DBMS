#pragma once
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
    virtual std::unique_ptr<NodeExpr> clone() = 0;
};

class NodeExprIdentifier : public NodeExpr {
public:
    std::string name;

    NodeExprIdentifier(const std::string& name) : name(name) {}

    std::unique_ptr<NodeExpr> clone() override{
        return std::make_unique<NodeExprIdentifier>(name);
    }
};

class NodeExprString : public NodeExpr {
public:
    std::string value;

    NodeExprString(const std::string& value) : value(value) {}

    std::unique_ptr<NodeExpr> clone() override{
        return std::make_unique<NodeExprString>(value);
    }
};

class NodeExprInteger : public NodeExpr {
public:
    int value;

    NodeExprInteger(int value) : value(value) {}

    std::unique_ptr<NodeExpr> clone() override{
        return std::make_unique<NodeExprInteger>(value);
    }
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
    
    void print() override{
        std::cout << "NodeStmtCreateDatabase" << std::endl;
        std::cout << "Database name: " << static_cast<NodeExprIdentifier*>(database_name.get())->name << std::endl;
        
    } 
};

class NodeStmtDeleteDatabase : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> database_name;

};

class NodeStmtCreateTable : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
    std::vector<std::unique_ptr<NodeExpr>> columns;
};

class NodeStmtUseDatabase : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> database_name;
};

class NodeStmtDeleteTable : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
};

class NodeStmtInsertIntoTable : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
    std::vector<std::unique_ptr<NodeExpr>> values;
    std::vector<std::unique_ptr<NodeExpr>> columns;
};


class NodeStmtAlterTable : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
    std::unique_ptr<NodeExpr> new_column_name;


};

class NodeStmtAlterDropColumn : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
    std::unique_ptr<NodeExpr> column_name;
};



class NodeStmtShowDatabases : public NodeStmt {
public:
};

class NodeStmtShowTables : public NodeStmt {
public:
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

