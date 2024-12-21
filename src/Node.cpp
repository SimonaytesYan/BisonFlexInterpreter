#include "Node.hpp"

#include <cstring>

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
