#ifndef AST_CONSTANT_HPP
#define AST_CONSTANT_HPP

#include "ast_node.hpp"
#include<cstring>

class IntConstant : public Node
{
private:
    int value_;

public:
    IntConstant(int value) : value_(value) {}

    void EmitRISC(std::ostream &stream, Context &context) const override {
        stream << "li a0, " << value_ << std::endl;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const override {
        if (dest_reg == "") {
            dest_reg = context.ReserveRegister("int");
        }

        stream << "li " << dest_reg << ", " << value_ << std::endl;
    };

    void DefineConstantType(std::string type) override {}

    void Print(std::ostream &stream) const override {
        stream << "iconst{";
        stream << value_;
        stream << "}";
    };

    int EvalExpression(std::string type) const override {
        return value_;
    };

    int GetNumBranches() const override {
        return 1;
    };
};

// need to be combined
// in lexer -> can't tell if number is float or double
class FloatDoubleConstant : public Node {
private:
    double value_;
    std::string type_;
public:
    FloatDoubleConstant(double value): value_(value) {}

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const override {
        if(type_ == "") throw std::runtime_error("Error: Float and Double constant - type not specified");

        if(dest_reg == "") dest_reg = context.ReserveRegister(type_);
        std::string label = context.GetNewLabel("LC");

        FunctionContext *f_context = context.f_context;

        // defines the value outside of the function
        if(f_context != nullptr){
            f_context->extern_declns += "\n" + label + ":\n";
            if(type_ == "float") f_context->extern_declns += ".word " + std::to_string(GetFloatVal()) + "\n\n";
            else f_context->extern_declns += ".word " + std::to_string(GetDoubleVal(0)) + "\n" + ".word " + std::to_string(GetDoubleVal(1)) + "\n\n";
        }

        // get a temp register
        std::string tmp_reg = context.ReserveRegister("int");

        // load the value defined into the floating point register
        stream << "lui " << tmp_reg << "," << "%hi(" << label << ")" << std::endl;
        stream << "flw " << dest_reg << ",%lo("<< label << ")(" << tmp_reg << ")" << std::endl;

        context.FreeRegister(tmp_reg);
    }

    void DefineConstantType(std::string type) override {
        type_ = type;
    }

    int EvalExpression(std::string type) const override {
        if(type == "float") return GetFloatVal();
        // TODO: add double condition
    }

    int GetFloatVal() const {
        float tmp = (float)value_;
        int dec_rep;

        std::memcpy(&dec_rep, &tmp, sizeof(float));
        return dec_rep;
    }

    int GetDoubleVal(int part) const {
        int dec_rep[2];
        std::memcpy(&dec_rep, &value_, 2*sizeof(int));
        return dec_rep[part];
    }

    void Print(std::ostream &stream) const{
        stream << "fdconst{" << value_ << "}";
    };
};

#endif
