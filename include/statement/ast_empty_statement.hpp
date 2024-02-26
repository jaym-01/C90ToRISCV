#ifndef AST_EMPTY_STATEMENT_HPP
#define AST_EMPTY_STATEMENT_HPP

#include "ast_node.hpp"

// NOTE: Create a class for each type of direct declarator?
class EmptyStatement : public Node
{
private:


public:
    EmptyStatement(){};

    ~EmptyStatement()
    {

    };
    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override {
    };

    void Print(std::ostream &stream) const
    {
        stream<<"empt_stmt"<<std::endl;
    };
};

#endif
