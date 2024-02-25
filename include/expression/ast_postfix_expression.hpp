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
        set_var_value(expression_, context, stream, var, temp_reg);
    };

    void Print(std::ostream &stream) const
    {
        expression_->Print(stream);
        std::cout<<postfix_operator_ <<std::endl;
    };
};

#endif

#ifndef AST_FUNCTION_CALL_HPP
#define AST_FUNCTION_CALL_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"

class FunctionCall : public Node
{
private:
    Node *identifier_;
    NodeList* argument_expression_list_;

public:
    FunctionCall(
        Node *identifier,
        NodeList* argument_expression_list) : identifier_(identifier),
                                            argument_expression_list_(argument_expression_list){};

    ~FunctionCall()
    {
        delete identifier_;
        delete argument_expression_list_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {};


    void Print(std::ostream &stream) const
    {
        identifier_->Print(stream);
        std::cout << "(";
        argument_expression_list_->Print(stream);
        std::cout << ")";
    };
};

#endif
