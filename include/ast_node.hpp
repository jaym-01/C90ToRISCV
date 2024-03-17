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

    virtual void EmitRISC(std::ostream &stream, Context &context) const {
        throw std::runtime_error("Error: EmitRISC not implemented for this node");
    };
    // Can either declare reg to save value, or wait for caller to declare
    virtual void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const {};
    // For compound statements
    // virtual void EmitRISCWithExistingContext(std::ostream &stream, Context &context) const {
    //     throw std::runtime_error("Error: EmitRISCWithExistingContext not implemented for this node");
    // };

    virtual void Print(std::ostream &stream) const = 0;

    // New
    virtual std::string GetIdentifier() const {
        // can be changed to helper spot errors
        // change by implementing in all expressions
        // used for pointer_offset
        // throw std::runtime_error("Error: GetIdentifier not implemented for this node");
        return "";
    };

    virtual std::string GetTypeSpecifier() const {
        throw std::runtime_error("Error: GetTypeSpecifier not implemented for this node");
    };

    // For declarations
    virtual VariableContext InitVariableContext(std::string type) {
        throw std::runtime_error("Error: InitVariableContext not implemented for this node");
    };
    virtual void EmitRISCGlobalVar (std::ostream &stream, Context &context) const {
        throw std::runtime_error("Error: EmitRISCGlobalVar not implemented for this node");
    };
    virtual DeclaratorType GetDeclaratorType() const {
        throw std::runtime_error("Error: GetDeclaratorType not implemented for this node");
        return DeclaratorType::None;
    };

    // needed for the FloatDouble constant
    // as type of the constant is only know when the value is being used
    virtual void DefineConstantType(std::string type) {
        // TODO: FIX THIS - IT IS CAUSING TESTS TO FAIL
        throw std::runtime_error("Error: DefineConstantType not implemented for this node");
    }
    virtual std::string GetType() const{
        throw std::runtime_error("Error: GetType not implemented for this node");
    }
    // virtual ScopeContext* BuildContext(Context &context, ScopeContext* cur_scope) {}; // old

    // For expressions
    virtual int GetNumBranches() const { return 1; };

    // For arrays
    virtual std::vector<int> EvalExpression(std::string type) const {
        throw std::runtime_error("Error: EvalExpression not implemented for this node");
    };
    virtual Node* GetIndexExpression() const {
        return nullptr;
        // throw std::runtime_error("Error: GetIndexExpression not implemented for this node");
    };

    virtual bool IsDereference() const { return false; }

    // onyl used in switch case
    virtual void PassRegister(std::string &reg) {
        // throw std::runtime_error("Error: PassRegister not implemented for this node");
    };
    virtual void EmitCases(std::ostream &stream, Context &context) {}

    virtual ~Node() {
        for (auto branch : branches_)
        {
            delete branch;
        }
    }
};

#endif
// Represents a list of nodes.
