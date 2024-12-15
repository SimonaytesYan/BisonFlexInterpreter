#include <cstring>
#include <string>

enum class Operator {

};

enum class Keyword {

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
    // Operator oper;
    // Keyword keyword;
    int num;
    // char sym;
    std::string var;
};

struct Node {


    Node(Node* left = nullptr, Node* right = nullptr):
    left_  (left),
    right_ (right),
    type_  (NodeType::FICT) { }

    Node(int num, Node* left = nullptr, Node* right = nullptr):
    left_  (left),
    right_ (right),
    type_  (NodeType::NUM),
    val_   () { }

    Node(const char* var, Node* left, Node* right):
    left_  (left),
    right_ (right),
    type_  (NodeType::VAR) { 
        copyVar(var);
    }

    Node(char sym, Node* left, Node* right):
    left_  (left),
    right_ (right),
    type_  (NodeType::SYM),
    val_    ({.sym = sym}) { }

    Node(Operator oper, Node* left = nullptr, Node* right = nullptr):
    left_  (left),
    right_ (right),
    type_  (NodeType::NUM),
    val_   ({.oper = oper}) { }

    Node(Keyword keyword, Node* left = nullptr, Node* right = nullptr):
    left_  (left),
    right_ (right),
    type_  (NodeType::NUM),
    val_   ({.keyword = keyword}) { }

    ~Node() {
        if (type_ == NodeType::VAR)
            delete[] val_.var;
    }

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