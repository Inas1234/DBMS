#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "Parser.h"
#include <nlohmann/json.hpp>

class Generator{
public:
    Generator(std::unique_ptr<NodeProgram> prog) : m_prog(std::move(prog)) {}
    ~Generator() = default;
    void genStmt(NodeStmt& stmt);
    NodeExpr* genExpr(NodeExpr& expr);
    void Generate();
private:
    std::unique_ptr<NodeProgram> m_prog;
};