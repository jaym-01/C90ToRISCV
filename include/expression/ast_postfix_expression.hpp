#ifndef AST_POSTFIX_EXPRESSION_HPP
#define AST_POSTFIX_EXPRESSION_HPP

#include "../ast_node.hpp"
#include "../helpers/var_helpers.hpp"
#include "../helpers/pointer_helpers.hpp"
#include "../ast_constant.hpp"
class PostfixExpression : public Node
{
private:
    Node *expression_;
    std::string postfix_operator_;
    std::string type_;

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
        // TODO: Check this
        // Increment id
        std::string id = expression_->GetIdentifier();
        ScopeContext* cur_scope = context.GetCurScope();
        VariableContext var = cur_scope->GetVarFromId(id);

        // register where result of postfix expression is assigned to
        if (dest_reg == ""){
            dest_reg = context.ReserveRegister(var.type);
        }

        expression_->EmitRISCWithDest(stream, context, dest_reg);

        std::string temp_reg = context.ReserveRegister(var.type);
        // TODO: Check if this is wrong?
        // Load var into temp reg
        // stream<< "add " << temp_reg << ", " << dest_reg << ", x0" << std::endl;

        int offset = one_pointer_offset(context, expression_);
        // Increment or decrement
        if (postfix_operator_ == "++") {
            stream << "addi " << temp_reg << ", " << dest_reg << ", " << offset << std::endl;
        }
        else if (postfix_operator_ == "--"){
            stream << "addi " << temp_reg << ", " << dest_reg << ", -" << offset << std::endl;
        }



        // Store temp reg back into var
        write_var_value(expression_, context, stream, var, temp_reg);
        context.FreeRegister(temp_reg);
    };

    void DefineConstantType(std::string type) override {
        type_ = type;
        expression_->DefineConstantType(type);
    }
    std::string GetType() const override {
        return expression_->GetType();
    }

    void Print(std::ostream &stream) const
    {
        std::cout<<"pf_expr{";
        expression_->Print(stream);
        std::cout<<postfix_operator_;
        std::cout<<"}";
    };

    bool IsMemoryReference(Context &context) const override {
        return expression_->IsMemoryReference(context);
    }
};

#endif
