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

    void EmitRISCHelper(std::ostream &stream, Context &context, std::string &dest_reg) const {
        // Set context.cur_scope to be compound statement's scope
        // std::cout << "---------" << std::endl;
        // std::cout << "Emitting RISC for compound statement\n";

        if (declaration_list_ != nullptr) {
            for (auto decl : declaration_list_->GetNodes()) {
                decl->EmitRISC(stream, context);
            }
        }

        if (statement_list_ != nullptr) {
            for (auto stmt : statement_list_->GetNodes()) {
                std::string dest_reg = "";
                stmt->EmitRISCWithDest(stream, context, dest_reg);
                context.FreeRegister(dest_reg);

                // context.PrintAvailTempRegs();

                // if (context.temp_registers_avail.size() < 6 || context.fp_registers_avail.size() < 32) {
                //     throw std::runtime_error("Less than 6 registers left after statement. Check for leaks");
                // }
            }
        }

        if (context.temp_registers_avail.size() < 6 || context.fp_registers_avail.size() < 32) {
            throw std::runtime_error("Less than 6 registers left after statement. Check for leaks");
        }
    }

    void EmitRISCWithExistingContext(std::ostream &stream, Context &context, std::string &dest) const {
        // Carry over previous context here
        EmitRISCHelper(stream, context, dest);
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override {
        ScopeContext* cur_scope = context.GetCurScope();

        // Create new scope for compound statement
        ScopeContext* tmp = context.GetCurScope();
        ScopeContext* new_scope = new ScopeContext(tmp);
        context.SetCurScope(new_scope);

        // std::cout<<"Creating new scope for compound statement\n";
        EmitRISCHelper(stream, context, dest);
        // tmp->PrintTree(0);

        tmp->AddChildScope(new_scope);
        tmp->MergeWithChild(new_scope);

        context.SetCurScope(tmp);
    };

    void Print(std::ostream &stream) const override
    {
        // stream<<"\ncomp_stmt: \n";
        if (declaration_list_ != nullptr) {
            stream << std::endl << "decs:";
            declaration_list_->Print(stream);
            stream << std::endl;
        }
        if (statement_list_ != nullptr) {
            stream << std::endl << "stmts:" << std::endl;
            statement_list_->Print(stream);
            stream << std::endl;
        }
    };
};

#endif



// OLD
// ScopeContext *BuildContext(Context &context, ScopeContext *cur_scope) override
// {
//     std::cout << "Building scope context for compound statement\n";

//     // For root compound_statement, cur_scope is same as func arg_scope
//     if (declaration_list_ != nullptr)
//     {
//         for (auto decl : declaration_list_->GetNodes())
//         {
//             decl->BuildContext(context, cur_scope);
//         }
//     }

//     if (statement_list_ != nullptr)
//     {
//         for (auto stmt : statement_list_->GetNodes())
//         {
//             // Build context for compound statements
//             // If statement_list->HasCompoundStatement() then create a new scope
//             // Union returned statement with current scope
//         }
//     }

//     // Set scope_context_ of CompountStatement
//     scope_context_ = cur_scope;
//     return cur_scope;
// }
