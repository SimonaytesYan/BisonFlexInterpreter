#include "AST.hpp"

#include <iostream>

//================================ AST ================================


static int ExecuteOperator(const Node* node);
static int ExecuteNode    (const Node* node);

AST::AST(AST&& other) :
root_  (other.root_),
vars_  (other.vars_) {
    other.root_ = nullptr;
}

AST::AST(Node* root) :
root_(root) { }
    
AST& AST::operator= (AST&& other) {
    root_ = other.root_;
    vars_ = other.vars_;

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