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
        std::cout<<"Emitting RIS for binary expression: ";
        Print(std::cout);
        std::cout<<std::endl;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string dest_reg) const {
        // std::cout<<"Emitting RIS for binary expression"<<std::endl;;
        std::string reg1 = context.ReserveTempRegister();
        std::string reg2 = context.ReserveTempRegister();

        // TODO: What happens if you run out of temp registers
        left_operand_->EmitRISCWithDest(stream, context, reg1);
        right_operand_->EmitRISCWithDest(stream, context, reg2);

        if (b_operator_ == "+") {
            stream << "add " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "-") {
            stream << "sub " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "*") {
            stream << "mul " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "/") {
            stream << "div " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else {
            throw std::runtime_error("Error: Invalid binary operator");
        }
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
