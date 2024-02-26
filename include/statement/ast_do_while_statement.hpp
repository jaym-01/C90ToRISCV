#ifndef AST_DO_WHILE_STATEMENT_HPP
#define AST_DO_WHILE_STATEMENT_HPP

#include "ast_node.hpp"

// NOTE: Create a class for each type of direct declarator?
class DoWhileStatement : public Node
{
private:
    Node *statement_;
    Node *cond_expression_;

public:
    DoWhileStatement(Node *statement, Node *expression) : statement_(statement), cond_expression_(expression){};

    ~DoWhileStatement()
    {
        delete cond_expression_;
        delete statement_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override {

        ScopeContext *scope = context.GetCurScope();
        std::string startwhile_label = context.GetNewLabel("startwhile_label");
        // std::string endwhile_label = context.GetNewLabel("endwhile_label");

        // 1. Evaluate condition
        stream << startwhile_label << ":" << std::endl;

        // 2. Emit if statement
        std::string empty_reg = "";
        statement_->EmitRISCWithDest(stream, context, empty_reg);
        context.FreeTempRegister(empty_reg);

        std::string cond_reg = "";
        cond_expression_->EmitRISCWithDest(stream, context, cond_reg);
        stream << "bne " << cond_reg << ", zero, " << startwhile_label << std::endl;
        context.FreeTempRegister(cond_reg);
        // stream <<"j "<<endwhile_label<<std::endl;
        // stream << endwhile_label << ":" << std::endl;
    };

    void Print(std::ostream &stream) const
    {
        std::cout << "\ndo{ ";
        statement_->Print(stream);
        std::cout << std::endl
                  << "} ";
        std::cout << "\nwhile (";
        cond_expression_->Print(stream);
        std::cout << ")";
    };
};

#endif
