#ifndef AST_ARRAY_ACCESS_HPP
#define AST_ARRAY_ACCESS_HPP

#include "ast_node.hpp"
#include "helpers/var_helpers.hpp"

class ArrayAccess : public Node
{
private:
    Node* identifier_;
    NodeList* index_expression_;

public:
    ArrayAccess(Node* identifier, NodeList* index_expression) : identifier_(identifier), index_expression_(index_expression){};
    ~ArrayAccess(){
        delete identifier_;
        delete index_expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override {};

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {

        ScopeContext *cur_scope = context.GetCurScope();
        std::string id = identifier_->GetIdentifier();
        VariableContext var_context = cur_scope->GetVarFromId(id);

        if (dest_reg == "") {
            dest_reg = context.ReserveRegister(var_context.type);
        }

        // std::cout << "Dest reg: " << dest_reg << ", Emitting RISC for ";
        // Print(std::cout);
        // std::cout << std::endl;

        read_var_value(this, context, stream, var_context, dest_reg);
    };

    Node* GetIndexExpression() const {
        return index_expression_->GetNodes()[0];
    };

    void DefineConstantType(std::string type) override {}

    int GetNumBranches() const override {
        return 1;
    };

    void Print(std::ostream &stream) const override {
        stream << "arr_acc{";
        identifier_->Print(stream);
        stream<< "[";
        index_expression_->Print(stream);
        stream<<"]}";
    };

    std::string GetIdentifier() const override {
        return identifier_->GetIdentifier();
    }
};

#endif
