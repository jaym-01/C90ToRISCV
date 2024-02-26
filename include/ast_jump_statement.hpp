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


    void EmitRISC(std::ostream &stream, Context &context) const override {};

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override
    {
        // std::cout << "Emitting RISC for ";
        // Print(std::cout);

        // Evaluate expression
        std::string ret_reg = context.GetReturnRegister();

        if (expression_ != nullptr)
        {
            expression_->EmitRISCWithDest(stream, context, ret_reg);
        }

        // Jump to return label
        // Create new label for return.
        stream << "j "<<context.GetFuncReturnLabel()<< std::endl;
    };

    void Print(std::ostream &stream) const override
    {
        stream << "\nreturn_stmt{";
        if (expression_ != nullptr)
        {
            stream << " ";
            expression_->Print(stream);
        }
        stream << " }";

    };
};

#endif
