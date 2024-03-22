#ifndef AST_IF_ELSE_STATEMENT_HPP
#define AST_IF_ELSE_STATEMENT_HPP

#include "ast_node.hpp"

// NOTE: Create a class for each type of direct declarator?
class IfElseStatement : public Node
{
private:
    Node* cond_expression_;
    Node* if_statement_;
    Node* else_statement_;

public:
    IfElseStatement(Node* expression, Node *if_statement, Node* else_statement) :
        cond_expression_(expression), if_statement_(if_statement), else_statement_(else_statement){};

    ~IfElseStatement()
    {
        if(cond_expression_ != nullptr) delete cond_expression_;
        if(if_statement_ != nullptr) delete if_statement_;
        if(else_statement_ != nullptr) delete else_statement_;
    };
    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override
    {
        std::cout<<"Emitting RISC for if else statement: "<<std::endl;
        ScopeContext* scope = context.GetCurScope();
        std::string endif_label = context.GetNewLabel("endif_label");
        std::string else_label = else_statement_ != nullptr ? context.GetNewLabel("else_label") : "";

        // 1. Evaluate condition
        std::string cond_reg = "";
        cond_expression_->EmitRISCWithDest(stream, context, cond_reg);

        // If condition is == 0, jump to else/endif label
        if (else_statement_ != nullptr) {
            stream <<"beq "<< cond_reg<<", zero, "<<else_label << std::endl;
        } else {
            stream<<"beq "<<cond_reg<<", zero, "<<endif_label<<std::endl;
        }
        // std::cout<<"Cond reg: "<<cond_reg<<std::endl;
        context.FreeRegister(cond_reg);

        // 2. Emit if statement
        std::string empty_reg = "";
        if_statement_->EmitRISCWithDest(stream, context, empty_reg);
        stream<<"j "<<endif_label<<std::endl;
        context.FreeRegister(empty_reg);

        if (else_statement_ != nullptr) {
            stream<<else_label<<":"<<std::endl;
            else_statement_->EmitRISCWithDest(stream, context, empty_reg);
            context.FreeRegister(empty_reg);
        }

        stream<<endif_label<<":"<<std::endl;
    };

    void Print(std::ostream &stream) const
    {
        std::cout << "\nif (";
        cond_expression_->Print(stream);
        std::cout << ")";
        std::cout << std::endl<< "{ ";
        if_statement_->Print(stream);
        std::cout <<std::endl<<"} ";
        if (else_statement_ != nullptr) {
            std::cout << "else { ";
            else_statement_->Print(stream);
            std::cout << std::endl<<"}"<<std::endl;
        }
    };
};

#endif
