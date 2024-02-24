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

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string dest_reg) const override {
        ScopeContext* cur_scope = context.GetCurScope();
        int var_offset = cur_scope->GetVarOffset(identifier_);

        // Load word into register first?
        // std::string tmp_reg = context.ReserveTempRegister();
        // stream << "sw " << tmp_reg << ", " << var_offset << std::endl;

        stream << "lw " << dest_reg << ", " << var_offset << "(fp)" << std::endl;
    };

    void Print(std::ostream &stream) const override {
        stream << "id{" << identifier_ << "}";
    };

    std::string GetIdentifier() const override {
        return identifier_;
    }

};

#endif
