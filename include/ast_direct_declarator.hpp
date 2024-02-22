#ifndef AST_DIRECT_DECLARATOR_HPP
#define AST_DIRECT_DECLARATOR_HPP

#include "ast_node.hpp"

class DirectDeclarator : public Node
{
private:
    Node *identifier_;

public:
    DirectDeclarator(Node *identifier) : identifier_(identifier){};
    ~DirectDeclarator()
    {
        delete identifier_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const {
        std::cout<<"Emitting RISC for direct declarator"<<std::endl;
        // identifier_->EmitRISC(stream, context);
    };
    void Print(std::ostream &stream) const override;
};

#endif
