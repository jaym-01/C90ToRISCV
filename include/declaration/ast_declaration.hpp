#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "../ast_node.hpp"

class Declaration : public Node
{
private:
    Node *declaration_specifiers_;
    Node *declarator_;

public:
    Declaration(
        Node *declaration_specifiers,
        Node *declarator
    ) : declaration_specifiers_(declaration_specifiers), declarator_(declarator){};

    ~Declaration()
    {
        delete declaration_specifiers_;
        delete declarator_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {};
    void Print(std::ostream &stream) const {
        std::cout<<"Declaration: ";
        declaration_specifiers_->Print(stream);
        std::cout<<" ";
        declarator_->Print(stream);
        std::cout<<std::endl;
    };
};

#endif
