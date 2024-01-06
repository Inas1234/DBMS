#include "Generator.h"
#include <string>

void Generator::genStmt(NodeStmt& stmt){
    if (NodeStmtCreateDatabase* createDbStmt = dynamic_cast<NodeStmtCreateDatabase*>(&stmt)){
        std::cout << "CREATE DATABASE " << static_cast<NodeExprIdentifier*>(createDbStmt->database_name.get())->name << std::endl;
        std::string db_name = static_cast<NodeExprIdentifier*>(createDbStmt->database_name.get())->name;
        std::ofstream db_file( "./data/"+ db_name + ".json");
    }
    else if (NodeStmtDeleteDatabase* deleteDbStmt = dynamic_cast<NodeStmtDeleteDatabase*>(&stmt)){
        std::cout << "DELETE DATABASE " << static_cast<NodeExprIdentifier*>(deleteDbStmt->database_name.get())->name << std::endl;
        std::string db_name = static_cast<NodeExprIdentifier*>(deleteDbStmt->database_name.get())->name;
        std::remove(("./data/"+ db_name + ".json").c_str());
    }
    else{
        throw std::runtime_error("Invalid statement");
    }
}

void Generator::Generate(){
    for (auto& stmt : m_prog->statements){
        genStmt(*stmt);
    }
}