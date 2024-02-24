#ifndef AST_ASSIGNMENT_EXPRESSION_HPP
#define AST_ASSIGNMENT_EXPRESSION_HPP

#include "../ast_node.hpp"

class AssignmentExpression : public Node
{
private:
    Node *unary_expression_;
    std::string assignment_operator_;
    Node *assignment_expression_;

public:
    AssignmentExpression(
        Node *unary_expression,
        std::string assignment_operator,
        Node *assignment_expression) :
        unary_expression_(unary_expression), assignment_operator_(assignment_operator), assignment_expression_(assignment_expression){};

    ~AssignmentExpression()
    {
        delete unary_expression_;
        delete assignment_expression_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string dest_reg) const {
        // 1. Store result of initializer in register using available func registers
        std::cout<<"Emitting RISC for ";
        Print(std::cout);

        // Other case: y = x = 10;
        bool initialised_dest_reg = false;

        if (dest_reg == "") {
            // Case: y = 5 + 10;
            initialised_dest_reg = true;
            dest_reg = context.ReserveTempRegister();
        }

        assignment_expression_->EmitRISCWithDest(stream, context, dest_reg);

        // 2a. Calculate fp offset for variable
        // TODO: what if unary_expression_ doesn't have an identifier?
        if (unary_expression_->GetIdentifier() == "") {
            throw std::runtime_error("Error: unary_expression_ in assignment_expression_ doesn't have an identifier");
        }

        std::string var_id = unary_expression_->GetIdentifier();
        ScopeContext *cur_scope = context.GetCurScope();
        VariableContext var = cur_scope->GetVarFromId(var_id);

        int var_offset;
        if (var.offset > 0) {

            // Create new func offset
            int cur_func_offset = context.GetCurFuncOffset();
            var_offset = calculate_var_offset(cur_func_offset, var.type);
            context.SetCurFuncOffset(var_offset);
            cur_scope->SetVarOffset(var_id, var_offset);
        } else {
            var_offset = var.offset;
        }

        stream << "sw " << dest_reg << ", " << var_offset << "(fp)" << std::endl;

        if (initialised_dest_reg) {
            context.FreeTempRegister(dest_reg);
        }
    };

    void EmitRISC(std::ostream &stream, Context &context) const {
        // 1. Store result of initializer in register using available func registers
        std::string dest_reg = context.ReserveTempRegister();
        assignment_expression_->EmitRISCWithDest(stream, context, dest_reg);

        // // 2. Store result in memory

        // // 2a. Calculate fp offset for variable
        // std::string id = declarator_->GetIdentifier();
        // std::string type = cur_scope->GetVarType(id);
        // int cur_func_offset = context.GetCurFuncOffset();
        // int var_offset = calculate_var_offset(cur_func_offset, type);

        // // 2b. Move cur func offset to new pos & set offset in context
        // context.SetCurFuncOffset(var_offset);
        // cur_scope->SetVarOffset(id, var_offset);

        // // 2c. Print store instruction
        // stream<<"sw "<<dest_reg<<", "<<var_offset<<"(fp)"<<std::endl;

        // context.FreeTempRegister(dest_reg);
        // // 2. Store result in memory
        // // - Save offset for variable in context
        // // - Print store instruction (sw reg, offset (fp))
    };

    void Print(std::ostream &stream) const
    {
        std::cout << "ass_expr{ ";
        unary_expression_->Print(stream);
        std::cout << " " << assignment_operator_ << " ";
        assignment_expression_->Print(stream);
        std::cout << " }"<<std::endl;
    };
};

#endif
