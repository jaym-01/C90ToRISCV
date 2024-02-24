#ifndef AST_IDENTIFIER_HPP
#define AST_IDENTIFIER_HPP

#include "ast_node.hpp"

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

        if (var.offset > 0) {
            std::string err_msg = "Variable " + identifier_ + " was not initialized";
            throw std::runtime_error(err_msg);
        }

        load_var_to_reg(stream, var.type, var.offset, dest_reg);
        // stream << "lw " << dest_reg << ", " << var.offset << "(fp)" << std::endl;
    };

    int GetNumBranches() const override{
        return 1;
    };

    void Print(std::ostream &stream) const override {
        stream << "id{" << identifier_ << "}";
    };

    std::string GetIdentifier() const override {
        return identifier_;
    }

};

#endif
