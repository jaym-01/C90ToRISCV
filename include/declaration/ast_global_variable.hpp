#ifndef AST_GLOBAL_VARIABLE_HPP
#define AST_GLOBAL_VARIABLE_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"

// GLOBAL DEFINITION
class GlobalVariable : public Node
{
private:
    Node* declaration_;

    // Init_declarator_list:
    // Direct_declarators (x), declarators (*x) & init_declarators (x = 5)
public:
    GlobalVariable(Node *declaration) : declaration_(declaration){};

    ~GlobalVariable() {
        delete declaration_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {
        std::cout<<"Em RISC for ";
        Print(std::cout);
        stream<<".data"<<std::endl;
        declaration_->GlobalVarEmitRISC(stream, context);
    };

    void Print(std::ostream &stream) const {

        stream << "glo_var: ";
        declaration_->Print(stream);
    };
};

#endif
