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
            else{
                throw std::runtime_error("Expected DATABASE");
            }
            break;
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
