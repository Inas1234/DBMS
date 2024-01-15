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
    virtual std::string toString(){
        return "NodeExpr";
    }  
};

class NodeExprIdentifier : public NodeExpr {
public:
    std::string name;

    NodeExprIdentifier(const std::string& name) : name(name) {}

    std::unique_ptr<NodeExpr> clone() override{
        return std::make_unique<NodeExprIdentifier>(name);
    }

    std::string toString() override{
        return name;
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

    std::string toString() override{
        return std::to_string(value);
    }
};

class NodeExprBinary : public NodeExpr {
public:
    std::unique_ptr<NodeExpr> left;
    std::unique_ptr<NodeExpr> right;
    Token op;

    NodeExprBinary(std::unique_ptr<NodeExpr> left, std::unique_ptr<NodeExpr> right, Token op) : left(std::move(left)), right(std::move(right)), op(op) {}

    std::unique_ptr<NodeExpr> clone() override{
        return std::make_unique<NodeExprBinary>(left->clone(), right->clone(), op);
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
    std::vector<Token> data_types;
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
    void print() override{
        std::cout << "inserted into " << static_cast<NodeExprIdentifier*>(table_name.get())->name<<std::endl;
        std::cout << "columns: " << std::endl;
        for (auto& col : columns){
            std::cout << static_cast<NodeExprIdentifier*>(col.get())->name<<" ";
        }
        std::cout << std::endl;
    }
};


class NodeStmtAlterTable : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
    std::unique_ptr<NodeExpr> new_column_name;
    Token data_type;
    void print() override{
        std::cout << "altered " << static_cast<NodeExprIdentifier*>(table_name.get())->name <<std::endl;
        std::cout << "new column name: " << static_cast<NodeExprIdentifier*>(new_column_name.get())->name << std::endl;
    }
};

class NodeStmtAlterDropColumn : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
    std::unique_ptr<NodeExpr> column_name;

    void print() override{
        std::cout << "dropped column " << static_cast<NodeExprIdentifier*>(column_name.get())->name << " from " <<
        static_cast<NodeExprIdentifier*>(table_name.get())->name  <<std::endl;
    }
};



class NodeStmtShowDatabases : public NodeStmt {
public:
};

class NodeStmtShowTables : public NodeStmt {
public:
};

class NodeStmtSelect : public NodeStmt {
public:
    std::vector<std::unique_ptr<NodeExpr>> columns;
    std::unique_ptr<NodeExpr> table_name;

    void print() override{
        std::cout << "selected ";
        std::cout << "columns: "<<std::endl;
        for (auto& col : columns){
            std::cout << static_cast<NodeExprIdentifier*>(col.get())->name<<" ";
        }
        std::cout << "from table: " << static_cast<NodeExprIdentifier*>(table_name.get())->name << std::endl;
    }
};

class NodeStmtSelectWhere : public NodeStmt {
public:
    std::vector<std::unique_ptr<NodeExpr>> columns;
    std::unique_ptr<NodeExpr> table_name;
    std::unique_ptr<NodeExpr> where_column;
    std::unique_ptr<NodeExpr> where_value;
    Token where_op;

    void print() override{
        std::cout << "selected columns: " << std::endl;
        for (auto& col : columns){
            std::cout << static_cast<NodeExprIdentifier*>(col.get())->name<<" ";
        }
        std::cout << "\nfrom table: " << static_cast<NodeExprIdentifier*>(table_name.get())->name << std::endl;
        std::cout << "where column: " << static_cast<NodeExprIdentifier*>(where_column.get())->name << std::endl;
        std::cout << "where value: " << static_cast<NodeExprString*>(where_value.get())->value << std::endl;
    }

};

class NodeStmtCreateUser : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> username;
    std::unique_ptr<NodeExpr> password;
    std::unique_ptr<NodeExpr> role;
};

class NodeStmtDeleteUser : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> username;
};

class NodeStmtLogout : public NodeStmt {
public:
};

class NodeStmtUpdate : public NodeStmt {
public:
    std::unique_ptr<NodeExpr> table_name;
    std::vector<std::unique_ptr<NodeExpr>> columns;
    std::vector<std::unique_ptr<NodeExpr>> values;
    std::unique_ptr<NodeExpr> where_column;
    std::unique_ptr<NodeExpr> where_value;
    Token where_op;


    void print() override{
        std::cout << "updated table " << static_cast<NodeExprIdentifier*>(table_name.get())->name <<std::endl;
        std::cout << "where column: " << static_cast<NodeExprIdentifier*>(where_column.get())->name << std::endl;
        std::cout << "where value: " << static_cast<NodeExprString*>(where_value.get())->value << std::endl;
    }
    std::ostream& operator<<(std::ostream& os) {
        print();
        return os;
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

