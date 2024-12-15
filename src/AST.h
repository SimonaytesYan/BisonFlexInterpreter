#include <cstring>
#include <algorithm>

enum class Operator {
    PLUS,
    MINUS,
    MUL,
    DIV,
    EQUATE,
    IS_EQ,
    IS_NOT_EQ,
    IS_GE,
    IS_LE,
    IS_G,
    IS_L,
};

enum class Keyword {
    PROGRAM,
    BEGIN,
    END,
    VAR,
    IF,
    THEN,
    ELSE,
    FOR,
    TO,
    DO,
    WHILE,
};

enum class NodeType {
    SYM,
    NUM,
    VAR,
    OPER,
    KEYWORD,
    FICT,
};

union NodeValue {
    Operator oper;
    Keyword keyword;
    int num;
    char* var;
    char sym;
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
    Node(char sym, Node* left, Node* right);
    Node(Operator oper, Node* left = nullptr, Node* right = nullptr);
    Node(Keyword keyword, Node* left = nullptr, Node* right = nullptr);

    ~Node();

    NodeType type_;
    NodeValue val_;

    Node* left_;
    Node* right_;
};

class AST {

public:
    AST() :
    root_() {
    }

    void RecDelete(Node* node) {
        if (node == nullptr)
            return;

        RecDelete(node->left_);
        RecDelete(node->right_);

        delete node;
    }

    ~AST() {
        RecDelete(root_.left_);
        RecDelete(root_.right_);
    }
private:
    Node root_;

};