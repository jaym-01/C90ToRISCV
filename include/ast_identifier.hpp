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
        std::cout<<"Emitting RISC for identifier"<<std::endl;
    };

    void Print(std::ostream &stream) const override;
};

#endif
