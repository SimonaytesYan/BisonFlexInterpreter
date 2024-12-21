#include "AST.hpp"

#include <iostream>

static void writeNodeAndEdge(Node* node, FILE* fp);

//================================ AST ================================


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
    try {
        ExecuteNode(root_);
    }
    catch(...) {
        std::cerr << "\n\nINTERPRETER: stop with error\n\n";
    }

    std::cerr << "\n\nINTERPRETER: stop successfully\n\n";
}

#define BINARY_OPER_EXEC(name, oper)                                            \
        case Operator::name: {                                                  \
            std::cerr << "INTERPRETER: " #name "\n";                            \
            int res = ExecuteNode(node->left_) oper ExecuteNode(node->right_);  \
            std::cerr << "INTERPRETER: " #name " res = " << res << "\n";        \
                                                                                \
            return res;                                                         \
        }

int AST::ExecuteOperator(const Node* node) {
    switch (node->val_.oper)
    {
        BINARY_OPER_EXEC(ADD, +);
        BINARY_OPER_EXEC(SUB, -);
        BINARY_OPER_EXEC(MUL, *);
        BINARY_OPER_EXEC(DIV, /);
        case Operator::EQUATE: {
            std::cerr << "INTERPRETER: =\n";
            int num = ExecuteNode(node->right_);

            const std::string& var_name = node->left_->val_.var;
            if (vars_.count(var_name) != 0)
                vars_[var_name] = num;
            else {
                std::cerr << "ERROR DURING EXECUTION\n"; 
                std::cerr << "variable <" + var_name + "> do not exist\n";
 
                throw -1;
            }

            return num;
        }
        default: {
            std::cerr << "INTERPRETER: Unknown operator type\n";
            throw -1;
        }
    }
}

int AST::ExecuteKeyword(const Node* node) {
    switch (node->val_.keyword)
    {
        case Keyword::VAR: {
            const std::string& var_name = node->left_->val_.var;
            if (vars_.count(var_name) == 0)
                vars_[var_name] = 0;
            else  {
                std::cerr << "ERROR DURING EXECUTION\n"; 
                std::cerr << "variable <" + var_name + "> already exist\n";
                throw -1;
            }

            return 0;
        }
        case Keyword::WRITE: {
            std::cout << ExecuteNode(node->left_);
            return 0;
        }

        case Keyword::READ: {
            const std::string& var_name = node->left_->val_.var;
            if (vars_.count(var_name) != 0)
                std::cin >> vars_[var_name];
            else  {
                std::cerr << "ERROR DURING EXECUTION\n"; 
                std::cerr << "variable <" + var_name + "> do not exist\n";
                throw -1;
            }

            return 0;
        }

        default: {
            std::cerr << "INTERPRETER: Unknown keyword type " << (int)node->val_.keyword << "\n";
            throw -1;
        }
    }
}
int AST::GetVarValue(const std::string& var_name) {
    if (vars_.count(var_name) != 0)
        return vars_[var_name];
    else  {
        std::cerr << "ERROR DURING EXECUTION\n"; 
        std::cerr << "variable <" + var_name + "> do not exist\n";
        throw -1;
    }
}

int AST::ExecuteNode(const Node* node) {
    switch (node->type_)
    {
        case NodeType::NUM: {
            std::cerr << "INTERPRETER: Number " << node->val_.num << "\n"; 
            return node->val_.num;
        }
        case NodeType::VAR: {
            std::cerr << "INTERPRETER: Var " << node->val_.var << "\n"; 

            const std::string& var_name = node->val_.var;
            return GetVarValue(var_name);
        }
        case NodeType::OPER: {
            std::cerr << "INTERPRETER: Operator " << (int)node->val_.oper << "\n"; 
            return ExecuteOperator(node);
        }
        case NodeType::KEYWORD: {
            std::cerr << "INTERPRETER: Keyword " << (int)node->val_.keyword << "\n"; 
            return ExecuteKeyword(node);
        }
        case NodeType::FICT: {
            std::cerr << "INTERPRETER: Fict\n"; 
            ExecuteNode(node->left_);
            ExecuteNode(node->right_);

            return 0;
        }
        default: {
            std::cerr << "INTERPRETER: Unknown node type " << (int)node->type_ << "\n";
            throw -1;
        }
    }
}

