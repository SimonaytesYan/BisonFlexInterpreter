#pragma once

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

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

    ~AST();

private:
    void RecDelete(Node* node);

    Node* root_;
    std::vector<Var> vars_;
};
