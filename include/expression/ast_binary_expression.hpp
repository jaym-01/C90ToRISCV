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

    void EvaluateOperation(std::ostream &stream, std::string reg1, std::string reg2, std::string dest_reg) const {
        if (b_operator_ == "+") {
            stream << "add " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "-"){
            stream << "sub " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "*") {
            stream << "mul " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "/"){
            stream << "div " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "%"){
            stream << "rem " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }

        // Bitwise operations
        else if (b_operator_ == "<<"){
            stream << "sll " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == ">>") {
            stream << "srl " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }

        // Logical operations
        else if (b_operator_ == "<") {
            stream << "slt " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "<=") {
            // Same as !(reg1 > reg2)
            stream << "sgt " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
        }
        else if (b_operator_ == ">") {
            stream << "sgt " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == ">=") {
            // Same as !(reg1 < reg2)
            stream << "slt " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
        }
        else if (b_operator_ == "==") {
            stream << "sub " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
        }
        else if (b_operator_ == "!=") {
            stream << "sub " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "&") {
            stream << "and " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "|") {
            stream << "or " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "^") {
            stream << "xor " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }

         else if (b_operator_ == "&&" ) {
            stream << "seqz " << reg1 << ", " << reg1 << std::endl;
            stream << "seqz " << reg2 << ", " << reg2 << std::endl;
            stream << "or " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
        } else if (b_operator_ == "||" ) {
            stream << "or " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            stream << "snez " << dest_reg << ", " << dest_reg << std::endl;
        }
    }

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const {
        std::cout<<"Emitting RISC for binary expression: ";
        Print(std::cout);
        std::cout<<std::endl;

        std::string reg1 = "";
        std::string reg2 = "";

        // TODO: What happens if you run out of temp registers
        // Evaluate child with most branches first
        if (left_operand_->GetNumBranches() > 1) {
            std::cout<<"Evaluating left operand first"<<std::endl;
            left_operand_->EmitRISCWithDest(stream, context, reg1);
            right_operand_->EmitRISCWithDest(stream, context, reg2);
        } else {
            std::cout << "Evaluating right operand first" << std::endl;
            right_operand_->EmitRISCWithDest(stream, context, reg2);
            left_operand_->EmitRISCWithDest(stream, context, reg1);
        }


        // TODO: need to find type
        if (dest_reg == "") {
            dest_reg = context.ReserveRegister("int");
        }

        EvaluateOperation(stream, reg1, reg2, dest_reg);

        // std::cout<<"Operand 1 stored in reg: "<<reg1<<std::endl;
        // std::cout<<"Operand 2 stored in reg: "<<reg2<<std::endl;

        context.FreeRegister(reg1);
        context.FreeRegister(reg2);

        // std::cout<<"Binary expression result in reg: "<<dest_reg<<std::endl;
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
