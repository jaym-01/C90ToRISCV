#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"
#include "../helpers/helpers.hpp"
#include "../helpers/ast_type_helpers.hpp"
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
        // std::cout<<"Em RISC for decl: ";
        // Print(std::cout);

        ScopeContext* cur_scope = context.GetCurScope();
        std::string type = declaration_specifiers_->GetTypeSpecifier();
        TypeDefContext type_context = resolve_type(type, context.GetCurScope());
        type = type_context.type;

        for (auto init_decl : init_declarator_list_->GetNodes())
        {
            if (type == "struct") {
                std::string struct_id = declaration_specifiers_->GetIdentifier();
                VariableContext struct_context = cur_scope->struct_map[struct_id];

                std::string id = init_decl->GetIdentifier();
                struct_context.id = id;
                cur_scope->SetVarContext(id, struct_context);

                init_decl->EmitRISC(stream, context);
                continue;
            } else if(declaration_specifiers_->GetDeclaratorType() == DeclaratorType::TypeDef){
                std::string id = init_decl->GetIdentifier();
                TypeDefContext type_def_context = {.id = id, .type = type, .is_pntr = init_decl->IsPointer()};
                // if()
                cur_scope->AddTypeDef(id, type_def_context);
                continue;
            }else if (init_decl->GetDeclaratorType() == DeclaratorType::Function){
                init_decl->EmitRISC(stream, context);
                context.id_to_func_def[init_decl->GetIdentifier()].return_type = type;
                continue;
            }
            // DeclaratorType declarator_type = ;
            // if (declarator_type  == ) {

            // }

            // 1. Initialise variable in context var_map
            std::string id = init_decl->GetIdentifier();
            VariableContext var_context = init_decl->InitVariableContext(type)[0];
            var_context.pntr_depth += type_context.pntr_depth;
            var_context.is_pntr = var_context.is_pntr | type_context.is_pntr;
            cur_scope->SetVarContext(id, var_context);

            // 2. EmitRISC for init_declarator
            init_decl->EmitRISC(stream, context);
        }
    };

    void Print(std::ostream &stream) const {


        stream<<"\ndecl{ ";
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
        stream<<" }";
    };

    void EmitRISCGlobalVar(std::ostream &stream, Context &context) const {
        bool is_pntr;

        std::string type = declaration_specifiers_->GetTypeSpecifier();
        TypeDefContext type_context = resolve_type(type, context.global_scope);
        type = type_context.type;

        for (auto init_decl : init_declarator_list_->GetNodes())
        {
            if(declaration_specifiers_->GetDeclaratorType() == DeclaratorType::TypeDef){

                std::string id = init_decl->GetIdentifier();
                is_pntr = init_decl->IsPointer();

                TypeDefContext type_def_context = {.id = id, .type = type, .is_pntr = is_pntr, .pntr_depth = is_pntr ? init_decl->GetPointerDepth() : 0};
                // if()
                context.global_scope->AddTypeDef(id, type_def_context);
                continue;
            } else if (init_decl->GetDeclaratorType() == DeclaratorType::Function) {
                init_decl->EmitRISCGlobalVar(stream, context);
                context.id_to_func_def[init_decl->GetIdentifier()].return_type = type;
                continue;
            }

            std::string id = init_decl->GetIdentifier();
            VariableContext var_context = init_decl->InitVariableContext(type)[0];
            var_context.is_global = true;
            var_context.pntr_depth += type_context.pntr_depth;
            var_context.is_pntr = var_context.is_pntr | type_context.is_pntr;
            context.global_scope->SetVarContext(id, var_context);

            // 2. EmitRISC for init_declarator
            init_decl->EmitRISCGlobalVar(stream, context);
        }
    };
};

#endif


// OLD
// ScopeContext *BuildContext(Context &context, ScopeContext *cur_scope) override
// {

//     // std::cout<<"Building scope context for declaration: ";
//     // Print(std::cout);
//     std::string type = declaration_specifiers_->GetIdentifier();
//     int cur_func_var_size = context.GetFuncTotalVarSize();
//     for (auto init_decl : init_declarator_list_->GetNodes())
//     {

//         std::string var_id = init_decl->GetIdentifier();

//         // TODO: If no identifier, invalid declaration
//         VariableContext var_context = init_decl->InitVariableContext(type)[0];
//         cur_scope->SetVarContext(var_id, var_context);
//         context.SetFuncTotalVarSize(cur_func_var_size + calculate_var_size(var_context));
//     }

//     return nullptr;
// };
