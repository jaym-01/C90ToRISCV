#ifndef AST_CONSTANT_HPP
#define AST_CONSTANT_HPP

#include "ast_node.hpp"
#include<cstring>
#include "helpers/memory_helpers.hpp"


// need to be combined
// in lexer -> can't tell if number is float or double
class FloatDoubleConstant : public Node {
private:
    double value_;
    std::string type_;
public:
    FloatDoubleConstant(double value): value_(value) {}
    FloatDoubleConstant(double value, std::string type): value_(value), type_(type) {}

    // this might be dangerous if value is stored in fa0
    void EmitRISC(std::ostream &stream, Context &context) const override{
        std::string dest_reg = "fa0";
        EmitRISCWithDest(stream, context, dest_reg);
    }

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const override {
        if(type_ == "") throw std::runtime_error("Error: Float and Double constant - type not specified");

        if(dest_reg == "") dest_reg = context.ReserveRegister(type_);
        std::string label = context.GetNewLabel("LC");

        FunctionContext *f_context = context.f_context;

        // defines the value outside of the function
        if(f_context != nullptr){
            f_context->extern_declns += "\n" + label + ":\n";
            if(type_ == "float") f_context->extern_declns += ".word " + std::to_string(GetFloatVal()) + "\n\n";
            else {
                std::vector<int> words = GetDoubleVal();
                for(int word : words) f_context->extern_declns += ".word " + std::to_string(word) + "\n";
                f_context->extern_declns += "\n";
            }
        }

        // get a temp register
        std::string tmp_reg = context.ReserveRegister("int");

        // load the value defined outside into the floating point register
        stream << "lui " << tmp_reg << "," << "%hi(" << label << ")" << std::endl;
        stream << get_mem_read(type_, false) << " " << dest_reg << ",%lo("<< label << ")(" << tmp_reg << ")" << std::endl;

        context.FreeRegister(tmp_reg);
    }

    void DefineConstantType(std::string type) override {
        type_ = type;
    }

    std::vector<int> EvalExpression(std::string type) const override {
        if(type == "float") return {GetFloatVal()};
        else return GetDoubleVal();
    }

    int GetFloatVal() const {
        float tmp = (float)value_;
        int dec_rep;

        std::memcpy(&dec_rep, &tmp, sizeof(float));
        return dec_rep;
    }

    std::vector<int> GetDoubleVal() const {
        int dec_rep[2];
        std::memcpy(&dec_rep, &value_, 2*sizeof(int));
        return {dec_rep[0], dec_rep[1]};
    }

    std::string GetType() const override{
        return type_;
    }

    void Print(std::ostream &stream) const{
        stream << "fdconst{" << value_ << "}";
    };

    std::string GetIdentifier() const override {
        return "";
    }
};


class IntConstant : public Node
{
private:
    int value_;
    FloatDoubleConstant *fd_val_ = nullptr;

public:
    IntConstant(int value) : value_(value) {}

    ~IntConstant(){
        if(fd_val_ != nullptr) delete fd_val_;
    }

    // this might be dangerous if value is stored in fa0 or a0
    void EmitRISC(std::ostream &stream, Context &context) const override {
        if(fd_val_ != nullptr) fd_val_->EmitRISC(stream, context);
        else stream << "li a0, " << value_ << std::endl;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const override {
        if(fd_val_ != nullptr) fd_val_->EmitRISCWithDest(stream, context, dest_reg);
        else{
            if (dest_reg == "") {
                dest_reg = context.ReserveRegister("int");
            }

            stream << "li " << dest_reg << ", " << value_ << std::endl;
        }
    };

    void DefineConstantType(std::string type) override {
        if(type == "float" || type == "double"){
            fd_val_ = new FloatDoubleConstant((double)value_, type);
        }
    }

    void Print(std::ostream &stream) const override {
        stream << "iconst{";
        stream << value_;
        stream << "}";
    };

    std::vector<int> EvalExpression(std::string type) const override {
        if(fd_val_ != nullptr) return fd_val_->EvalExpression(type);
        else return {value_};
    };

    int GetNumBranches() const override {
        return 1;
    };

    std::string GetType() const override{
        if(fd_val_ != nullptr) return fd_val_->GetType();
        else return "int";
    }

    std::string GetIdentifier() const override {
        return "";
    }
};

#endif
