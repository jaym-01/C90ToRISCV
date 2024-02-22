#ifndef AST_BINARY_EXPRESSION_HPP
#define AST_BINARY_EXPRESSION_HPP

#include "../ast_node.hpp"

class BinaryExpression : public Node
{
private:
    Node *left_operand_;
    std::string b_operator_;
    Node *right_operand_;

public:
    BinaryExpression(
        Node *left_operand,
        std::string b_operator,
        Node *right_operand) : left_operand_(left_operand),
                               b_operator_(b_operator),
                               right_operand_(right_operand){};

    ~BinaryExpression()
    {
        delete left_operand_;
        delete right_operand_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {};
    void Print(std::ostream &stream) const
    {
        std::cout<<"(";
        left_operand_->Print(stream);
        stream << " " << b_operator_ << " ";
        right_operand_->Print(stream);
        std::cout<<")";
    };
};

#endif
