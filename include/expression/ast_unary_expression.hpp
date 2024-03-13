#ifndef AST_UNARY_EXPRESSION_HPP
#define AST_UNARY_EXPRESSION_HPP

#include "../ast_node.hpp"
#include "helpers/var_helpers.hpp"

class UnaryExpression : public Node
{
private:
    std::string unary_operator_;
    Node *expression_;

public:
    UnaryExpression(
        std::string unary_operator,
        Node *expression ) : unary_operator_(unary_operator), expression_(expression){};

    ~UnaryExpression()
    {
        delete expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {};
    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const {
        std::string id = expression_->GetIdentifier();
        ScopeContext* cur_scope = context.GetCurScope();
        VariableContext var = cur_scope->GetVarFromId(id);


        // 1. Evaluate
        if (dest_reg == ""){
            dest_reg = context.ReserveRegister(var.type);
        }

        if(unary_operator_ != "&") expression_->EmitRISCWithDest(stream, context, dest_reg);
        // 2. Apply unary operator
        // Note: do nothing for '+' operator
        if (unary_operator_ == "++") {
            stream << "addi " << dest_reg << ", " << dest_reg << ", 1" << std::endl;
        } else if (unary_operator_ == "--") {
            stream << "addi " << dest_reg << ", " << dest_reg << ", -1" << std::endl;
        } else if (unary_operator_ == "-") {

            // negate instruction, pseudo-ins for sub dest_reg, x0, dest_reg
            stream << "neg " << dest_reg << ", " << dest_reg << std::endl;
        } else if (unary_operator_ == "~") {
            std::string tmp_reg = context.ReserveRegister(var.type);
            stream << "li " << tmp_reg << ", -1" << std::endl; // Load 0xFFFFFFFF into temp_reg
            stream << "xor " << dest_reg << ", " << dest_reg <<", "<<tmp_reg << std::endl;
        } else if (unary_operator_ == "!") {
            stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
        } else if(unary_operator_ == "&"){
            // TODO: get it to write the address of data into dest_reg
            std::string var_name = expression_->GetIdentifier();
            ScopeContext* cur_scope = context.GetCurScope();
            VariableContext var = cur_scope->GetVarFromId(var_name);

            // TODO: handle global case
            stream << "addi " << dest_reg << ", fp, " << var.offset << std::endl;
        }

        // Store result back to var if INC / DEC op
        if (unary_operator_ == "++" || unary_operator_ == "--") {

            write_var_value(expression_, context, stream, var, dest_reg);
        }
    };

    void DefineConstantType(std::string type) override {
        expression_->DefineConstantType(type);
    };

    void Print(std::ostream &stream) const
    {
        stream<<"u_expr{ ";
        stream << unary_operator_;
        expression_->Print(stream);
        stream<<" }";
    };
};

#endif
