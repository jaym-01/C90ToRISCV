#ifndef AST_DIRECT_DECLARATOR_HPP
#define AST_DIRECT_DECLARATOR_HPP

#include "ast_node.hpp"

// NOTE: Create a class for each type of direct declarator?
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
    void EmitRISC(std::ostream &stream, Context &context) const override{
        identifier_->EmitRISC(stream, context);
        stream << ":" << std::endl;
    };

    std::string GetIdentifier() const override {
        return identifier_->GetIdentifier();
    };

    void Print(std::ostream &stream) const {
        std::cout<<"direct_decl{ ";
        identifier_->Print(stream);
        std::cout<<" }";
    };
};

#endif


