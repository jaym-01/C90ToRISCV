#ifndef AST_FOR_STATEMENT_HPP
#define AST_FOR_STATEMENT_HPP

#include "ast_node.hpp"

// NOTE: Create a class for each type of direct declarator?
class ForStatement : public Node
{
private:
    Node* initial_expression_;
    Node* cond_expression_;
    Node* postloop_expression_;
    Node* statement_;

public:
    ForStatement(Node *initial, Node *cond, Node* postloop, Node* statement) :
        initial_expression_(initial), cond_expression_(cond), postloop_expression_(postloop), statement_(statement) {};

    ~ForStatement()
    {
        delete initial_expression_;
        delete cond_expression_;
        if (postloop_expression_ != nullptr) delete postloop_expression_;
        delete statement_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override
    {

        ScopeContext *scope = context.GetCurScope();
        std::string startfor_label = context.GetNewLabel("endfor_label");
        std::string endfor_label = context.GetNewLabel("startfor_label");

        std::string contfor_label = context.GetNewLabel("contfor_label");

        std::string prev_contloop_label = context.cur_contloop_label;
        std::string prev_breakloop_label = context.cur_breakloop_label;
        context.SetBreakContLabels(endfor_label, contfor_label);

        std::string temp_reg = "";

        // 1. Perform initial expression
        initial_expression_->EmitRISCWithDest(stream, context, dest);
        context.FreeRegister(dest);

        stream<< startfor_label << ":" << std::endl;


        cond_expression_->EmitRISCWithDest(stream, context, temp_reg);
        stream << "beq " << temp_reg << ", zero, " << endfor_label << std::endl;
        context.FreeRegister(temp_reg);

        // 2. Emit if statement
        statement_->EmitRISCWithDest(stream, context, temp_reg);
        context.FreeRegister(temp_reg);

        // 3. Perform postloop expression
        stream<< contfor_label << ":" << std::endl;
        if (postloop_expression_ != nullptr)
        {
            postloop_expression_->EmitRISCWithDest(stream, context, temp_reg);
            context.FreeRegister(temp_reg);
        }
        stream << "j " << startfor_label << std::endl;

        stream << endfor_label << ":" << std::endl;

        context.SetBreakContLabels(prev_breakloop_label, prev_contloop_label);
    };

    void Print(std::ostream &stream) const
    {
        std::cout<<"\nfor (";
        initial_expression_->Print(stream);
        std::cout<<"; ";
        cond_expression_->Print(stream);
        std::cout<<"; ";

        if (postloop_expression_ != nullptr) {
            postloop_expression_->Print(stream);
        }

        std::cout<<") ";
        std::cout<<"{";
        std::cout<<"\n";
        statement_->Print(stream);
        std::cout<<"\n}";
    };
};

#endif
