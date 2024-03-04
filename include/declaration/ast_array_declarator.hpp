
#ifndef AST_ARRAY_DECLARATOR_HPP
#define AST_ARRAY_DECLARATOR_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"

// Declarator can either be a direct declarator or a pointer declarator
// Direct declarator can either be an identifier or a function declarator

class ArrayDeclarator : public Node
{
private:
    Node *identifier_;
    Node *size_expression_;

public:
    ArrayDeclarator(Node *identifier, Node *size_expresion) : identifier_(identifier), size_expression_(size_expresion){};

    ~ArrayDeclarator()
    {
        delete identifier_;
        delete size_expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        // // Declarator, initializer
        // // declarator_->EmitRISC(stream, context);

        // ScopeContext *cur_scope = context.GetCurScope();

        // if (initializer_ != nullptr)
        // {

        //     // 1. Store result of initializer in register using available func registers
        //     std::string dest_reg = "";
        //     initializer_->EmitRISCWithDest(stream, context, dest_reg);

        //     // 2. Store result in memory

        //     // 2a. Calculate fp offset for variable
        //     std::string id = declarator_->GetIdentifier();
        //     std::string type = cur_scope->GetVarType(id);
        //     int cur_func_offset = context.GetCurFuncOffset();
        //     int var_offset = calculate_var_offset(cur_func_offset, type);

        //     // 2b. Move cur func offset to new pos & set offset in context
        //     context.SetCurFuncOffset(var_offset);
        //     cur_scope->SetVarOffset(id, var_offset);

        //     // 2c. Print store instruction
        //     stream << "sw " << dest_reg << ", " << var_offset << "(fp)" << std::endl;

        //     context.FreeRegister(dest_reg);
        //     // 2. Store result in memory
        //     // - Save offset for variable in context
        //     // - Print store instruction (sw reg, offset (fp))
        // }
    };

    void Print(std::ostream &stream) const override
    {
        stream<< "arr_decl{ ";
        identifier_->Print(stream);

        stream<<" [ ";
        if (size_expression_ != nullptr){
            size_expression_->Print(stream);
        }
        stream<<" ]";
        stream<< " }";
    };

    VariableContext InitVariableContext(std::string type) override {
        if (size_expression_ != nullptr){
            int size = size_expression_->EvalExpression(type);
            return {
                .id=identifier_->GetIdentifier(),
                .type=type,
                .array_size=size,
                .is_array=true,
            };
        } else {
            // return VariableContext{type : type, offset : 1, array_size : -1, is_array : true};
            return {
                .id=identifier_->GetIdentifier(),
                .type = type,
                .array_size = -1,
                .is_array = true,
            };
        }
    };

    std::string GetIdentifier() const override
    {
        return identifier_->GetIdentifier();
    }

    DeclaratorType GetDeclaratorType() const override{
        return DeclaratorType::Array;
    }
};

#endif
