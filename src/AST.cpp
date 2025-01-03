#include "AST.hpp"
#include "Logger.hpp"

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
    log_out << "Delete node " << node << "\n";

    if (node == nullptr)
        return;

    RecDelete(node->left_);
    RecDelete(node->right_);

    delete node;
    log_out << "Stop delete node " << node << "\n";
}

void AST::run() {
    log_out << "\n\nINTERPRETER: run\n\n";
    try {
        ExecuteNode(root_);
    }
    catch(...) {
        log_out << "\n\nINTERPRETER: stop with error\n\n";
    }

    log_out << "\n\nINTERPRETER: stop successfully\n\n";
}

#define BINARY_OPER_EXEC(name, oper)                                            \
        case Operator::name: {                                                  \
            log_out << "INTERPRETER: " #name "\n";                            \
            int res = ExecuteNode(node->left_) oper ExecuteNode(node->right_);  \
            log_out << "INTERPRETER: " #name " res = " << res << "\n";        \
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

        BINARY_OPER_EXEC(IS_EQ,     ==);
        BINARY_OPER_EXEC(IS_NOT_EQ, !=);
        BINARY_OPER_EXEC(IS_GE,     >=);
        BINARY_OPER_EXEC(IS_LE,     <=);
        BINARY_OPER_EXEC(IS_G,      >);
        BINARY_OPER_EXEC(IS_L,      <);
        BINARY_OPER_EXEC(AND,       &&);
        BINARY_OPER_EXEC(OR,        ||);

        case Operator::NOT: {
            log_out << "INTERPRETER: NOT \n";
            int res = !ExecuteNode(node->left_);
            log_out << "INTERPRETER: NOT res = " << res << "\n";

            return res;
        }

        case Operator::EQUATE: {
            log_out << "INTERPRETER: =\n";
            int num = ExecuteNode(node->right_);

            const std::string& var_name = node->left_->val_.var;
            if (vars_.count(var_name) != 0)
                vars_[var_name] = num;
            else {
                log_out << "ERROR DURING EXECUTION\n"; 
                log_out << "variable <" + var_name + "> do not exist\n";
 
                throw -1;
            }

            return num;
        }
        default: {
            log_out << "INTERPRETER: Unknown operator type\n";
            throw -1;
        }
    }
}

void AST::ExecuteIf(const Node* node) {
    bool statement = ExecuteNode(node->left_);

    Node* true_branch  = node->right_->left_;
    Node* false_branch = node->right_->right_;

    if (statement) {
        ExecuteNode(true_branch);
    }
    else {
        ExecuteNode(false_branch);
    }
}

void AST::ExecuteWhile(const Node* node) {
    while (ExecuteNode(node->left_)) {
        ExecuteNode(node->right_);
    }    
}

void AST::ExecuteFor(const Node* node) {

    int start_val  = ExecuteNode(node->left_->right_->left_);
    int finish_val = ExecuteNode(node->left_->right_->right_);

    Node* counter = node->left_->left_;
    const std::string& var_name = counter->val_.var;

    log_out << "start_val  = " << start_val << "\n";
    log_out << "finish_val = " << finish_val << "\n";
    log_out << "counter    = " << var_name << "\n";

    if (vars_.count(var_name) == 0) {
        vars_[var_name] = start_val;
    }
    else {
        log_out << "ERROR DURING EXECUTION\n"; 
        log_out << "variable <" + var_name + "> do not exist\n";

        throw -1;
    }

    while(vars_[var_name] < finish_val) {
        ExecuteNode(node->right_);
        vars_[var_name]++;
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
                log_out << "ERROR DURING EXECUTION\n"; 
                log_out << "variable <" + var_name + "> already exist\n";
                throw -1;
            }

            return 0;
        }
        case Keyword::WRITE: {
            if (node->left_->type_ == NodeType::STR)
                std::cout << node->left_->val_.var << std::endl;
            else
                std::cout << ExecuteNode(node->left_) << std::endl;
            return 0;
        }

        case Keyword::READ: {
            const std::string& var_name = node->left_->val_.var;
            if (vars_.count(var_name) != 0)
                std::cin >> vars_[var_name];
            else  {
                log_out << "ERROR DURING EXECUTION\n"; 
                log_out << "variable <" + var_name + "> do not exist\n";
                throw -1;
            }

            return 0;
        }

        case Keyword::IF: {
            ExecuteIf(node);
            return 0;
        }

        case Keyword::WHILE: {
            ExecuteWhile(node);
            return 0;
        }

        case Keyword::FOR: {
            log_out << "INTERPRETER: Execute for\n";
            ExecuteFor(node);
            return 0;
        }

        default: {
            log_out << "INTERPRETER: Unknown keyword type " << (int)node->val_.keyword << "\n";
            throw -1;
        }
    }
}
int AST::GetVarValue(const std::string& var_name) {
    if (vars_.count(var_name) != 0)
        return vars_[var_name];
    else  {
        log_out << "ERROR DURING EXECUTION\n"; 
        log_out << "variable <" + var_name + "> do not exist\n";
        throw -1;
    }
}

int AST::ExecuteNode(const Node* node) {
    switch (node->type_)
    {
        case NodeType::NUM: {
            log_out << "INTERPRETER: Number " << node->val_.num << "\n"; 
            return node->val_.num;
        }
        case NodeType::VAR: {
            log_out << "INTERPRETER: Var " << node->val_.var << "\n"; 

            const std::string& var_name = node->val_.var;
            return GetVarValue(var_name);
        }
        case NodeType::OPER: {
            log_out << "INTERPRETER: Operator " << (int)node->val_.oper << "\n"; 
            return ExecuteOperator(node);
        }
        case NodeType::KEYWORD: {
            log_out << "INTERPRETER: Keyword " << (int)node->val_.keyword << "\n"; 
            return ExecuteKeyword(node);
        }
        case NodeType::FICT: {
            log_out << "INTERPRETER: Fict\n"; 
            ExecuteNode(node->left_);
            ExecuteNode(node->right_);

            return 0;
        }
        default: {
            log_out << "INTERPRETER: Unknown node type " << (int)node->type_ << "\n";
            throw -1;
        }
    }
}

static const char* kKeywords[] = { "VAR",
                                   "READ",
                                   "WRITE",
                                   "PROGRAM",
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
            fprintf(fp, "\\>=");
            break;
        case Operator::IS_LE:
            fprintf(fp, "\\<=");
            break;
        case Operator::IS_L:
            fprintf(fp, "\\<");
            break;
        case Operator::IS_G:
            fprintf(fp, "\\>");
            break;
        case Operator::IS_NOT_EQ:
            fprintf(fp, "\\<\\>");
            break;
        case Operator::AND:
            fprintf(fp, "and");
            break;
        case Operator::OR:
            fprintf(fp, "or");
            break;
        case Operator::NOT:
            fprintf(fp, "not");
            break;
        default:
            fprintf(fp, "#");
            break;
    }
}

void AST::graphicDump()
{
    log_out << "Graphic dump\n";

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
        case NodeType::STR:
        {
            fprintf(fp, node_format, node, light_red);
            fprintf(fp, "STR | %s ", node->val_.var);
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
