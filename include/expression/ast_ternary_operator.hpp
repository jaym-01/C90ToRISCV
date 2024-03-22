#ifndef AST_TERNARY_OPERATOR_HPP
#define AST_TERNARY_OPERATOR_HPP

#include "../ast_node.hpp"

class TernaryOperator : public Node {
private:
    Node *condition_;
    Node *true_expression_;
    Node *false_expression_;
public:
    TernaryOperator(Node *condition, Node* true_expression, Node* false_expression) : condition_(condition), true_expression_(true_expression), false_expression_(false_expression){};

    ~TernaryOperator()
    {
        delete condition_;
        delete true_expression_;
        delete false_expression_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override{
        std::string endif_label = context.GetNewLabel("endif"), else_label = context.GetNewLabel("else");

        // evaluate condition
        std::string cond_reg = "";
        condition_->EmitRISCWithDest(stream, context, cond_reg);

        // branch when condition is false
        stream << "beq " << cond_reg << ", zero, " << else_label << std::endl;
        context.FreeRegister(cond_reg);

        true_expression_->EmitRISCWithDest(stream, context, dest);
        stream << "j " << endif_label << std::endl;

        stream << else_label << ":" << std::endl;

        false_expression_->EmitRISCWithDest(stream, context, dest);

        stream << endif_label << ":" << std::endl;
    }

    void DefineConstantType(std::string type) override {
        true_expression_->DefineConstantType(type);
        false_expression_->DefineConstantType(type);
    }

    void Print(std::ostream &stream) const override {
        stream << "TernaryOperator( ";
        condition_->Print(stream);
        stream << ") ? ( ";
        true_expression_->Print(stream);
        stream << " ) : ( ";
        false_expression_->Print(stream);
        stream << " )" << std::endl;
    }
};

#endif
