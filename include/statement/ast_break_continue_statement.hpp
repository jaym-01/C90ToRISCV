#ifndef AST_BREAK_CONTINUE_STATEMENT_HPP
#define AST_BREAK_CONTINUE_STATEMENT_HPP

#include "ast_node.hpp"

class BreakContinueStatement : public Node
{
private:
    std::string jump_statement_;

public:
    BreakContinueStatement(std::string jump_statement) : jump_statement_(jump_statement) {}
    ~BreakContinueStatement(){};

    void EmitRISC(std::ostream &stream, Context &context) const override{};

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override
    {
        if (jump_statement_ == "break") {
            if (context.cur_breakloop_label == "")
                throw std::runtime_error("break statement not in loop");
            stream << "j " << context.cur_breakloop_label<< std::endl;
        }
        else if (jump_statement_ == "continue"){
            if (context.cur_contloop_label == "")
                throw std::runtime_error("continue statement not in loop");

            stream << "j " << context.cur_contloop_label << std::endl;
        }
    };

    void Print(std::ostream &stream) const override
    {
        stream << "\n" << jump_statement_;
    };
};

#endif
