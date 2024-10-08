#ifndef AST_BINARY_EXPRESSION_HPP
#define AST_BINARY_EXPRESSION_HPP

#include "../ast_node.hpp"
#include "helpers/helpers.hpp"
#include "../helpers/pointer_helpers.hpp"
#include "../helpers/var_helpers.hpp"
#include <string>
#include <vector>

class BinaryExpression : public Node
{
public:
    Node *left_operand_;
    std::string b_operator_;
    Node *right_operand_;
    std::string result_type_;


    BinaryExpression(
        Node *left_operand,
        std::string b_operator,
        Node *right_operand) : left_operand_(left_operand),
                               b_operator_(b_operator),
                               right_operand_(right_operand){};

    ~BinaryExpression()
    {
        if(left_operand_ != nullptr) delete left_operand_;
        if(right_operand_ != nullptr) delete right_operand_;
    };

    // void EmitRISC(std::ostream &stream, Context &context) const {

    // };

    void EvaluateOperation(std::ostream &stream, Context &context, std::string reg1, std::string reg2, std::string dest_reg) const {
        std::string ins_prefix = "", ins_postfix = " ", comp_postfix = " ";

        bool float_double = result_type_ == "double" || result_type_ == "float"  || GetChildrenType(context) == "float" || GetChildrenType(context) == "double";

        if(float_double){
            ins_prefix = "f";
            ins_postfix = result_type_ == "double" || GetChildrenType(context) == "double" ? ".d " : ".s ";
        }

        if(GetChildrenType(context) == "unsigned"){
            comp_postfix = "u ";
        }

        if (b_operator_ == "+") {
            stream << ins_prefix << "add" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "-"){
            stream << ins_prefix << "sub" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        } else if (b_operator_ == "*") {
            // std::cout << ins_prefix << "mul" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            stream << ins_prefix << "mul" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "/"){
            stream << ins_prefix << "div" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "%"){
            stream << ins_prefix << "rem" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
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
            if(float_double) stream << "flt" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            else stream << "slt" << comp_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == "<=") {
            if(float_double) stream << "fle" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            else {
                // Same as !(reg1 > reg2)
                stream << "sgt" << comp_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
                stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
            }
        }
        else if (b_operator_ == ">") {
            if(float_double) stream << "fgt" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            else stream << "sgt" << comp_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
        }
        else if (b_operator_ == ">=") {
            // Same as !(reg1 < reg2)
            if(float_double) stream << "fge" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            else {
                stream << "slt" << comp_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
                stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
            }
        }
        else if (b_operator_ == "==") {
            if(float_double){
                // this will write 1 into reg when equal
                stream << "feq" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            } else {
                stream << "sub " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
                stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
            }
        }
        else if (b_operator_ == "!=") {
            if(float_double){
                // this will write 1 into reg when equal
                stream << "feq" << ins_postfix << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
                stream << "seqz " << dest_reg << ", " << dest_reg << std::endl;
            } else {
                stream << "sub " << dest_reg << ", " << reg1 << ", " << reg2 << std::endl;
            }
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

        std::cout << result_type_ << std::endl;
        // Define type of children
        left_operand_->DefineConstantType(result_type_);
        right_operand_->DefineConstantType(result_type_);

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


        ScopeContext *scope = context.cur_scope;
        // check if left is pointer
        // if it is -> shift right val
        std::string l_id = left_operand_->GetIdentifier(), r_id = right_operand_->GetIdentifier();
        if(l_id != "" && scope->var_map.find(l_id) != scope->var_map.end() && scope->GetVarFromId(l_id).is_pntr){
            stream << "slli " << reg2 << ", " << reg2 << ", " << type_to_shift_amt[scope->var_map[l_id].type] << std::endl;
        } else if(r_id != "" && scope->var_map.find(r_id) != scope->var_map.end() && scope->GetVarFromId(r_id).is_pntr){
            stream << "slli " << reg1 << ", " << reg1 << ", " << type_to_shift_amt[scope->var_map[l_id].type] << std::endl;
        }

        // std::cout << "type: " << result_type_ << std::endl;
        if (dest_reg == "") {
            dest_reg = context.ReserveRegister(result_type_);
        }

        EvaluateOperation(stream, context, reg1, reg2, dest_reg);

        // std::cout<<"Operand 1 stored in reg: "<<reg1<<std::endl;
        // std::cout<<"Operand 2 stored in reg: "<<reg2<<std::endl;

        context.FreeRegister(reg1);
        context.FreeRegister(reg2);

        // std::cout<<"Binary expression result in reg: "<<dest_reg<<std::endl;
    };

    std::vector<int> EvalExpression(std::string type) const override {
        std::vector<int> left = left_operand_->EvalExpression(type);
        std::vector<int> right = right_operand_->EvalExpression(type);

        if(left.size() == 0) return right;
        if(right.size() == 0) return left;

        // if (b_operator_ == "+") {
        //     return left + right;
        // } else if (b_operator_ == "-") {
        //     return left - right;
        // } else if (b_operator_ == "*") {
        //     return left * right;
        // } else if (b_operator_ == "/") {
        //     return left / right;
        // } else {
        //     throw std::runtime_error("Error: Invalid binary operator");
        // }

        if(type == "int") return CalcVal<int>(left, right, b_operator_);
        else if(type == "unsigned") return CalcVal<unsigned>(left, right, b_operator_);
        else if(type == "float") return CalcVal<float>(left, right, b_operator_);
        else if(type == "double") return CalcVal<double>(left, right, b_operator_);
        else if(type == "char") return {(int)((signed char)(left[0] + right[0]))};
        else throw std::runtime_error("An invalid type is trying to be evaluated");
    }

    void DefineConstantType(std::string type) override {
        // std::cout << "this is what is passed through : " << type << std::endl;
        result_type_ = type;
        // left_operand_->DefineConstantType(type);
        // right_operand_->DefineConstantType(type);
    }

    std::string GetChildrenType(Context &context) const {
        std::string left = left_operand_->GetType(context), right = right_operand_->GetType(context);
        // only unsigned takes precedence
        // normally there shouldn't be mixing of type
        // this might happen with constant int and unsigned var
        // treat expression as unsigned (i.e. use unsigned operations)
        if(left == "unsigned" || right == "unsigned") return "unsigned";
        // give char precedence over other types
        else if(left == "char" || right == "char") return "char";
        else return left;
    }

    std::string GetType(Context &context) const override{
        // get type of children so you always get the type of the variable or constant used
        return result_type_;
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

    bool IsMemoryReference(Context &context) const override {
        return left_operand_->IsMemoryReference(context) | right_operand_->IsMemoryReference(context);
    }

    std::string DFSIdentifier() const override {
        std::string l = left_operand_->DFSIdentifier(), r = right_operand_->DFSIdentifier();
        if(l != "" && r != "") throw std::runtime_error("Error: Cannot have two identifiers in binary expression");
        else if(l != "") return l;
        else return r;
    }
};

#endif
