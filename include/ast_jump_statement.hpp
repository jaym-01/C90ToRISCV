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

        // Evaluate expression
        std::string ret_reg = context.GetReturnRegister();
        std::cout << "Emitting RISC for ";
        Print(std::cout);

        if (expression_ != nullptr)
        {
            expression_->EmitRISCWithDest(stream, context, ret_reg);
        }

        // Jump to return label
        stream << "j return" << std::endl;
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
