#include "Parser.h"


std::unique_ptr<NodeExpr> Parser::parseExpression() {
    const Token token = consume();
    if (token.type == TokenType::IDENTIFIER) {
        if (token.value.has_value()) {
            return std::make_unique<NodeExprIdentifier>(token.value.value());
        }
    }
    throw std::runtime_error("Invalid token in parseExpression");  
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
                        stmt->columns.push_back(parseExpression());
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
                throw std::runtime_error("Expected DATABASE");
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
                    std::cout<<"dobar if, -> consume\n";
                    consume();
                    while (peak().value().type != TokenType::RBRACE){
                        std::cout<<"dobro\n";
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
        //case TokenType::COMMA

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
