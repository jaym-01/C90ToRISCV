#ifndef AST_STRUCT_ACCESS
#define AST_STRUCT_ACCESS

#include "../ast_node.hpp"
#include "../helpers/memory_helpers.hpp"
#include "../helpers/var_helpers.hpp"

class StructAccess : public Node {
private:
    Node* pf_expr_;
    std::string identifier_;

public:
    StructAccess(Node* pf_expr, std::string identifier) : pf_expr_(pf_expr), identifier_(identifier) {}

    ~StructAccess() {
        delete pf_expr_;
    }

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {
        // get the identifer of the struct
        std::string struct_id = pf_expr_->GetIdentifier();
        // std::cout << struct_id << std::endl;

        ScopeContext *cur_scope = context.GetCurScope();
        VariableContext struct_context = cur_scope->GetVarFromId(struct_id);
        VariableContext var_context = struct_context.GetMemberById(identifier_);

        // write the memeber into a register
        if(dest_reg == "") dest_reg = context.ReserveRegister(var_context.GetType());

        read_var_value(nullptr, context, stream, var_context, dest_reg);
        // stream << get_mem_read(var_context.GetType(), var_context.is_pntr) << " " << dest_reg << "," << var_context.offset << "(fp)" << std::endl;
    }

    std::string GetIdentifier() const override {
        return pf_expr_->GetIdentifier();
    }

    std::string GetMemberId() const override {
        return identifier_;
    }

    std::string GetType(Context &context) const override {
        std::string struct_id = pf_expr_->GetIdentifier();
        return context.GetCurScope()->GetVarFromId(struct_id).GetMemberById(identifier_).GetType();
    }

    void DefineConstantType(std::string type) override {
        pf_expr_->DefineConstantType(type);
    }

    void Print(std::ostream &stream) const override {
        pf_expr_->Print(stream);
        stream << "." << identifier_;
    }

};

#endif
