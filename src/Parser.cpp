#include "Parser.h"


std::unique_ptr<NodeExpr> Parser::parseExpression() {
    const Token token = consume();
    if (token.type == TokenType::IDENTIFIER) {
        if (token.value.has_value()) {
            return std::make_unique<NodeExprIdentifier>(token.value.value());
        }
    }
    else if (token.type == TokenType::STRING) {
        if (token.value.has_value()) {
            return std::make_unique<NodeExprString>(token.value.value());
        }
    }
    else if (token.type == TokenType::INTEGER) {
        if (token.value.has_value()) {
            return std::make_unique<NodeExprInteger>(std::stoi(token.value.value()));
        }
    }
    else if (token.type == TokenType::EQUALS || token.type == TokenType::NOT_EQUAL) {
        std::unique_ptr<NodeExpr> left = parseExpression();
        std::unique_ptr<NodeExpr> right = parseExpression();
        return std::make_unique<NodeExprBinary>(std::move(left), std::move(right), token);
    }
    else {
        throw std::runtime_error("Invalid token in parseExpression");
    }
}


std::unique_ptr<NodeStmt> Parser::parseStmt(){
    const Token token = peak().value();
    
    switch(token.type){
        case TokenType::CREATE:
            consume();
            if (peak().value().type == TokenType::DATABASE){
                consume();
                std::unique_ptr<NodeStmtCreateDatabase> stmt = std::make_unique<NodeStmtCreateDatabase>();
                stmt->database_name = parseExpression();
                return stmt;
            }
            else if (peak().value().type == TokenType::TABLE){
                consume();
                std::unique_ptr<NodeStmtCreateTable> stmt = std::make_unique<NodeStmtCreateTable>();
                stmt->table_name = parseExpression();
                if (peak().value().type == TokenType::LBRACE){
                    consume();
                    while (peak().value().type != TokenType::RBRACE){
                        stmt->columns.push_back(parseExpression()); // Parse column name
                        // Parse column data type
                        Token dataTypeToken = peak().value();
                        if (dataTypeToken.type == TokenType::INT_DATA_TYPE || dataTypeToken.type == TokenType::STRING_DATA_TYPE) {
                            stmt->data_types.push_back(dataTypeToken); // Store data type
                            consume();
                        } else {
                            throw std::runtime_error("Expected data type (INT or STRING)");
                        }
                        if (peak().value().type == TokenType::COMMA){
                            consume();
                        }
                    }
                    consume();
                    return stmt;
                }
                else{
                    throw std::runtime_error("Expected LBRACE");
                }
            }
            else if (peak().value().type == TokenType::USER){
                consume();
                std::unique_ptr<NodeStmtCreateUser> stmt = std::make_unique<NodeStmtCreateUser>();
                stmt->username = parseExpression();
                if (peak().value().type == TokenType::IDENTIFIED){
                    consume();
                    if (peak().value().type == TokenType::BY){
                        consume();
                        stmt->password = parseExpression();
                    }
                    else{
                        throw std::runtime_error("Expected BY");
                    }
                }
                else{
                    throw std::runtime_error("Expected IDENTIFIED");
                }
                if (peak().value().type == TokenType::ROLE){
                    consume();
                    stmt->role = parseExpression();
                    return stmt;
                }
                else{
                    throw std::runtime_error("Expected ROLE");
                }
            }
            else{
                throw std::runtime_error("Expected DATABASE or TABLE or USER");
            }
            break;
        
        case TokenType::DELETE:
            consume();
            if (peak().value().type == TokenType::DATABASE){
                consume();
                std::unique_ptr<NodeStmtDeleteDatabase> stmt = std::make_unique<NodeStmtDeleteDatabase>();
                stmt->database_name = parseExpression();
                return stmt;
            }
            else if(peak().value().type == TokenType::TABLE){
                consume();
                std::unique_ptr<NodeStmtDeleteTable> stmt = std::make_unique<NodeStmtDeleteTable>();
                stmt->table_name = parseExpression();
                return stmt;
            }
            else if(peak().value().type == TokenType::USER){
                consume();
                std::unique_ptr<NodeStmtDeleteUser> stmt = std::make_unique<NodeStmtDeleteUser>();
                stmt->username = parseExpression();
                return stmt;
            }
            else{
                throw std::runtime_error("Expected DATABASE or TABLE"); //treba malo popravit
            }
            break;

        case TokenType::INSERT:
            consume();
            if (peak().value().type == TokenType::INTO){
                consume();
                std::unique_ptr<NodeStmtInsertIntoTable> stmt = std::make_unique<NodeStmtInsertIntoTable>();
                stmt->table_name = parseExpression();
                if (peak().value().type == TokenType::LBRACE){
                    consume();
                    while (peak().value().type != TokenType::RBRACE){
                        stmt->columns.push_back(parseExpression());
                        if (peak().value().type == TokenType::COMMA){
                            consume();
                        }
                    }
                    consume();
                }
                else{
                    throw std::runtime_error("Expected LBRACE");
                }

                if (peak().value().type == TokenType::VALUES){
                    consume();
                    if (peak().value().type == TokenType::LBRACE){
                        consume();
                        while (peak().value().type != TokenType::RBRACE){
                            stmt->values.push_back(parseExpression());
                            if (peak().value().type == TokenType::COMMA){
                                consume();
                            }
                        }
                        consume();
                        return stmt;
                    }
                    else{
                        throw std::runtime_error("Expected LBRACE");
                    }
                }
                else{
                    throw std::runtime_error("Expected VALUES");
                }
            }
            else{
                throw std::runtime_error("Expected INTO");
            }
            break;

        case TokenType::SHOW:
            consume();
            if (peak().value().type == TokenType::DATABASES){
                consume();
                std::unique_ptr<NodeStmtShowDatabases> stmt = std::make_unique<NodeStmtShowDatabases>();
                return stmt;
            }
            else if(peak().value().type == TokenType::TABLES){
                consume();
                std::unique_ptr<NodeStmtShowTables> stmt = std::make_unique<NodeStmtShowTables>();
                return stmt;
            }
            else{
                throw std::runtime_error("Expected DATABASES or TABLES"); //popravit, treba bit specificno
            }
            break;

        case TokenType::USE:
            consume();
            if (peak().value().type == TokenType::DATABASE){
                consume();
                std::unique_ptr<NodeStmtUseDatabase> stmt = std::make_unique<NodeStmtUseDatabase>();
                stmt->database_name = parseExpression();
                return stmt;
            }
            else{
                throw std::runtime_error("Expected DATABASE");
            }
            break;
            
        case TokenType::ALTER:
            consume();
            if (peak().value().type == TokenType::TABLE){
                consume();
                std::unique_ptr<NodeStmtAlterTable> stmt = std::make_unique<NodeStmtAlterTable>();
                std::unique_ptr<NodeStmtAlterDropColumn> stmt2 = std::make_unique<NodeStmtAlterDropColumn>();
                stmt->table_name = parseExpression();
                stmt2->table_name = stmt->table_name->clone();
                if (peak().value().type == TokenType::ADD){
                    consume();
                    stmt->new_column_name = parseExpression();
                    Token dataTypeToken = peak().value();
                    if (dataTypeToken.type == TokenType::INT_DATA_TYPE || dataTypeToken.type == TokenType::STRING_DATA_TYPE) {
                        stmt->data_type = dataTypeToken; 
                        consume();
                    } else {
                        throw std::runtime_error("Expected data type (INT or STRING) for new column");
                    }
                    return stmt;
                }
                else if (peak().value().type == TokenType::DROP){
                    consume();
                    if (peak().value().type == TokenType::COLUMN){
                        consume();
                        stmt2->column_name = parseExpression();
                        return stmt2;
                    }
                    else{
                        throw std::runtime_error("Expected COLUMN");
                    }
                }
                else{
                    throw std::runtime_error("Expected ADD or DROP");
                }

            }
            else{
                throw std::runtime_error("Expected TABLE");
            }
        case TokenType::SELECT:
        {
            consume(); // Consume SELECT token
            std::unique_ptr<NodeStmtSelect> stmt = std::make_unique<NodeStmtSelect>();
            std::unique_ptr<NodeStmtSelectWhere> stmt2 = std::make_unique<NodeStmtSelectWhere>();

            while (peak().has_value() && peak()->type != TokenType::FROM){
                stmt->columns.push_back(parseExpression());
                stmt2->columns.push_back(stmt->columns.back()->clone());
                if (peak().has_value() && peak()->type == TokenType::COMMA){
                    consume();
                }
            }
            if (!peak().has_value()) {
                throw std::runtime_error("Unexpected end of input: Expected FROM");
            }
            consume(); // Consume FROM token
            stmt->table_name = parseExpression();
            stmt2->table_name = stmt->table_name->clone();

            if (peak().has_value() && peak()->type == TokenType::WHERE){
                consume(); // Consume WHERE token

                stmt2->where_column = parseExpression();

                if (peak().has_value() && (peak()->type == TokenType::EQUALS || peak()->type == TokenType::NOT_EQUAL)){
                    stmt2->where_op = peak().value();
                    consume(); // Consume the operator token

                    stmt2->where_value = parseExpression();
                    return stmt2; // Return SELECT WHERE statement
                }
                else{
                    throw std::runtime_error("Expected EQUALS or NOT_EQUAL after WHERE column");
                }
            }
            else {
                return stmt; // Return regular SELECT statement
            }
        }
        case TokenType::LOGOUT:{
            consume();
            std::unique_ptr<NodeStmtLogout> stmt = std::make_unique<NodeStmtLogout>();
            return stmt;
            break;
        }
        
        default:
            throw std::runtime_error("Invalid token in parseStmt");
    }
}

std::unique_ptr<NodeProgram> Parser::parseProgram(){
    std::unique_ptr<NodeProgram> program = std::make_unique<NodeProgram>();
    while (peak().has_value()){
        program->statements.push_back(parseStmt());
    }
    return program;
}
