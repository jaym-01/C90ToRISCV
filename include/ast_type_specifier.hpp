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

    // std::string GetIdentifier() const override { return type_; };
    std::string GetTypeSpecifier() const override { return type_; };

    void EmitRISC(std::ostream &stream, Context &context) const override {
        std::cout<<"Emitting risc for type_spec, "<<type_<<std::endl;
    };

    std::string GetType(Context &context) const override {
        return type_;
    }

    void Print(std::ostream &stream) const override {
        std::cout<<"type_spec{"<<type_<<"}";
    };

    bool IsPointer() const override {
        return false;
    }
};

#endif
