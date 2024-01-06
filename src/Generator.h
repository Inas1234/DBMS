#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "Parser.h"

class Generator{
public:
    Generator(std::unique_ptr<NodeProgram> prog) : m_prog(std::move(prog)) {}
    ~Generator() = default;
    void genStmt(NodeStmt& stmt);
    void Generate();
private:
    std::unique_ptr<NodeProgram> m_prog;

};