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
        if(type_name_ != nullptr) delete type_name_;
        if(expr_ != nullptr) delete expr_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {
        if(dest_reg == "") dest_reg = context.ReserveRegister("int");

        if(expr_ == nullptr){
            std::string type = type_name_->GetType(context);

            // for type def types
            if(type_size.find(type) == type_size.end()){
                std::string id = type_name_->GetType(context);
                ScopeContext *scope = context.GetCurScope();
                TypeDefContext var = scope->GetTypeDef(id);
                TypeDefContext tmp = resolve_type(type, scope);
                type = tmp.type;
            }
            stream << "li " << dest_reg << ", " << type_size[type] << std::endl;
        } else {
            std::string id = expr_->GetIdentifier();
            std::cout << id << std::endl;
            ScopeContext *scope = context.GetCurScope();
            VariableContext var = scope->GetVarFromId(id);

            if(expr_->IsDereference()){
                // check whether to load a pointer or the actual value
                // first adjust the working pntr depth
                std::string tmp;
                std::stringstream s;
                expr_->EmitRISCWithDest(s, context, tmp);
                context.FreeRegister(tmp);

                var = scope->GetVarFromId(id);

                if(var.working_pntr_depth == 0){
                    var.is_pntr = false;
                    stream << "li " << dest_reg << ", " << var.GetSize() << std::endl;
                }
            }

            // int size = type_size[var.GetType()] * var.array_size;
            stream << "li " << dest_reg << ", " << var.GetSize() << std::endl;
        }
    }

    void Print(std::ostream &stream) const override {
        stream << "sizeof(";
        if(expr_ == nullptr) type_name_->Print(stream);
        else expr_->Print(stream);
        stream << ")" << std::endl;
    }

    void DefineConstantType(std::string type) override {
        return;
    }
};


#endif
