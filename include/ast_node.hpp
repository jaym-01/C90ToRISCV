#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <iostream>
#include <vector>

#include "ast_context.hpp"

class Node
{
protected:
    std::vector<Node *> branches_;

public:
    Node(){};
    virtual ~Node();
    virtual void EmitRISC(std::ostream &stream, Context &context) const = 0;

    virtual void Print(std::ostream &stream) const = 0;

    // New
    virtual std::string GetIdentifier() const {return "";};
};

#endif
// Represents a list of nodes.
