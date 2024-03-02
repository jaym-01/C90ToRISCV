
#ifndef AST_FUNCTION_DECLARATOR_HPP
#define AST_FUNCTION_DECLARATOR_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"

// Declarator can either be a direct declarator or a pointer declarator
// Direct declarator can either be an identifier or a function declarator

class FunctionDeclarator : public Node
{
private:
    Node* identifier_;
    NodeList *param_list_;
    // NodeList* identifier_list;

public:
    FunctionDeclarator(Node *identifier, NodeList *param_list) : identifier_(identifier), param_list_(param_list){};

    ~FunctionDeclarator()
    {
        delete identifier_;

        if (param_list_ != nullptr)
            delete param_list_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (param_list_ == nullptr) {
            context.AddFuncDef(identifier_->GetIdentifier(), FuncDefinition{.params = {}});
            return;
        }

        std::vector<Node*> params = param_list_->GetNodes();
        ScopeContext* cur_scope = context.GetCurScope();

        // These are the registers that the caller will use to pass arguments to the callee
        int cur_s0_offset = 0;
        std::vector<VariableContext> arg_contexts = {};
        for (int i = 0; i < params.size(); i++)
        {
            std::string id = params[i]->GetIdentifier();
            VariableContext arg_context = params[i]->InitVariableContext("");
            int var_offset = context.CalcVarOffsetAndUpdate(arg_context);

            // Set off set and save to var map
            arg_context.offset = var_offset;
            arg_contexts.push_back(arg_context);
            cur_scope->SetVarContext(id, arg_context);

            // Need to call after frame size is calculated
            if (i <= 7) {
                // Arg passed through register
                std::string arg_reg = "a" + std::to_string(i); // e.g. a0
                stream << "sw " << arg_reg << ", " << var_offset << "(fp)" << std::endl;
            } else {
                // Arg passed through stack
                int offset = calculate_var_offset(cur_s0_offset, arg_context);
                // TODO: Check this
                std::string temp_reg = context.ReserveRegister(arg_context.type);

                stream<<"lw "<<temp_reg<<", "<<offset<<"(fp)"<< std::endl;
                stream << "sw " <<temp_reg<< ", " << var_offset << "(fp)" << std::endl;

                context.FreeRegister(temp_reg);
            }
        }

        // Add function to context?
        context.AddFuncDef(identifier_->GetIdentifier(), FuncDefinition{
            .params = arg_contexts,
        });
    };

    void EmitRISCGlobalVar(std::ostream &stream, Context &context) const override
    {
        // Do nothing
        std::vector<VariableContext> arg_contexts = {};

        if (param_list_ == nullptr) {
            context.AddFuncDef(identifier_->GetIdentifier(), FuncDefinition{.params = {}});
            return;
        }

        std::vector<Node*> params = param_list_->GetNodes();
        for (int i = 0; i < params.size(); i++)
        {
            std::string id = params[i]->GetIdentifier();
            VariableContext arg_context = params[i]->InitVariableContext(""); // TODO make it work for just type specifier

            arg_contexts.push_back(arg_context);
        }
        context.AddFuncDef(identifier_->GetIdentifier(), FuncDefinition{
            .params = arg_contexts,
        });
    };

    DeclaratorType GetDeclaratorType() const override {
        return DeclaratorType::Function;
    };


    void Print(std::ostream &stream) const override
    {

        std::cout << "f_dec{ ";
        identifier_->Print(stream);
        std::cout << " (";
        if (param_list_ != nullptr) {
            param_list_->Print(stream);
        }
        std::cout << ")";
        std::cout << " }";
    };

    std::string GetIdentifier() const override
    {
        return identifier_->GetIdentifier();
    }
};

#endif
