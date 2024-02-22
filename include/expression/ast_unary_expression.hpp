#ifndef AST_UNARY_EXPRESSION_HPP
#define AST_UNARY_EXPRESSION_HPP

#include "../ast_node.hpp"

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
    void Print(std::ostream &stream) const
    {
        std::cout << unary_operator_;
        expression_->Print(stream);
    };
};

#endif
