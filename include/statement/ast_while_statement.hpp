#ifndef AST_WHILE_STATEMENT_HPP
#define AST_WHILE_STATEMENT_HPP

#include "ast_node.hpp"

// NOTE: Create a class for each type of direct declarator?
class WhileStatement : public Node
{
private:
    Node* cond_expression_;
    Node* statement_;

public:
    WhileStatement(Node *expression, Node *statement) : cond_expression_(expression), statement_(statement){};

    ~WhileStatement()
    {
        delete cond_expression_;
        delete statement_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override
    {

        ScopeContext *scope = context.GetCurScope();
        std::string endwhile_label = context.GetNewLabel("endwhile_label");
        std::string startwhile_label = context.GetNewLabel("startwhile_label");

        std::string prev_contloop_label = context.cur_contloop_label;
        std::string prev_breakloop_label = context.cur_breakloop_label;
        context.SetBreakContLabels(endwhile_label, startwhile_label);

        // 1. Evaluate condition
        stream << startwhile_label << ":" << std::endl;

        std::string cond_reg = "";
        cond_expression_->EmitRISCWithDest(stream, context, cond_reg);
        // If condition is == 0, jump to endwhile label
        stream << "beq " << cond_reg << ", zero, " << endwhile_label << std::endl;
        context.FreeRegister(cond_reg);

        // 2. Emit if statement
        std::string empty_reg = "";
        statement_->EmitRISCWithDest(stream, context, empty_reg);
        stream << "j " << startwhile_label << std::endl;
        context.FreeRegister(empty_reg);

        stream << endwhile_label << ":" << std::endl;

        context.SetBreakContLabels(prev_breakloop_label, prev_contloop_label);
    };

    void Print(std::ostream &stream) const
    {
        std::cout << "\nwhile (";
        cond_expression_->Print(stream);
        std::cout << ")";
        std::cout << std::endl<< "{ ";
        statement_->Print(stream);
        std::cout << std::endl<< "} ";
    };
};

#endif
