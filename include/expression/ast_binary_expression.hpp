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

    void EmitRISC(std::ostream &stream, Context &context) const {

    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const {
        std::cout<<"Emitting RISC for binary expression: ";
        Print(std::cout);
        std::cout<<std::endl;

        std::string reg1 = "";
        std::string reg2 = "";

        // TODO: What happens if you run out of temp registers
        // Evaluate child with most branches first
        if (left_operand_->GetNumBranches() > 1) {
            left_operand_->EmitRISCWithDest(stream, context, reg1);
            right_operand_->EmitRISCWithDest(stream, context, reg2);
        } else {
            right_operand_->EmitRISCWithDest(stream, context, reg2);
            left_operand_->EmitRISCWithDest(stream, context, reg1);
        }


        if (dest_reg == "") {
            dest_reg = context.ReserveTempRegister();
        }

        if (b_operator_ == "+") {
            stream << "add " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "-") {
            stream << "sub " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "*") {
            stream << "mul " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "/") {
            stream << "div " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "%") {
            stream << "rem " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        context.FreeTempRegister(reg1);
        context.FreeTempRegister(reg2);
    };

    int EvalIntExpression() const override {
        int left = left_operand_->EvalIntExpression();
        int right = right_operand_->EvalIntExpression();

        if (b_operator_ == "+") {
            return left + right;
        } else if (b_operator_ == "-") {
            return left - right;
        } else if (b_operator_ == "*") {
            return left * right;
        } else if (b_operator_ == "/") {
            return left / right;
        } else {
            throw std::runtime_error("Error: Invalid binary operator");
        }
    }

    int GetNumBranches() const override{
        return 2;
    };

    void Print(std::ostream &stream) const
    {
        std::cout<<"bin_expr{ ";
        left_operand_->Print(stream);
        stream << " " << b_operator_ << " ";
        right_operand_->Print(stream);
        std::cout<<" }";
    };
};

#endif
