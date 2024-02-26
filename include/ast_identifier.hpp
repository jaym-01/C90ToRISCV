#ifndef AST_IDENTIFIER_HPP
#define AST_IDENTIFIER_HPP

#include "ast_node.hpp"
#include "helpers/var_helpers.hpp"
class Identifier : public Node
{
private:
    std::string identifier_;

public:
    Identifier(std::string identifier) : identifier_(identifier){};
    ~Identifier(){};

    void EmitRISC(std::ostream &stream, Context &context) const override {
        stream << identifier_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {

        if (dest_reg == "") {
            dest_reg = context.ReserveTempRegister();
        }

        ScopeContext* cur_scope = context.GetCurScope();
        VariableContext var = cur_scope->GetVarFromId(identifier_);


        // if (var.offset > 0) {
        //     std::string err_msg = "Variable " + identifier_ + " was not initialized";
        //     throw std::runtime_error(err_msg);
        // }

        read_var_value(nullptr, context, stream, var, dest_reg);
    };

    int GetNumBranches() const override{
        return 1;
    };

    VariableContext InitVariableContext(std::string type) override {
        return {
            .id=identifier_,
            .type=type,
            .is_array=false,
            .array_size=1,
        };
    };

    void Print(std::ostream &stream) const override {
        stream << "id{" << identifier_ << "}";
    };

    int EvalIntExpression() const override {
        throw std::runtime_error("Cannot evaluate identifier as int expression");
    };

    std::string GetIdentifier() const override {
        return identifier_;
    }

    DeclaratorType GetDeclaratorType() const override {
        return DeclaratorType::Variable;
    }

};

#endif
