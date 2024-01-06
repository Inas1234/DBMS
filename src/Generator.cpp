#include "Generator.h"
#include <string>

std::string m_db_name;


void Generator::genStmt(NodeStmt& stmt){
    if (NodeStmtCreateDatabase* createDbStmt = dynamic_cast<NodeStmtCreateDatabase*>(&stmt)){
        std::cout << "CREATE DATABASE " << static_cast<NodeExprIdentifier*>(createDbStmt->database_name.get())->name << std::endl;
        std::string db_name = static_cast<NodeExprIdentifier*>(createDbStmt->database_name.get())->name;
        std::ofstream db_file( "./data/"+ db_name + ".json");
        db_file << "{}";
    }
    else if (NodeStmtDeleteDatabase* deleteDbStmt = dynamic_cast<NodeStmtDeleteDatabase*>(&stmt)){
        std::cout << "DELETE DATABASE " << static_cast<NodeExprIdentifier*>(deleteDbStmt->database_name.get())->name << std::endl;
        std::string db_name = static_cast<NodeExprIdentifier*>(deleteDbStmt->database_name.get())->name;
        std::remove(("./data/"+ db_name + ".json").c_str());
    }
    if (NodeStmtCreateTable* createTableStmt = dynamic_cast<NodeStmtCreateTable*>(&stmt)) {
        std::cout << "CREATE TABLE " << static_cast<NodeExprIdentifier*>(createTableStmt->table_name.get())->name << std::endl;
        std::string table_name = static_cast<NodeExprIdentifier*>(createTableStmt->table_name.get())->name;

        std::ifstream db_file_in("./data/" + m_db_name + ".json");
        nlohmann::json db_json;
        if (db_file_in.peek() != std::ifstream::traits_type::eof()) {
            db_file_in >> db_json;
        }
        db_file_in.close();

        if (db_json.find(table_name) == db_json.end()) {
            nlohmann::json table_schema;
            for (auto& col : createTableStmt->columns) {
                std::string col_name = static_cast<NodeExprIdentifier*>(col.get())->name;
                table_schema[col_name] = ""; 
            }

            db_json[table_name] = {
                {"schema", table_schema},
                {"data", nlohmann::json::array()}
            };
        }
        else {
            std::cout << "Table " << table_name << " already exists." << std::endl;
        }

        std::ofstream db_file_out("./data/" + m_db_name + ".json");
        db_file_out << db_json.dump(4); 
        db_file_out.close();
    }
    else if (NodeStmtUseDatabase* useDbStmt = dynamic_cast<NodeStmtUseDatabase*>(&stmt)){
        std::cout << "USE DATABASE " << static_cast<NodeExprIdentifier*>(useDbStmt->database_name.get())->name << std::endl;
        std::string db_name = static_cast<NodeExprIdentifier*>(useDbStmt->database_name.get())->name;
        m_db_name = db_name;
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