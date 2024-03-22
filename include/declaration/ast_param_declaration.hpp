#ifndef AST_PARAM_DECLARATION_HPP
#define AST_PARAM_DECLARATION_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"
#include "helpers/helpers.hpp"
class ParamDeclaration : public Node
{
private:
    Node* declaration_specifiers_;
    Node* declarator_;

    // Init_declarator_list:
    // Direct_declarators (x), declarators (*x) & init_declarators (x = 5)
public:
    ParamDeclaration(
        Node* declaration_specifiers,
        Node* declarator) : declaration_specifiers_(declaration_specifiers), declarator_(declarator){};

    ~ParamDeclaration()
    {
        delete declaration_specifiers_;
        if (declarator_ != nullptr)
            delete declarator_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const
    {
        // // std::cout<<"Em RISC for decl: ";
        // // Print(std::cout);

        // ScopeContext *cur_scope = context.GetCurScope();
        // std::string type = declaration_specifiers_->GetIdentifier();

        // for (auto init_decl : init_declarator_list_->GetNodes())
        // {
        //     // 1. Initialise variable in context var_map
        //     std::string id = init_decl->GetIdentifier();
        //     VariableContext var_context = init_decl->InitVariableContext(type)[0];
        //     cur_scope->SetVarContext(id, var_context);

        //     // 2. EmitRISC for init_declarator
        //     init_decl->EmitRISC(stream, context);
        // }
    };

    std::string GetIdentifier() const {
        return declarator_->GetIdentifier();
    };

    std::vector<VariableContext> InitVariableContext(std::string t) override {

        // Don't need to use type from arg
        std::string type = declaration_specifiers_->GetTypeSpecifier();
        VariableContext var_context = declarator_->InitVariableContext(type)[0];
        var_context.is_param = true;

        return {var_context};
    };

    void Print(std::ostream &stream) const
    {

        stream<<"\np_decl{";
        declaration_specifiers_->Print(stream);
        stream<<" ";
        declarator_->Print(stream);
        stream<<"}";
    };
};

#endif
