#include "Generator.h"
#include <string>
#include <filesystem>
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
    else if (NodeStmtCreateTable* createTableStmt = dynamic_cast<NodeStmtCreateTable*>(&stmt)) {
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

    else if (NodeStmtDeleteTable* deleteTableStmt = dynamic_cast<NodeStmtDeleteTable*>(&stmt)){
        std::cout << "DELETE TABLE " << static_cast<NodeExprIdentifier*>(deleteTableStmt->table_name.get())->name << std::endl;
        std::string table_name = static_cast<NodeExprIdentifier*>(deleteTableStmt->table_name.get())->name;

        std::ifstream db_file_in("./data/" + m_db_name + ".json");
        nlohmann::json db_json;
        if (db_file_in.peek() != std::ifstream::traits_type::eof()) {
            db_file_in >> db_json;
        }
        db_file_in.close();

        if (db_json.find(table_name) != db_json.end()) {
            db_json.erase(table_name);
        }
        else {
            std::cout << "Table " << table_name << " not found." << std::endl;
        }

        std::ofstream db_file_out("./data/" + m_db_name + ".json");
        db_file_out << db_json.dump(4); 
        db_file_out.close();
    }
    
    //err: INVALID TOKEN IN PARSEEXPRESSION
    // potrebno popraviti funkciju, tako da dopisuje ispravno dopisuje u json, i da se ispravno tokenizira
    /*
    else if (NodeStmtInsertIntoTable* insertIntoTableStmt = dynamic_cast<NodeStmtInsertIntoTable*>(&stmt)){
        std::cout << "INSERT INTO TABLE " << static_cast<NodeExprIdentifier*>(insertIntoTableStmt->table_name.get())->name << std::endl;
        std::string table_name = static_cast<NodeExprIdentifier*>(insertIntoTableStmt->table_name.get())->name;

        std::ifstream db_file_in("./data/" + m_db_name + ".json");
        nlohmann::json db_json;
        if (db_file_in.peek() != std::ifstream::traits_type::eof()) {
            db_file_in >> db_json;
        }
        db_file_in.close();

        if (db_json.find(table_name) != db_json.end()) {
            nlohmann::json table_schema = db_json[table_name]["schema"];
            nlohmann::json table_data = db_json[table_name]["data"];
            nlohmann::json row;
        // iterate through the table structure and add the values to the row

            for (int i = 0; i < insertIntoTableStmt->values.size(); i++) {
                std::string col_name = static_cast<NodeExprIdentifier*>(insertIntoTableStmt->values[i].get())->name;
                row[col_name] = "";
            }
        

        }
        else {
            std::cout << "Table " << table_name << " not found." << std::endl;
        }

        std::ofstream db_file_out("./data/" + m_db_name + ".json");
        db_file_out << db_json.dump(4); 
        db_file_out.close();
    }
    */

    else if (NodeStmtUseDatabase* useDbStmt = dynamic_cast<NodeStmtUseDatabase*>(&stmt)){
        std::cout << "USE DATABASE " << static_cast<NodeExprIdentifier*>(useDbStmt->database_name.get())->name << std::endl;
        std::string db_name = static_cast<NodeExprIdentifier*>(useDbStmt->database_name.get())->name;
        m_db_name = db_name;
    }

    else if(NodeStmtShowDatabases* showDbStmt = dynamic_cast<NodeStmtShowDatabases*>(&stmt)){
        //std::cout << std::setw(20) << std::left << "List of Databases" << std::endl;
        std::cout << "-------------------------------------------" << std::endl;

        const std::string dataFolderPath = "./data/";
        std::cout << std::setw(20) << std::left << "Name" << " | " << std::setw(10) << "Owner" << std::endl;    // DODATNO NAPRAVITI USER SISTEM I AUTENTIFIKACIJU KORISNIKA
        std::cout << "-------------------------------------------" << std::endl;

        for (const auto& entry : std::filesystem::directory_iterator(dataFolderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                std::cout << std::setw(20) << std::left << entry.path().stem().string() << " | " << std::setw(10) << "current" << std::endl;
            }
        }
        std::cout << "-------------------------------------------" << std::endl;
    }

    else if(NodeStmtShowTables* showTableStmt = dynamic_cast<NodeStmtShowTables*>(&stmt)){
        std::cout << std::setw(15) << std::left << "TABLE" << " | " <<"COLUMNS" << std::endl;
        std::cout << "-------------------------------------------" << std::endl;

        std::ifstream db_file_in("./data/" + m_db_name + ".json");
        nlohmann::json db_json;
        if (db_file_in.peek() != std::ifstream::traits_type::eof()) {
            db_file_in >> db_json;
        }
        db_file_in.close();

        for (auto& [tableName, tableDetails] : db_json.items()) {
            std::cout << std::setw(15) << std::left << tableName << " | " ;
            for (auto& colName : tableDetails["schema"].items()) {
                std::cout << colName.key() << " ";
            }
            std::cout<<std::endl;
        }
        std::cout << "-------------------------------------------" << std::endl;
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