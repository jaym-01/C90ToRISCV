#ifndef AST_ROOT_SPECIFIER
#define AST_ROOT_SPECIFIER

#include "ast_node.hpp"
#include "ast_node_list.hpp"

class ASTRoot : public NodeList
{
private:

public:
    ASTRoot(NodeList *nodes) : NodeList(nodes){};
    // ~ASTRoot(){};

    void EmitRISC(std::ostream &stream, Context &context) const override {

    };

    // void Print(std::ostream &stream) const override
    // {
    //     std::cout << "type_spec{" << type_.substr(0, 3) << "}";
    // };
};

#endif
