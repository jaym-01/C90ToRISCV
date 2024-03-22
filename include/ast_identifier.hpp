#ifndef AST_IDENTIFIER_HPP
#define AST_IDENTIFIER_HPP

#include "ast_node.hpp"
#include "helpers/var_helpers.hpp"
#include <vector>
class Identifier : public Node
{
private:
    std::string identifier_;
    std::string type_;

public:
    Identifier(std::string identifier) : identifier_(identifier){};
    ~Identifier(){};

    void EmitRISC(std::ostream &stream, Context &context) const override {
        stream << identifier_;
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest_reg) const override {

        ScopeContext* cur_scope = context.GetCurScope();
        VariableContext var = cur_scope->GetVarFromId(identifier_);

        if (dest_reg == "") {
            dest_reg = context.ReserveRegister(var.is_array || var.is_pntr? type_: var.type);
        }

        if(var.is_enum){
            stream << "li " << dest_reg << ", " << var.enum_val << std::endl;
        } else{
            read_var_value(this, context, stream, var, dest_reg);
        }


        // if (var.offset > 0) {
        //     std::string err_msg = "Variable " + identifier_ + " was not initialized";
        //     throw std::runtime_error(err_msg);
        // }
    };

    int GetNumBranches() const override{
        return 1;
    };

    std::vector<VariableContext> InitVariableContext(std::string type) override {
        VariableContext var;
        var.id=identifier_;
        var.type=type;
        var.array_size=1;
        var.is_array=false;

        return {var};
    };

    void Print(std::ostream &stream) const override {
        stream << "id{" << identifier_ << "}";
    };

    std::vector<int> EvalExpression(std::string type) const override {
        // throw std::runtime_error("Cannot evaluate identifier as int expression");
        return {};
    };

    std::string GetIdentifier() const override {
        return identifier_;
    }

    DeclaratorType GetDeclaratorType() const override {
        return DeclaratorType::Variable;
    }

    void DefineConstantType(std::string type) override { type_ = type; }
    std::string GetType(Context &context) const override{
        ScopeContext* cur_scope = context.GetCurScope();
        if(cur_scope->var_map.find(identifier_) != cur_scope->var_map.end()){
            return cur_scope->GetVarFromId(identifier_).type;
        } else {
            // return the type passed down to it if not recieved a type
            return type_;
        }
    }

    bool IsMemoryReference(Context &context) const override {
        ScopeContext* cur_scope = context.GetCurScope();
        if(cur_scope->var_map.find(identifier_) != cur_scope->var_map.end()){
            return context.GetCurScope()->GetVarFromId(identifier_).is_pntr | context.GetCurScope()->GetVarFromId(identifier_).is_array;
        } else {
            return false;
        }
    }

    bool IsPointer() const override {
        return false;
    }

    std::string DFSIdentifier() const override {
        return identifier_;
    }

};

#endif
