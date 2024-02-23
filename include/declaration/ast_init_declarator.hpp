
#ifndef AST_INIT_DECLARATOR_HPP
#define AST_INIT_DECLARATOR_HPP

#include "../ast_node.hpp"

// Declarator can either be a direct declarator or a pointer declarator
// Direct declarator can either be an identifier or a function declarator

class InitDeclarator : public Node
{
private:
    Node *declarator_;
    Node *initializer_;

public:
    InitDeclarator(Node *declarator, Node *initializer) :
        declarator_(declarator), initializer_(initializer) {};

    ~InitDeclarator()
    {
        delete declarator_;
        delete initializer_;
    };



    void EmitRISC(std::ostream &stream, Context &context) const {
        // Declarator, initializer
    };

    void Print(std::ostream &stream) const override {
        std::cout<<"init_decl{ ";
        declarator_->Print(stream);
        if (initializer_ != nullptr)
        {
            std::cout << " = ";
            initializer_->Print(stream);
            // std::cout << ", ";
        }
        std::cout<<" }";
    };

    std::string GetIdentifier() const override {
        return declarator_->GetIdentifier();
    }
};

#endif
