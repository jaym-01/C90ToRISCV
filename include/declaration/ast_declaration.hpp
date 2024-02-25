#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"
#include "helpers.hpp"
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

    ScopeContext *BuildContext(Context &context, ScopeContext* cur_scope) override
    {

        // std::cout<<"Building scope context for declaration: ";
        // Print(std::cout);
        std::string type = declaration_specifiers_->GetIdentifier();
        // int cur_func_offset = context.GetCurFuncOffset();

        int cur_func_var_size = context.GetFuncTotalVarSize();
        for (auto init_decl : init_declarator_list_->GetNodes())
        {

            std::string var_id = init_decl->GetIdentifier();
            // TODO: If no identifier, invalid declaration
            VariableContext var_context = init_decl->InitVariableContext(type);
            cur_scope->SetVarContext(var_id, var_context);

            context.SetFuncTotalVarSize(cur_func_var_size + calculate_var_size(var_context));
        }

        return nullptr;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {
        std::cout<<"Em RISC for decl: ";
        Print(std::cout);

        // 1. Get appropriate context for declaration
        for (auto init_decl : init_declarator_list_->GetNodes())
        {
            init_decl->EmitRISC(stream, context);
        }

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
