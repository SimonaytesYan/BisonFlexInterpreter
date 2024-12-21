#include "AST.hpp"

#include <iostream>

//================================ NODE ================================

Node::Node(const Node& other):
left_  (other.left_),
right_ (other.right_),
type_  (other.type_) {
    if (type_ == NodeType::VAR)
        copyVar(other.val_.var);
    else
        val_ = other.val_;
}

Node::Node(Node&& other):
left_  (other.left_),
right_ (other.right_),
type_  (other.type_) {
    if (type_ == NodeType::VAR)
        moveVar(std::move(other));
    else
        val_ = other.val_;
}

Node& Node::operator=(Node& other) {

    left_  = other.left_;
    right_ = other.right_;
    type_  = other.type_;

    if (type_ == NodeType::VAR)
        copyVar(other.val_.var);
    else
        val_ = other.val_;

    return *this;
}

Node& Node::operator=(Node&& other) {
    left_  = other.left_;
    right_ = other.right_;
    type_  = other.type_;

    if (type_ == NodeType::VAR)
        moveVar(std::move(other));
    else
        val_ = other.val_;

    return *this;
}

void Node::moveVar(Node&& other) {
    val_.var = other.val_.var;
    other.val_.var;
}

void Node::copyVar(const char* var) {
    val_.var = new char[strlen(var)];
    strcpy(val_.var, var);
}

Node::Node(Node* left, Node* right):
left_  (left),
right_ (right),
type_  (NodeType::FICT) { }

Node::Node(int num, Node* left, Node* right):
left_  (left),
right_ (right),
type_  (NodeType::NUM),
val_   ({.num = num}) { }

// Node::Node(const char* var, Node* left, Node* right):
// left_  (left),
// right_ (right),
// type_  (NodeType::VAR) { 
//     copyVar(var);
// }

Node::Node(std::string num, Node* left, Node* right):
left_  (left),
right_ (right),
type_  (NodeType::NUM) { 
    val_ = {.num = atoi(num.c_str())};
}

Node::Node(Operator oper, Node* left, Node* right):
left_  (left),
right_ (right),
type_  (NodeType::NUM),
val_   ({.oper = oper}) { }

Node::Node(Keyword keyword, Node* left, Node* right):
left_  (left),
right_ (right),
type_  (NodeType::NUM),
val_   ({.keyword = keyword}) { }

Node::~Node() {
    if (type_ == NodeType::VAR)
        delete[] val_.var;
}

//================================ AST ================================


static int ExecuteOperator(const Node* node);
static int ExecuteNode    (const Node* node);

AST::AST(AST&& other) :
root_  (other.root_),
vars_  (other.vars_),
funcs_ (other.funcs_) {
    other.root_ = nullptr;
}

AST::AST(Node* root) :
root_(root) { }
    
AST& AST::operator= (AST&& other) {
    root_ = other.root_;
    vars_ = other.vars_;
    funcs_= other.funcs_;

    other.root_ = nullptr;
    
    return *this;
}

AST::~AST() {
    RecDelete(root_);
}

void AST::RecDelete(Node* node) {
    std::cerr << "Delete node " << node << "\n";
        
    if (node == nullptr)
        return;

    RecDelete(node->left_);
    RecDelete(node->right_);

    delete node;
    std::cerr << "Stop delete node " << node << "\n";
}

void AST::run() {
    std::cerr << "\n\nINTERPRETER: run\n\n";
    ExecuteNode(root_);
    std::cerr << "\n\nINTERPRETER: stop\n\n";
}


#define BINARY_OPER_EXEC(name, oper)                                            \
        case Operator::name: {                                                  \
            std::cerr << "INTERPRETER: " #name "\n";                           \
            int res = ExecuteNode(node->left_) oper ExecuteNode(node->right_);  \
            std::cerr << "INTERPRETER: ADD res = " << res << "\n";              \
                                                                                \
            return res;                                                         \
        }

int ExecuteOperator(const Node* node) {
    switch (node->val_.oper)
    {
        BINARY_OPER_EXEC(ADD, +);
        BINARY_OPER_EXEC(SUB, -);
        BINARY_OPER_EXEC(MUL, *);
        BINARY_OPER_EXEC(DIV, /);
        default: {
            std::cerr << "INTERPRETER: Unknown oper type\n";
            return -1;
        }
    }
}

int ExecuteNode(const Node* node) {
    switch (node->type_)
    {
        case NodeType::NUM: {
            std::cerr << "INTERPRETER: Number " << node->val_.num << "\n"; 
            return node->val_.num;
        }
        case NodeType::OPER: {
            ExecuteOperator(node);
        }
    
        default: {
            std::cerr << "INTERPRETER: Unknown node type\n";
            return -1;
        }
    }
}