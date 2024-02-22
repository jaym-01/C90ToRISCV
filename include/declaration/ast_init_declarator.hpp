
#ifndef AST_INIT_DECLARATOR_HPP
#define AST_INIT_DECLARATOR_HPP

#include "../ast_node.hpp"

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

    void EmitRISC(std::ostream &stream, Context &context) const {};
    void Print(std::ostream &stream) const {
        std::cout<<"InitDeclarator\n";
    };
};

#endif
