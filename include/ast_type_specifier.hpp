#ifndef AST_TYPE_SPECIFIER
#define AST_TYPE_SPECIFIER

#include "ast_node.hpp"

class TypeSpecifier : public Node
{
private:
    std::string type_;

public:
    TypeSpecifier(std::string type) : type_(type){};
    ~TypeSpecifier(){};
    void EmitRISC(std::ostream &stream, Context &context) const override {
        std::cout<<"Emitting risc for type_spec, "<<type_<<std::endl;
    };

    void Print(std::ostream &stream) const {
        std::cout<<"type_spec{"<<type_<<"}";
    };
};

#endif
