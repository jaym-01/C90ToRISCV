#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "../ast_node.hpp"

class Declaration : public Node
{
private:
    Node *declaration_specifiers_;
    NodeList *init_declarator_list_;

// Init_declarator_list:
// Direct_declarators (x), declarators (*x) & init_declarators (x = 5)
public:
    Declaration(
        Node *declaration_specifiers,
        NodeList *init_declarator_list) :
        declaration_specifiers_(declaration_specifiers), init_declarator_list_(init_declarator_list){};

    ~Declaration()
    {
        delete declaration_specifiers_;
        delete init_declarator_list_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {
        std::cout<<"Emitting RISC for declaration"<<std::endl;
        declaration_specifiers_->EmitRISC(stream, context);

        // For each of the declarators, emit RISC
        init_declarator_list_->EmitRISC(stream, context);
    };

    void Print(std::ostream &stream) const {
        // std::cout<<"Declaration: ";
        declaration_specifiers_->Print(stream);
        std::cout<<" ";
        // declarator_->Print(stream);
        init_declarator_list_->Print(stream);
        std::cout<<std::endl;
    };
};

#endif
