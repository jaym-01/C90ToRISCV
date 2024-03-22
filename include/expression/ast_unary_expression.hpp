#ifndef AST_UNARY_EXPRESSION_HPP
#define AST_UNARY_EXPRESSION_HPP

#include "../ast_node.hpp"
#include "helpers/var_helpers.hpp"
#include "helpers/helpers.hpp"
#include "helpers/pointer_helpers.hpp"
#include "../ast_constant.hpp"
#include <string>

class UnaryExpression : public Node
{
private:
    std::string unary_operator_;
    Node *expression_;
    std::string type_;

public:
    UnaryExpression(
        std::string unary_operator,
        Node *expression ) : unary_operator_(unary_operator), expression_(expression){};

    ~UnaryExpression()
    {
        delete expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const {};
    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const {
        std::string id = expression_->GetIdentifier();
        VariableContext var;
        std::string type;
        ScopeContext* cur_scope = nullptr;
        std::string tmp = "";

        // if identifer exists
        // use type from below
        // to correctly evaluate unsigned types
        // TODO: fix finding the type
        // will give incorrect results for float + double
        if(unary_operator_ == "&") type = "int";
        else if(id != ""){
            cur_scope = context.GetCurScope();
            if(cur_scope->var_map.find(id) == cur_scope->var_map.end()) type = context.id_to_func_def[id].return_type;
            else {
                var = cur_scope->GetVarFromId(id);
                // type = var.GetType();
                type = GetType(context);
            }
        }
        // if no identifier - then value is a constant
        // use type passed from above
        else{
            type = GetType(context);
        }


        // 1. Evaluate
        if (dest_reg == ""){
            dest_reg = context.ReserveRegister(type);
        }

        // special case - recieving floating point register for a pointer type
        // must only write on the final expression = this one
        if(dest_reg.size() > 0 && dest_reg[0] == 'f'){
            tmp = dest_reg;
            dest_reg = context.ReserveRegister("int");
        }

        // clean up from previous use
        if(unary_operator_ == "*" && cur_scope != nullptr && var.is_pntr && var.working_pntr_depth < var.pntr_depth){
            var.working_pntr_depth = var.pntr_depth;
        }

        if(unary_operator_ != "&") {
            // if dereferencing then it must recieve an int
            expression_->DefineConstantType(unary_operator_ == "*"? "int": type);
            expression_->EmitRISCWithDest(stream, context, dest_reg);
        }

        if(unary_operator_ == "*" && cur_scope != nullptr && var.is_pntr) {
            var = cur_scope->var_map[id];
        }
        // 2. Apply unary operator
        // Note: do nothing for '+' operator
        if (unary_operator_ == "++") {
            stream << "addi " << dest_reg << ", " << dest_reg << ", " << one_pointer_offset(context, expression_) << std::endl;

        } else if (unary_operator_ == "--") {
            stream << "addi " << dest_reg << ", " << dest_reg << ", -" << one_pointer_offset(context, expression_) << std::endl;

        } else if (unary_operator_ == "-") {
            // negate instruction, pseudo-ins for sub dest_reg, x0, dest_reg
            stream << "neg " << dest_reg << ", " << dest_reg << std::endl;

        } else if (unary_operator_ == "~") {
            std::string tmp_reg = context.ReserveRegister(type);
            stream << "li " << tmp_reg << ", -1" << std::endl; // Load 0xFFFFFFFF into temp_reg
            stream << "xor " << dest_reg << ", " << dest_reg <<", "<<tmp_reg << std::endl;

        } else if (unary_operator_ == "!") {
            stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;

        } else if(unary_operator_ == "*") {
            // TODO: is this condition right?
            stream << get_mem_read(var.is_pntr? var.type: type, var.is_pntr? !(var.working_pntr_depth == 0): false) << " " << (tmp.size() > 0 && tmp[0] == 'f'? tmp : dest_reg) << ",0(" << dest_reg << ")" << std::endl;

            if(unary_operator_ == "*" && cur_scope != nullptr && var.is_pntr){
                --var.working_pntr_depth;
                cur_scope->var_map[id] = var;
            }

        } else if(unary_operator_ == "&"){
            // get var information
            std::string var_name = expression_->GetIdentifier();
            ScopeContext* cur_scope = context.GetCurScope();
            VariableContext var = cur_scope->GetVarFromId(var_name);

            // arrays always point to the first element
            // so this will always load a pointer to the first element in the array
            if(var.is_global) {
                stream << "lui " << dest_reg << ", %hi(" << var_name << ")" << std::endl;
                stream << "addi " << dest_reg << ", " << dest_reg << ", %lo(" << var_name << ")" << std::endl;
            }
            else stream << "addi " << dest_reg << ", fp, " << var.offset << std::endl;

            // getting the address of a specific index
            if(var.is_array){
                Node *index_expr = expression_->GetIndexExpression();
                if(index_expr != nullptr){
                    std::string index_reg = context.ReserveRegister("int");
                    index_expr->EmitRISCWithDest(stream, context, index_reg);
                    stream << "slli " << index_reg << ", " << index_reg << ", 2" << std::endl;
                    stream << "add " << dest_reg << ", " << dest_reg << ", " << index_reg << std::endl;
                    context.FreeRegister(index_reg);
                }
            }
        }

        if(tmp.size() > 0 && tmp[0] == 'f'){
            context.FreeRegister(dest_reg);
            dest_reg = tmp;
        }

        // Store result back to var if INC / DEC op
        if (id != "" && (unary_operator_ == "++" || unary_operator_ == "--")) {

            write_var_value(expression_, context, stream, var, dest_reg);
        }
    };

    void DefineConstantType(std::string type) override {
        type_ = type;
        expression_->DefineConstantType(type);
    };

    std::string GetType(Context &context) const override{
        return expression_->GetType(context);
    }

    std::string GetIdentifier() const override {
        return expression_->GetIdentifier();
    }

    bool IsDereference() const override {
        if(unary_operator_ == "*") return true;
        else return false;
    }


    void Print(std::ostream &stream) const
    {
        stream<<"u_expr{ ";
        stream << unary_operator_;
        expression_->Print(stream);
        stream<<" }";
    };

    std::vector<int> EvalExpression(std::string type) const override{
        if(unary_operator_ == "&") return {0};
        IntConstant *zero_c = new IntConstant(0);
        zero_c->DefineConstantType(type);
        std::vector<int> left = zero_c->EvalExpression(type), right = expression_->EvalExpression(type);

        if(type == "int") return CalcVal<int>(left, right, unary_operator_);
        else if(type == "unsigned") return CalcVal<unsigned>(left, right, unary_operator_);
        else if(type == "float") return CalcVal<float>(left, right, unary_operator_);
        else if(type == "double") return CalcVal<double>(left, right, unary_operator_);
        else if(type == "char") return {(int)((signed char)(left[0] + right[0]))};
        else throw std::runtime_error("An invalid type is trying to be evaluated");
    }

    bool IsMemoryReference(Context &context) const override {
        if(unary_operator_ == "&") return true;
        else if (unary_operator_ != "*") return expression_->IsMemoryReference(context);
        else return false;
    }
    std::string DFSIdentifier() const override {
        return expression_->DFSIdentifier();
    }
};

#endif
