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

        // Read the existing JSON object from the database file
        std::ifstream db_file_in("./data/" + m_db_name + ".json");
        nlohmann::json db_json;
        db_file_in >> db_json;
        db_file_in.close();

        // Create a JSON object for the columns
        nlohmann::json columns_json;
        for (auto& col : createTableStmt->columns) {
            std::cout << "Column: " << static_cast<NodeExprIdentifier*>(col.get())->name << std::endl;
            std::string col_name = static_cast<NodeExprIdentifier*>(col.get())->name;
            columns_json[col_name] = std::vector<nlohmann::json>();
        }

        // Add the columns JSON object to the database JSON object
        db_json[table_name] = columns_json;

        // Write the updated database JSON object back to the file
        std::ofstream db_file_out("./data/" + m_db_name + ".json");
        db_file_out << db_json.dump(4); // dump with an indentation of 4 spaces
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