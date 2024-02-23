#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"


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
        std::cout<<"Em RISC for decl: \n";
        Print(std::cout);
        std::cout<<std::endl;

        // 1. Iterate through init_decl_list and save vars
        // std::vector<std::string> var_ids;
        // for (auto init_decl : init_declarator_list_->GetNodes())
        // {
        //     var_ids.push_back(init_decl->GetIdentifier());
        // }

        // 2. Add variables to scope top
        // context->scope[0]->AddVariables(var_ids);

        // 3. Emit RISC for each init_decl
        // for (auto init_decl : init_declarator_list_->GetNodes())
        // {
        //     init_decl->EmitRISC(stream, context);
        // }
    };

    void Print(std::ostream &stream) const {

        stream<<"decl{ ";
        declaration_specifiers_->Print(stream);
        stream<<" ";
        NodeList *init_declarator_list = init_declarator_list_;
        stream<<"[ ";
        for (auto init_decl : init_declarator_list->GetNodes())
        {
            init_decl->Print(stream);
            if (init_decl != init_declarator_list->GetNodes().back())
                stream<<", ";
        }
        stream<< " ]";
        stream<<" }"<<std::endl;
    };
};



#endif
