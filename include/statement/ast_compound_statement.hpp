#ifndef AST_COMPOUND_STATEMENT_HPP
#define AST_COMPOUND_STATEMENT_HPP

#include "ast_node.hpp"
#include "ast_node_list.hpp"

class CompoundStatement : public Node
{
private:
    NodeList* declaration_list_;
    NodeList* statement_list_;
    ScopeContext* scope_context_;

public:
    CompoundStatement(NodeList* decl_list, NodeList* stmt_list) :
        declaration_list_(decl_list), statement_list_(stmt_list) {}

    ~CompoundStatement() {
        delete declaration_list_;
        delete statement_list_;
    }

    void AddDeclarationList(NodeList* decl_list) { declaration_list_ = decl_list; }
    void AddStatementList(NodeList* stmt_list) { statement_list_ = stmt_list; }

    ScopeContext* BuildContext(Context &context, ScopeContext* cur_scope) override {
        std::cout<<"Building scope context for compound statement\n";

        // For root compound_statement, cur_scope is same as func arg_scope
        if (declaration_list_ != nullptr)
        {
            for (auto decl : declaration_list_->GetNodes()) {
                decl->BuildContext(context, cur_scope);
            }
        }

        if (statement_list_ != nullptr)
        {
            for (auto stmt : statement_list_->GetNodes()) {
                // Build context for compound statements
                // If statement_list->HasCompoundStatement() then create a new scope
                // Union returned statement with current scope
            }
        }

        // Set scope_context_ of CompountStatement
        scope_context_ = cur_scope;
        return cur_scope;
    }

    void EmitRISC(std::ostream &stream, Context &context) const override {

        // Scope child: compound statement's scope:
        ScopeContext* temp = context.GetCurScope();
        context.SetCurScope(scope_context_);

        // Set context.cur_scope to be compound statement's scope
        std::cout<<"---------"<<std::endl;
        std::cout<<"Emitting RISC for compound statement\n";

        if (declaration_list_ != nullptr) {
            for (auto decl : declaration_list_->GetNodes()) {
                decl->EmitRISC(stream, context);
            }
        }

        if (statement_list_ != nullptr) {
            for (auto stmt : statement_list_->GetNodes()) {
                std::string dest_reg = "";
                stmt->EmitRISCWithDest(stream, context, dest_reg);
                if (dest_reg != "") {
                    context.FreeTempRegister(dest_reg);
                }
            }
        }

        // Set context.cur_scope to be previous scope?
        scope_context_->PrintTree(0);
        context.SetCurScope(temp);
    };

    void Print(std::ostream &stream) const override
    {
        stream<<"Compound statment\n";
        if (declaration_list_ != nullptr) {
            declaration_list_->Print(stream);
        }
        if (statement_list_ != nullptr) {
            statement_list_->Print(stream);
        }
    };
};

#endif
