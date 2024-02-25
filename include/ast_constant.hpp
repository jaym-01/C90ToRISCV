#ifndef AST_CONSTANT_HPP
#define AST_CONSTANT_HPP

#include "ast_node.hpp"

class IntConstant : public Node
{
private:
    int value_;

public:
    IntConstant(int value) : value_(value) {}

    void EmitRISC(std::ostream &stream, Context &context) const override {
        stream << "li a0, " << value_ << std::endl;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const override {
        if (dest_reg == "") {
            dest_reg = context.ReserveTempRegister();
        }

        stream << "li " << dest_reg << ", " << value_ << std::endl;
    };

    void Print(std::ostream &stream) const override {
        stream << "iconst{";
        stream << value_;
        stream << "}";
    };

    int EvalIntExpression() const override {
        return value_;
    };

    int GetNumBranches() const override {
        return 1;
    };
};

#endif
