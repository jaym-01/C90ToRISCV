#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <iostream>
#include <vector>
#include <sstream>

#include "ast_context.hpp"

class Node
{
protected:
    std::vector<Node *> branches_;

public:
    Node(){};

    virtual void EmitRISC(std::ostream &stream, Context &context) const = 0;
    // virtual void EmitRISCWithScopeContext(std::ostream &stream, Context &context, ScopeContext* cur_scope) const {};

    // Can either declare reg to save value, or wait for caller to declare
    virtual void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const {};

    virtual void Print(std::ostream &stream) const = 0;

    // New
    virtual std::string GetIdentifier() const {return "";};
    virtual ScopeContext* BuildContext(Context &context, ScopeContext* cur_scope) {};

    // For expressions
    virtual int GetNumBranches() const { return 1; };

    virtual ~Node() {
        for (auto branch : branches_)
        {
            delete branch;
        }
    }
};

#endif
// Represents a list of nodes.
