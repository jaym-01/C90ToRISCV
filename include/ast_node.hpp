#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <iostream>
#include <vector>
#include <sstream>

#include "./context/ast_context.hpp"

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
    virtual std::string GetIdentifier() const {
        throw std::runtime_error("Error: GetIdentifier not implemented for this node");
    };

    // For declarations
    virtual VariableContext InitVariableContext(std::string type) {
        throw std::runtime_error("Error: InitVariableContext not implemented for this node");
    };
    virtual void GlobalVarEmitRISC (std::ostream &stream, Context &context) const {
        throw std::runtime_error("Error: GlobalVarEmitRISC not implemented for this node");
    };
    // virtual ScopeContext* BuildContext(Context &context, ScopeContext* cur_scope) {}; // old

    // For expressions
    virtual int GetNumBranches() const { return 1; };

    // For arrays
    virtual int EvalIntExpression() const {
        throw std::runtime_error("Error: EvalExpression not implemented for this node");
    };
    virtual Node* GetIndexExpression() const {
        throw std::runtime_error("Error: GetIndexExpression not implemented for this node");
    };

    virtual ~Node() {
        for (auto branch : branches_)
        {
            delete branch;
        }
    }
};

#endif
// Represents a list of nodes.
