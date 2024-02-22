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
        unary_expression_(unary_expression),
        assignment_operator_(assignment_operator),
        assignment_expression_(assignment_expression){};

    ~AssignmentExpression()
    {
        delete unary_expression_;
        delete assignment_expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {};
    void Print(std::ostream &stream) const
    {
        // std::cout << "a_expression: ";
        unary_expression_->Print(stream);
        std::cout << " " << assignment_operator_ << " ";
        assignment_expression_->Print(stream);
        std::cout << std::endl;
    };
};

#endif
