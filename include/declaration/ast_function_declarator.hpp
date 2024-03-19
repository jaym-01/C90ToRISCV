
#ifndef AST_FUNCTION_DECLARATOR_HPP
#define AST_FUNCTION_DECLARATOR_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"
#include "../helpers/memory_helpers.hpp"

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
        int cur_s0_offset = 0, cur_a_reg = 0, cur_fp_reg = 0;
        std::vector<VariableContext> arg_contexts = {};
        std::string ins;
        for (std::size_t i = 0; i < params.size(); i++)
        {
            std::string id = params[i]->GetIdentifier();
            // type is defined further down, don't need to specify it here
            VariableContext arg_context = params[i]->InitVariableContext("")[0];
            // allocates stack memory to param
            int var_offset = context.CalcVarOffsetAndUpdate(arg_context);

            // Set off set and save to var map
            arg_context.offset = var_offset;
            arg_contexts.push_back(arg_context);
            cur_scope->SetVarContext(id, arg_context);

            // Need to call after frame size is calculated
            // can get rid of this condition?
            if(arg_context.is_array && cur_a_reg <= 7){
                // if in register -> store address on stack
                stream << "sw a" << std::to_string(cur_a_reg) << ", " << var_offset << "(fp)" <<  std::endl;
                cur_a_reg++;

            } else if ((arg_context.GetType() == "int" || arg_context.GetType() == "char" || arg_context.GetType() == "unsigned") && cur_a_reg <= 7 && !arg_context.is_array) {
                // Arg passed through register
                // if(arg_context.type == "int") ins = "sw";
                // else if(arg_context.type == "char") ins = "sb";
                stream << get_mem_write(arg_context.GetType(), arg_context.is_pntr) << " a" << std::to_string(cur_a_reg) << ", " << var_offset << "(fp)" << std::endl;
                cur_a_reg++;

            } else if((arg_context.GetType() == "float" || arg_context.GetType() == "double") && cur_fp_reg <= 7 && !arg_context.is_array){
                // if(arg_context.type == "float") ins = "fsw";
                // else if(arg_context.type == "double") ins = "fsd";
                stream << get_mem_write(arg_context.type, arg_context.is_pntr) << " fa" << std::to_string(cur_fp_reg) << ", " << var_offset << "(fp)" << std::endl;
                cur_fp_reg++;

            } else {
                // Arg passed through stack
                // always at the bottom of the previous stack
                int offset = calculate_var_offset(cur_s0_offset, arg_context);

                std::string temp_reg = context.ReserveRegister(arg_context.GetType());

                // TODO: CHECK THIS
                // should offset be -offset to make it positive?
                // to access above the current stack?
                // std::string l_ins, s_ins;
                // if(arg_context.type == "int"){
                //     l_ins = "lw";
                //     s_ins = "sw";
                // }
                // else if(arg_context.type == "char"){
                //     l_ins = "lbu";
                //     s_ins = "sbu";
                // }
                // // TODO: Check this
                // else if(arg_context.type == "float"){
                //     l_ins = "flw";
                //     s_ins = "fsw";
                // }
                // else if(arg_context.type == "double"){
                //     l_ins = "fld";
                //     s_ins = "fsd";
                // }
                stream << get_mem_read(arg_context.GetType(), arg_context.is_pntr)  <<" "<<temp_reg<<", "<<offset<<"(fp)"<< std::endl;
                stream << get_mem_write(arg_context.GetType(), arg_context.is_pntr) << " " <<temp_reg<< ", " << var_offset << "(fp)" << std::endl;

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
        for (std::size_t i = 0; i < params.size(); i++)
        {
            std::string id = params[i]->GetIdentifier();
            VariableContext arg_context = params[i]->InitVariableContext("")[0]; // TODO make it work for just type specifier

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
