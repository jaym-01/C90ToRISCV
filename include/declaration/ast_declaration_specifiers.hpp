#ifndef AST_DECLARATION_SPECIFIERS_HPP
#define AST_DECLARATION_SPECIFIERS_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"

class DeclarationSpecifiers : public NodeList
{

public:
    // DeclarationSpecifiers(){}

    ~DeclarationSpecifiers()
    {
        // delete specifiers_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {
        // Allocate
        std::cout<<"Emitting RISC for declaration specifier"<<std::endl;
    };

    void Print(std::ostream &stream) const
    {
        // specifiers_->Print(stream);
    };
};

#endif
