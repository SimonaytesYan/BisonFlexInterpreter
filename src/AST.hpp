#pragma once

#include <algorithm>
#include <cstring>
#include <string>
#include <map>

#include "Node.hpp"

struct Var {
    Var(const std::string& str, int value) :
    name_(str),
    value_(value) {}

    std::string name_;
    int value_;
};

const Var kStartScope = Var("*", 0);

class AST {

public:
    AST(AST&& other);
    AST(Node* root = nullptr);
    AST& operator=(AST&& other);

    void run();

    void graphicDump();

    ~AST();

private:
    int  ExecuteOperator(const Node* node);
    int  ExecuteNode    (const Node* node);
    int  ExecuteKeyword (const Node* node);
    void RecDelete      (Node* node);

    Node* root_;
    std::map<std::string, int> vars_;
};
