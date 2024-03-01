#ifndef AST_POSTFIX_EXPRESSION_HPP
#define AST_POSTFIX_EXPRESSION_HPP

#include "../ast_node.hpp"
#include "helpers/var_helpers.hpp"
class PostfixExpression : public Node
{
private:
    Node *expression_;
    std::string postfix_operator_;

public:
    PostfixExpression(
        Node *expression,
        std::string postfix_operator) : expression_(expression),
                               postfix_operator_(postfix_operator){};

    ~PostfixExpression()
    {
        delete expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {};
    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const
    {
        // register where result of postfix expression is assigned to
        if (dest_reg == ""){
            dest_reg = context.ReserveTempRegister();
        }

        expression_->EmitRISCWithDest(stream, context, dest_reg);

        // Increment id
        std::string id = expression_->GetIdentifier();
        ScopeContext* cur_scope = context.GetCurScope();
        VariableContext var = cur_scope->GetVarFromId(id);

        // Load var into temp reg
        std::string temp_reg = context.ReserveTempRegister();
        stream<< "add " << temp_reg << ", " << dest_reg << ", x0" << std::endl;

        // Increment or decrement
        if (postfix_operator_ == "++") {
            stream << "addi " << temp_reg << ", " << temp_reg << ", 1" << std::endl;
        }
        else if (postfix_operator_ == "--"){
            stream << "addi " << temp_reg << ", " << temp_reg << ", -1" << std::endl;
        }

        // Store temp reg back into var
        write_var_value(expression_, context, stream, var, temp_reg);
        context.FreeTempRegister(temp_reg);
    };

    void Print(std::ostream &stream) const
    {
        std::cout<<"pf_expr{";
        expression_->Print(stream);
        std::cout<<postfix_operator_;
        std::cout<<"}";
    };
};

#endif
