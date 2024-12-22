#pragma once

#include <string>

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
    IS_L,
    AND,
    OR,
    NOT
};

enum class Keyword {
    VAR,
    READ,
    WRITE,
    PROGRAM,
    IF,
    FOR,
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

    Node(const char* var, Node* left, Node* right);
    Node(Operator oper, Node* left = nullptr, Node* right = nullptr);
    Node(Keyword keyword, Node* left = nullptr, Node* right = nullptr);

    ~Node();

    NodeType type_;
    NodeValue val_;

    Node* left_;
    Node* right_;
};
