#pragma once

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

enum class Operator {
    ADD,
    SUB,
    MUL,
    DIV,
    EQUATE,
    IS_EQ,
    IS_NOT_EQ,
    IS_GE,
    IS_LE,
    IS_G,
    IS_L
};

enum class Keyword {
    PROGRAM,
    BEGIN_KW,
    END,
    VAR,
    IF,
    THEN,
    ELSE,
    FOR,
    TO,
    DO,
    WHILE
};

enum class NodeType {
    NUM,
    VAR,
    OPER,
    KEYWORD,
    FICT
};

union NodeValue {
    Operator oper;
    Keyword keyword;
    int num;
    char* var;
};

struct Node {

    Node(const Node& other);
    Node(Node&& other);

    Node& operator=(Node& other);
    Node& operator=(Node&& other);

    void moveVar(Node&& other);

    void copyVar(const char* var);

    Node(Node* left = nullptr, Node* right = nullptr);
    Node(int num, Node* left = nullptr, Node* right = nullptr);

    Node(std::string num, Node* left = nullptr, Node* right = nullptr);

    // Node(const char* var, Node* left, Node* right);
    Node(Operator oper, Node* left = nullptr, Node* right = nullptr);
    Node(Keyword keyword, Node* left = nullptr, Node* right = nullptr);

    ~Node();

    NodeType type_;
    NodeValue val_;

    Node* left_;
    Node* right_;
};

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
    std::vector<Func> funcs_;
};
