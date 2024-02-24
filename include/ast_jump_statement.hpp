#ifndef AST_JUMP_STATEMENT_HPP
#define AST_JUMP_STATEMENT_HPP

#include "ast_node.hpp"

class ReturnStatement : public Node
{
private:
    Node *expression_;

public:
    ReturnStatement(Node *expression) : expression_(expression) {}
    ~ReturnStatement()
    {
        delete expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        if (expression_ != nullptr)
        {
            expression_->EmitRISC(stream, context);
        }
        stream << "ret" << std::endl;
    };

    void Print(std::ostream &stream) const override
    {
        stream << "return_stmt{";
        if (expression_ != nullptr)
        {
            stream << " ";
            expression_->Print(stream);
        }
        stream << " }" << std::endl;
        // stream << ";" << std::endl;

    };
};

#endif
