
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
        // Declarator, initializer
        // declarator_->EmitRISC(stream, context);

        ScopeContext* cur_scope = context.GetCurScope();

        if (initializer_ != nullptr)
        {

            // 1. Store result of initializer in register using available func registers
            std::string dest_reg = context.ReserveTempRegister();
            initializer_->EmitRISCWithDest(stream, context, dest_reg);

            // 2. Store result in memory

            // 2a. Calculate fp offset for variable
            std::string id = declarator_->GetIdentifier();
            std::string type = cur_scope->GetVarType(id);
            int cur_func_offset = context.GetCurFuncOffset();
            int var_offset = calculate_var_offset(cur_func_offset, type);

            // 2b. Move cur func offset to new pos & set offset in context
            context.SetCurFuncOffset(var_offset);
            cur_scope->SetVarOffset(id, var_offset);

            // 2c. Print store instruction
            stream<<"sw "<<dest_reg<<", "<<var_offset<<"(fp)"<<std::endl;

            context.FreeTempRegister(dest_reg);
            // 2. Store result in memory
            // - Save offset for variable in context
            // - Print store instruction (sw reg, offset (fp))
        }
    };

    void Print(std::ostream &stream) const override {
        std::cout<<"init_decl{ ";
        declarator_->Print(stream);
        if (initializer_ != nullptr)
        {
            std::cout << " = ";
            initializer_->Print(stream);
            // std::cout << ", ";
        }
        std::cout<<" }";
    };

    std::string GetIdentifier() const override {
        return declarator_->GetIdentifier();
    }
};

#endif
