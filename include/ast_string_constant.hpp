#ifndef AST_STRING_CONSTANT
#define AST_STRING_CONSTANT

#include "ast_node.hpp"
#include "ast_node_list.hpp"
#include "ast_constant.hpp"
#include <sstream>

class StringConstant : public NodeList {
private:
    std::string value_;

public:
    StringConstant(std::string value) : value_(value), NodeList(new IntConstant(value.size() > 0? (int)value[0]: 0)) {
        for(int i = 1; i < value.size(); i++) {
            // std::cout << "here: " << (int)value[i] << std::endl;
            PushBack(new IntConstant((int)value[i]));
        }
        if(value.size() > 0) PushBack(new IntConstant(0));
    }

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {
        std::string label = context.GetNewLabel("string");

        std::stringstream label_data;
        label_data << ".data" << std::endl;
        label_data << label << ":" << std::endl;
        label_data << ".string \"" << value_ << "\"" << std::endl << std::endl;
        context.f_context->extern_declns += label_data.str();

        if(dest_reg == "") {
            dest_reg = context.ReserveRegister("int");
        }

        stream << "lui " << dest_reg << ", %hi(" << label << ")" << std::endl;
        stream << "addi " << dest_reg << ", " << dest_reg << ", %lo(" << label << ")" << std::endl;
    }

    bool IsString() const override {
        return true;
    }

    // std::string EmitRISCString(std::ostream &stream, Context &context) const override{
    //     std::string label = context.GetNewLabel("string");

    //     stream << label << ":" << std::endl;
    //     stream << ".string \"" << value_ << "\"" << std::endl << std::endl;

    //     return label;
    // }

    void Print(std::ostream &stream) const override {
        stream << "sconst{\"" << value_ << "\\0\"}";
    }
};

#endif