static const char* kKeywords[] = { "VAR",
                                   "READ",
                                   "WRITE",
                                   "PROGRAM",
                                   "BEGIN_KW",
                                   "END",
                                   "IF",
                                   "THEN",
                                   "ELSE",
                                   "FOR",
                                   "TO",
                                   "DO",
                                   "WHILE"};  


void fprintOper(const Node* node, FILE* fp) {
    switch(node->val_.oper)
    {
        case Operator::ADD:
            fprintf(fp, "+");
            break;
        case Operator::SUB:
            fprintf(fp, "-");
            break;
        case Operator::MUL:
            fprintf(fp, "*");
            break;
        case Operator::DIV:
            fprintf(fp, "/");
            break;
        case Operator::EQUATE:
            fprintf(fp, ":=");
            break;
        case Operator::IS_EQ:
            fprintf(fp, "=");
            break;
        case Operator::IS_GE:
            fprintf(fp, ">=");
            break;
        case Operator::IS_NOT_EQ:
            fprintf(fp, "!=");
            break;
        case Operator::IS_L:
            fprintf(fp, "&#60;");
            break;
        case Operator::IS_LE:
            fprintf(fp, "<=");
            break;
        default:
            fprintf(fp, "#");
            break;
    }
}

void AST::graphicDump()
{
    printf("Graphic dump\n");

    FILE* fp = fopen("GraphicDumps/dump", "w");

    fprintf(fp, "digraph G{\n");
    fprintf(fp, "node[shape = record, fontsize = 14];\n");
    fprintf(fp, "splines = ortho\n");

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    fprintf(fp, "info[label = \"root = 0x%06X\"]\n", root_);
    #pragma GCC diagnostic pop

    writeNodeAndEdge(root_, fp);

    fprintf(fp, "}");

    fclose(fp);

    system("dot GraphicDumps/dump -o GraphicDumps/Dump.png -T png");
}

static void writeNodeAndEdge(Node* node, FILE* fp)
{ 
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    const char light_green[]  = "#B1FF9F";
    const char light_red[]    = "#FF4646";
    const char light_blue[]   = "#87A7FF";
    const char light_grey[]   = "#C8C8C8";
    const char light_yellow[] = "#FFDC4B";

    const char node_format[] = "Node%06X[style = \"filled,rounded\", fillcolor = \"%s\", label = \"{{ <v>";
    
    switch (node->type_)
    {
        case NodeType::NUM:
        {
            fprintf(fp, node_format, node, light_blue);
            fprintf(fp, "NUM | %d", node->val_.num);
            break;
        }
        case NodeType::OPER:
        {
            fprintf(fp, node_format, node, light_yellow);
            fprintf(fp, "OPER | ");
            fprintOper(node, fp);
            break;
        }
        case NodeType::VAR:
        {
            fprintf(fp, node_format, node, light_blue);
            fprintf(fp, "VAR | %s ", node->val_.var);
            break;
        }
        case NodeType::KEYWORD:
        {
            fprintf(fp, node_format, node, light_green);
            fprintf(fp, "KEYWORD | %s ", kKeywords[(int)node->val_.keyword]);
            break;
        }
        case NodeType::FICT:
        {
            fprintf(fp, node_format, node, light_grey);
            fprintf(fp, "FICT ");
            break;
        }
        default:
            fprintf(fp, "unknown");
            break;
    }

    fprintf(fp, "} }\"]\n");
                
    if (node->left_ != nullptr) {
        fprintf(fp, "Node%06X -> Node%06X[xlabel = \"L\"]\n", node, node->left_);
        writeNodeAndEdge(node->left_, fp);
    }
    if (node->right_ != nullptr) {
        fprintf(fp, "Node%06X -> Node%06X[xlabel = \"R\"]\n", node, node->right_);
        writeNodeAndEdge(node->right_, fp);
    }

    #pragma GCC diagnostic pop
}
