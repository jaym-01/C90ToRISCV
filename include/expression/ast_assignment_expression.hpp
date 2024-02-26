#ifndef AST_ASSIGNMENT_EXPRESSION_HPP
#define AST_ASSIGNMENT_EXPRESSION_HPP

#include "../ast_node.hpp"
#include "helpers/var_helpers.hpp"

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

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const {
        // std::cout<<"Emitting RISC for ";
        // Print(std::cout);
        // std::cout<<std::endl;

        if (dest_reg == "") {
            dest_reg = context.ReserveTempRegister();
        }

        assignment_expression_->EmitRISCWithDest(stream, context, dest_reg);
        if (unary_expression_->GetIdentifier() == "") {
            throw std::runtime_error("Error: unary_expression_ in assignment_expression_ doesn't have an identifier");
        }

        // 1. Get var from context
        std::string var_id = unary_expression_->GetIdentifier();
        ScopeContext* cur_scope = context.GetCurScope();
        VariableContext var = cur_scope->GetVarFromId(var_id);

        // 2. Calculate var offset if not already done
        // TODO: optimize by doing graph coloring and assigning variables to saved regs?
        int var_offset;
        if (!var.is_global && var.offset > 0) {
            var_offset = context.CalcVarOffsetAndUpdate(var);
            cur_scope->SetVarOffset(var_id, var_offset);
            var.offset = var_offset;
        }

        // Dest reg here holds the value to be stored in var
        write_var_value(unary_expression_, context, stream, var, dest_reg);
    };

    void EmitRISC(std::ostream &stream, Context &context) const {
    };

    void Print(std::ostream &stream) const
    {
        // stream<<std::endl;
        stream << "ass_expr{ ";
        unary_expression_->Print(stream);
        stream << " " << assignment_operator_ << " ";
        assignment_expression_->Print(stream);
        stream << " }";
    };
};

#endif
