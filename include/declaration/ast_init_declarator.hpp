
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
        std::cout<<"Emitting RISC for init declarator"<<std::endl;

        // 1. If initializer is present, store value in register

        // 2. Allocate memory and store identifier / function in context

        // 1. Store identifier in context: [identifier: register, type]
        // declarator_->EmitRISC(stream, context);
    };

    void Print(std::ostream &stream) const override {
        declarator_->Print(stream);
        if (initializer_ != nullptr)
        {
            std::cout << " = ";
            initializer_->Print(stream);
            std::cout << ", ";
        }
    };
};

#endif
