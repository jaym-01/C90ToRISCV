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

    void Print(std::ostream &stream) const override {
        stream << "id{" << identifier_ << "}";
    };

    std::string GetIdentifier() const override {
        return identifier_;
    }

};

#endif
