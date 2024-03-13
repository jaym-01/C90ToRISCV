#ifndef ATS_SWITCH_CASE_STATEMENT
#define ATS_SWITCH_CASE_STATEMENT

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"
#include <sstream>

class SwitchStatement : public Node {
private:
    Node *expression_;
    Node *statement_;
public:
    SwitchStatement(Node *expression, Node *statement) : expression_(expression), statement_(statement){}

    ~SwitchStatement(){
        delete expression_;
        delete statement_;
    }

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {
        // TODO: check if type needs to be changed, can it stay int as always an integral type?
        if(dest_reg == "") dest_reg = context.ReserveRegister("int");
        std::string end_switch = context.GetNewLabel("end_switch");

        // std::stringstream switch_expr;
        std::string tmp_brk_lbl = context.cur_breakloop_label;
        context.cur_breakloop_label = end_switch;

        // get the switch expression
        // store result of the expression in dest_reg
        expression_->EmitRISCWithDest(stream, context, dest_reg);
        // pass down register to each case
        statement_->PassRegister(dest_reg);
        // write all the branch statements
        statement_->EmitCases(stream, context);


        if(context.f_context->default_case == "") stream << "j " << end_switch << std::endl;
        else stream << context.f_context->default_case;
        context.f_context->default_case = "";
        context.FreeRegister(dest_reg);

        // emit case staments
        std::string empty_reg = "";
        statement_->EmitRISCWithDest(stream, context, empty_reg);
        context.FreeRegister(empty_reg);

        stream << end_switch << ":" << std::endl;
        context.FreeRegister(dest_reg);

        context.cur_breakloop_label = tmp_brk_lbl;
    }

    void Print(std::ostream &stream) const override{
        stream << "switch( ";
        expression_->Print(stream);
        stream << " ) { " << std::endl;
        statement_->Print(stream);
        stream << "}" << std::endl;
    }
};

class CaseStatement : public Node{
private:
    Node *expression_;
    Node *statement_;
    std::string label_;
    std::string switch_expr_reg_;

public:
    CaseStatement(Node *expression, Node *statement) : expression_(expression), statement_(statement) {}

    ~CaseStatement(){
        if(expression_ != nullptr) delete expression_;
        delete statement_;
    }

    // void EmitRISC(std::ostream &stream, Context &context) const override {
    //     // emits what happens on each case
    //     if(label_ == "") throw std::runtime_error("Case label not implemented");

    //     stream << label_ << ":" << std::endl;
    //     std::string empty_reg = "";
    //     statement_->EmitRISCWithDest(stream, context, empty_reg);
    //     context.FreeRegister(empty_reg);
    // }

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {
        // emits what happens on each case
        if(label_ == "") throw std::runtime_error("Case label not implemented");

        stream << label_ << ":" << std::endl;
        std::string empty_reg = "";
        statement_->EmitRISCWithDest(stream, context, empty_reg);
        context.FreeRegister(empty_reg);
    }

    void PassRegister(std::string &reg) override {
        switch_expr_reg_ = reg;
    }

    void EmitCases(std::ostream &stream, Context &context) override {
        // emits the case statement
        std::stringstream case_branch = BranchStatement(context);
        if(expression_ == nullptr) context.f_context->default_case = case_branch.str();
        else stream << case_branch.str();
    }

    std::stringstream BranchStatement(Context &context){
        if(switch_expr_reg_ == "") throw std::runtime_error("Switch expression register not passed down to case statement");
        std::stringstream case_branch;

        label_ = context.GetNewLabel("case");
        // TODO: Check if type is right

        if(expression_ != nullptr) {
            std::string tmp_reg = context.ReserveRegister("int");
            expression_->EmitRISCWithDest(case_branch, context, tmp_reg);
            case_branch << "beq " << switch_expr_reg_ << ", " << tmp_reg << ", " << label_ << std::endl;
            context.FreeRegister(tmp_reg);
        }
        else case_branch << "j " << label_ << std::endl;

        return case_branch;
    }

    void Print(std::ostream &stream) const override{
        stream << "case( ";
        if(expression_ != nullptr) expression_->Print(stream);
        else stream << "default";
        stream << " ) { " << std::endl;
        statement_->Print(stream);
        stream << "}" << std::endl;
    }
};
#endif
