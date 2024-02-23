#ifndef AST_COMPOUND_STATEMENT_HPP
#define AST_COMPOUND_STATEMENT_HPP

#include "ast_node.hpp"
#include "ast_node_list.hpp"

class CompoundStatement : public Node
{
private:
    NodeList* declaration_list_;
    NodeList* statement_list_;

public:
    CompoundStatement(NodeList* decl_list, NodeList* stmt_list) :
        declaration_list_(decl_list), statement_list_(stmt_list) {}

    ~CompoundStatement() {
        delete declaration_list_;
        delete statement_list_;
    }

    void AddDeclarationList(NodeList* decl_list) { declaration_list_ = decl_list; }
    void AddStatementList(NodeList* stmt_list) { statement_list_ = stmt_list; }

    ScopeContext* BuildContext(std::ostream &stream, Context &context) const {
        // std::cout<<"Building scope context for compound statement\n";

        // 1. Create new Scope Context
        ScopeContext* scope = new ScopeContext();

        // 2. Add declarations to scope

        return new ScopeContext();
    }

    void EmitRISC(std::ostream &stream, Context &context) const override {

    };

    void Print(std::ostream &stream) const override
    {
        if (declaration_list_ != nullptr) {
            declaration_list_->Print(stream);
        }
        if (statement_list_ != nullptr) {
            statement_list_->Print(stream);
        }
    };
};

#endif
