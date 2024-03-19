#ifndef AST_SIZE_OF
#define AST_SIZE_OF

#include "../ast_node.hpp"
#include "../helpers/helpers.hpp"

class SizeOfExpression : public Node {
private:
    Node *type_name_;
    Node *expr_;
    std::string type_ = "int";

public:
    SizeOfExpression(Node *type_name, Node* expr) : type_name_(type_name), expr_(expr) {};

    ~SizeOfExpression() {
        delete type_name_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {
        if(expr_ == nullptr){
            if(dest_reg == "") dest_reg = context.ReserveRegister("int");

            std::string type = type_name_->GetType(context);
            stream << "li " << dest_reg << ", " << type_size[type] << std::endl;
        } else {
            std::string id = expr_->GetIdentifier();
            std::cout << id << std::endl;
            ScopeContext *scope = context.GetCurScope();
            VariableContext var = scope->GetVarFromId(id);

            int size = type_size[var.type] * var.array_size;
            stream << "li " << dest_reg << ", " << size << std::endl;
        }
    }

    void Print(std::ostream &stream) const override {
        stream << "sizeof(";
        if(expr_ == nullptr) type_name_->Print(stream);
        else expr_->Print(stream);
        stream << ")" << std::endl;
    }

    void DefineConstantType(std::string type) override {
        type_ = type;
    }
};


#endif
