#include "Generator.h"
#include <string>
#include <filesystem>
std::string m_db_name;




NodeExpr* Generator::genExpr(NodeExpr& expr){
    if (NodeExprIdentifier* id = dynamic_cast<NodeExprIdentifier*>(&expr)){
        return id;
    }
    else if (NodeExprString* str = dynamic_cast<NodeExprString*>(&expr)){
        return str;
    }
    else if (NodeExprInteger* intgr = dynamic_cast<NodeExprInteger*>(&expr)){
        return intgr;
    }
    else if (NodeExprBinary* bin = dynamic_cast<NodeExprBinary*>(&expr)){
        return bin;
    }
    else{
        throw std::runtime_error("Invalid expression");
    }
}

void Generator::genStmt(NodeStmt& stmt){
    if (NodeStmtCreateDatabase* createDbStmt = dynamic_cast<NodeStmtCreateDatabase*>(&stmt)){
        NodeExpr* expr = genExpr(*(createDbStmt->database_name));
        if (NodeExprIdentifier* dbNameExpr = dynamic_cast<NodeExprIdentifier*>(expr)) {
            std::cout << "CREATE DATABASE " << dbNameExpr->name << std::endl;
            std::string db_name = dbNameExpr->name;
            std::ofstream db_file( "./data/"+ db_name + ".json");
            db_file << "{}";
        }
        else {
            throw std::runtime_error("Expected NodeExprIdentifier for database name");
        }
    }
    else if (NodeStmtDeleteDatabase* deleteDbStmt = dynamic_cast<NodeStmtDeleteDatabase*>(&stmt)){
        NodeExpr* expr = genExpr(*(deleteDbStmt->database_name));
        if (NodeExprIdentifier* dbNameExpr = dynamic_cast<NodeExprIdentifier*>(expr)) {
            std::string db_name = dbNameExpr->name;
            std::cout << "DELETE DATABASE " << db_name << std::endl;
            std::remove(("./data/"+ db_name + ".json").c_str());
        }
        else {
            throw std::runtime_error("Expected NodeExprIdentifier for database name");
        }
    }
    else if (NodeStmtCreateTable* createTableStmt = dynamic_cast<NodeStmtCreateTable*>(&stmt)) {
        NodeExpr* tableExpr = genExpr(*(createTableStmt->table_name));
        if (NodeExprIdentifier* tableNameExpr = dynamic_cast<NodeExprIdentifier*>(tableExpr)) {
            std::string table_name = tableNameExpr->name;
            std::cout << "CREATE TABLE " << table_name << std::endl;
            std::ifstream db_file_in("./data/" + m_db_name + ".json");
            nlohmann::json db_json;
            if (db_file_in.peek() != std::ifstream::traits_type::eof()) {
                db_file_in >> db_json;
            }
            db_file_in.close();

            if (db_json.find(table_name) == db_json.end()) {
                nlohmann::json table_schema;
                for (size_t i = 0; i < createTableStmt->columns.size(); ++i) {
                    std::string col_name = static_cast<NodeExprIdentifier*>(createTableStmt->columns[i].get())->name;
                    Token col_type = createTableStmt->data_types[i]; // Assume this is correctly populated
                    std::string type_str = (col_type.type == TokenType::INT_DATA_TYPE) ? "int" : "string"; // Map token types to string
                    table_schema[col_name] = type_str;
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
        else {
            throw std::runtime_error("Expected NodeExprIdentifier for table name");
        }            
        
    }

    else if (NodeStmtDeleteTable* deleteTableStmt = dynamic_cast<NodeStmtDeleteTable*>(&stmt)){
        NodeExpr* tableExpr = genExpr(*(createTableStmt->table_name));
        if (NodeExprIdentifier* tableNameExpr = dynamic_cast<NodeExprIdentifier*>(tableExpr)) {
            std::string table_name = tableNameExpr->name;
            std::cout << "CREATE TABLE " << table_name << std::endl;

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
        else {
            throw std::runtime_error("Expected NodeExprIdentifier for table name");
        }

        
    }
    else if (NodeStmtInsertIntoTable* insertIntoTableStmt = dynamic_cast<NodeStmtInsertIntoTable*>(&stmt)) {
        NodeExpr* tableExpr = genExpr(*(insertIntoTableStmt->table_name));
        if (NodeExprIdentifier* tableNameExpr = dynamic_cast<NodeExprIdentifier*>(tableExpr)) {
            std::string table_name = tableNameExpr->name;
            std::cout << "INSERT INTO TABLE " << table_name << std::endl;

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

                for (size_t i = 0; i < insertIntoTableStmt->columns.size(); ++i) {
                    NodeExpr* colExpr = genExpr(*(insertIntoTableStmt->columns[i]));
                    NodeExpr* valueExpr = genExpr(*(insertIntoTableStmt->values[i]));

                    if (NodeExprIdentifier* colNameExpr = dynamic_cast<NodeExprIdentifier*>(colExpr)) {
                        std::string col_name = colNameExpr->name;

                        if (table_schema.find(col_name) == table_schema.end()) {
                            std::cout << "Column " << col_name << " not found in schema." << std::endl;
                            return;
                        }

                        std::string expected_type = table_schema[col_name];
                        if ((expected_type == "int" && dynamic_cast<NodeExprInteger*>(valueExpr)) ||
                            (expected_type == "string" && dynamic_cast<NodeExprString*>(valueExpr))) {
                            if (NodeExprString* valueStringExpr = dynamic_cast<NodeExprString*>(valueExpr)) {
                                row[col_name] = valueStringExpr->value;
                            } else if (NodeExprInteger* valueIntExpr = dynamic_cast<NodeExprInteger*>(valueExpr)) {
                                row[col_name] = valueIntExpr->value;
                            }
                        } else {
                            std::cout << "Type mismatch for column " << col_name << std::endl;
                            return;
                        }
                    } else {
                        std::cout << "Invalid type for column identifier" << std::endl;
                        return;
                    }
                }

                table_data.push_back(row);
                db_json[table_name]["data"] = table_data;

                std::ofstream db_file_out("./data/" + m_db_name + ".json");
                db_file_out << db_json.dump(4);
                db_file_out.close();

            } else {
                std::cout << "Table " << table_name << " not found." << std::endl;
            }
        } else {
            throw std::runtime_error("Expected NodeExprIdentifier for table name");
        }
    }
    else if (NodeStmtUseDatabase* useDbStmt = dynamic_cast<NodeStmtUseDatabase*>(&stmt)){
        NodeExpr* expr = genExpr(*(useDbStmt->database_name));
        if (NodeExprIdentifier* dbNameExpr = dynamic_cast<NodeExprIdentifier*>(expr)) {
            m_db_name = dbNameExpr->name;
            std::cout << "USE DATABASE " << m_db_name << std::endl;
        }
        else {
            throw std::runtime_error("Expected NodeExprIdentifier for database name");
        }
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
    else if (NodeStmtAlterTable* alterTableStmt = dynamic_cast<NodeStmtAlterTable*>(&stmt)){
        NodeExpr* tableExpr = genExpr(*(alterTableStmt->table_name));
        if (NodeExprIdentifier* tableNameExpr = dynamic_cast<NodeExprIdentifier*>(tableExpr)) {
            std::string table_name = tableNameExpr->name;
            std::cout << "ALTER TABLE " << table_name << std::endl;

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

                std::string col_name = static_cast<NodeExprIdentifier*>(alterTableStmt->new_column_name.get())->name;
                if (table_schema.find(col_name) != table_schema.end()) {
                    std::cout << "Column " << col_name << " already exists." << std::endl;
                    return;
                }
                Token col_type = alterTableStmt->data_type; 
                std::string type_str = (col_type.type == TokenType::INT_DATA_TYPE) ? "int" : "string"; 
                table_schema[col_name] = type_str;

                db_json[table_name]["schema"] = table_schema;

            }
            else {
                std::cout << "Table " << table_name << " not found." << std::endl;
            }

            std::ofstream db_file_out("./data/" + m_db_name + ".json");
            db_file_out << db_json.dump(4); 
            db_file_out.close();

        }
        else {
            throw std::runtime_error("Expected NodeExprIdentifier for table name");
        }

    }
    else if (NodeStmtAlterDropColumn* alterDropColumnStmt = dynamic_cast<NodeStmtAlterDropColumn*>(&stmt)){
        std::cout << "ALTER TABLE " << static_cast<NodeExprIdentifier*>(alterDropColumnStmt->table_name.get())->name << std::endl;
        std::string table_name = static_cast<NodeExprIdentifier*>(alterDropColumnStmt->table_name.get())->name;

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

            std::string col_name = static_cast<NodeExprIdentifier*>(alterDropColumnStmt->column_name.get())->name;
            if (table_schema.find(col_name) == table_schema.end()) {
                std::cout << "Column " << col_name << " not found." << std::endl;
                return;
            }
            table_schema.erase(col_name);
            db_json[table_name]["schema"] = table_schema;

        }
        else {
            std::cout << "Table " << table_name << " not found." << std::endl;
        }

        std::ofstream db_file_out("./data/" + m_db_name + ".json");
        db_file_out << db_json.dump(4); 
        db_file_out.close();
    }
    else if (NodeStmtSelect* selectStmt = dynamic_cast<NodeStmtSelect*>(&stmt)){
        std::cout << "SELECT" << std::endl;

        std::ifstream db_file_in("./data/" + m_db_name + ".json");
        nlohmann::json db_json;
        if (db_file_in.peek() != std::ifstream::traits_type::eof()) {
            db_file_in >> db_json;
        }
        db_file_in.close();

        std::string table_name = static_cast<NodeExprIdentifier*>(selectStmt->table_name.get())->name;
        if (db_json.find(table_name) != db_json.end()) {
            nlohmann::json table_schema = db_json[table_name]["schema"];
            nlohmann::json table_data = db_json[table_name]["data"];

            std::vector<std::string> col_names;
            for (auto& colName : table_schema.items()) {
                col_names.push_back(colName.key());
            }

            std::vector<std::string> select_col_names;

            if (selectStmt->columns.size() == 1 && dynamic_cast<NodeExprIdentifier*>(selectStmt->columns.front().get())->name == "*") {
                select_col_names = col_names;
            } else {
                for (auto& colExpr : selectStmt->columns) {
                    NodeExpr* colExprPtr = genExpr(*colExpr);
                    if (NodeExprIdentifier* colNameExpr = dynamic_cast<NodeExprIdentifier*>(colExprPtr)) {
                        std::string col_name = colNameExpr->name;
                        if (std::find(col_names.begin(), col_names.end(), col_name) != col_names.end()) {
                            select_col_names.push_back(col_name);
                        } else {
                            std::cout << "Column " << col_name << " not found in schema." << std::endl;
                            return;
                        }
                    } else {
                        std::cout << "Invalid type for column identifier" << std::endl;
                        return;
                    }
                }
            }

            const int col_width = 20; 
            std::cout << std::string(col_width * (select_col_names.size() + 1) + select_col_names.size() * 3, '-') << std::endl;
            std::cout << std::setw(col_width) << std::left << "TABLE" << " | ";
            for (auto& col_name : select_col_names) {
                std::cout << std::setw(col_width) << std::left << col_name << " | ";
            }
            std::cout << std::endl;
            std::cout << std::string(col_width * (select_col_names.size() + 1) + select_col_names.size() * 3, '-') << std::endl;

            for (auto& row : table_data) {
                std::cout << std::setw(col_width) << std::left << table_name << " | ";
                for (auto& col_name : select_col_names) {
                    std::cout << std::setw(col_width) << std::left << row[col_name].dump() << " | ";
                }
                std::cout << std::endl;
            }
            std::cout << std::string(col_width * (select_col_names.size() + 1) + select_col_names.size() * 3, '-') << std::endl;

        } else {
            std::cout << "Table " << table_name << " not found." << std::endl;
        }
    }
    else if (NodeStmtSelectWhere * selectWhereStmt = dynamic_cast<NodeStmtSelectWhere*>(&stmt)){
        
        std::cout << "SELECT WHERE" << std::endl;

        std::ifstream db_file_in("./data/" + m_db_name + ".json");
        nlohmann::json db_json;
        if (db_file_in.peek() != std::ifstream::traits_type::eof()) {
            db_file_in >> db_json;
        }
        db_file_in.close();

        std::string table_name = static_cast<NodeExprIdentifier*>(selectWhereStmt->table_name.get())->name;
        if (db_json.find(table_name) != db_json.end()) {
            nlohmann::json table_schema = db_json[table_name]["schema"];
            nlohmann::json table_data = db_json[table_name]["data"];

            // ... [Rest of the code in this block remains unchanged] ...

            // Get column names
            std::vector<std::string> col_names;
            for (auto& colName : table_schema.items()) {
                col_names.push_back(colName.key());
            }



            // Evaluate WHERE condition
            std::string where_col_name = static_cast<NodeExprIdentifier*>(selectWhereStmt->where_column.get())->name;
            NodeExpr* where_value_expr = genExpr(*selectWhereStmt->where_value);
            std::string where_value = where_value_expr->toString(); // Assuming you have a toString method
            TokenType where_op = selectWhereStmt->where_op.type;
            std::cout << "WHERE " << where_col_name << " " << where_value << std::endl;

            std::vector<nlohmann::json> filtered_rows;
            for (auto& row : table_data) {
                std::string row_col_value = row[where_col_name].dump();
                // check if row[where_col_name] has quatation marks
                if (row_col_value[0] == '"') {
                    row_col_value = row[where_col_name].dump().substr(1, row[where_col_name].dump().size() - 2);
                }
                else {
                    row_col_value = row[where_col_name].dump();
                }
                if (where_op == TokenType::EQUALS){
                    if (row_col_value == where_value) {
                        filtered_rows.push_back(row);
                    }
                }
                else if (where_op == TokenType::NOT_EQUAL){
                    if (row_col_value != where_value) {
                        filtered_rows.push_back(row);
                    }
                }
            }

            // Print results
            std::vector<std::string> select_col_names;

            if (selectWhereStmt->columns.size() == 1 && dynamic_cast<NodeExprIdentifier*>(selectWhereStmt->columns.front().get())->name == "*") {
                select_col_names = col_names;
            } else {
                for (auto& colExpr : selectWhereStmt->columns) {
                    NodeExpr* colExprPtr = genExpr(*colExpr);
                    if (NodeExprIdentifier* colNameExpr = dynamic_cast<NodeExprIdentifier*>(colExprPtr)) {
                        std::string col_name = colNameExpr->name;
                        if (std::find(col_names.begin(), col_names.end(), col_name) != col_names.end()) {
                            select_col_names.push_back(col_name);
                        } else {
                            std::cout << "Column " << col_name << " not found in schema." << std::endl;
                            return;
                        }
                    } else {
                        std::cout << "Invalid type for column identifier" << std::endl;
                        return;
                    }
                }
            }

            // Make sure you use the filtered rows here
            const int col_width = 20;
            std::cout << std::string(col_width * (select_col_names.size() + 1) + select_col_names.size() * 3, '-') << std::endl;
            std::cout << std::setw(col_width) << std::left << "TABLE" << " | ";
            for (auto& col_name : select_col_names) {
                std::cout << std::setw(col_width) << std::left << col_name << " | ";
            }
            std::cout << std::endl;
            std::cout << std::string(col_width * (select_col_names.size() + 1) + select_col_names.size() * 3, '-') << std::endl;

            for (auto& row : filtered_rows) {
                std::cout << std::setw(col_width) << std::left << table_name << " | ";
                for (auto& col_name : select_col_names) {
                    std::cout << std::setw(col_width) << std::left << row[col_name].dump() << " | ";
                }
                std::cout << std::endl;
            }
            std::cout << std::string(col_width * (select_col_names.size() + 1) + select_col_names.size() * 3, '-') << std::endl;

        
            // ... [Rest of the code in this block remains unchanged] ...
        } else {
            std::cout << "Table " << table_name << " not found." << std::endl;
        }



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