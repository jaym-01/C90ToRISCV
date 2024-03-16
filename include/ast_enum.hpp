#ifndef AST_ENUM
#define AST_ENUM

#include "ast_node.hpp"
#include "ast_node_list.hpp"

class EnumValue : public Node {
public:
    std::string id_;
    Node *constant_expr_;

    EnumValue(std::string id, Node *constant_expr): id_(id), constant_expr_(constant_expr) {}

    ~EnumValue(){
        if(constant_expr_ != nullptr) delete constant_expr_;
    }

    void Print(std::ostream &stream) const override{
        stream << id_;
        if(constant_expr_ != nullptr) {
            stream << ": ";
            constant_expr_->Print(stream);
        }
    }

    std::string GetIdentifier() const override {
        return id_;
    }

    std::vector<int> EvalExpression(std::string type) const override {
        // ignore type - enum is always an int
        if(constant_expr_ != nullptr) return constant_expr_->EvalExpression("int");
        else return {};
    }

};

class EnumSpecifier : public Node{
public:
    std::string id_;
    NodeList *enum_list_;

    EnumSpecifier(std::string id, NodeList *enum_list): id_(id), enum_list_(enum_list) {}

    ~EnumSpecifier(){
        if(enum_list_ != nullptr) delete enum_list_;
    }

    void EmitRISC(std::ostream &stream, Context &context) const override{
        // store the values in the context
        ScopeContext *scope = context.cur_scope;

        // for (auto enumNode : enum_list_->GetNodes()){
        //     // all enums of type int
        //     std::vector<int> result = enumNode->EvalExpression("int");
        //     if (result.size() > 0){
        //         cur_val = result[0];
        //     }

        //     id = enumNode->GetIdentifier();
        //     VariableContext var = {
        //         .id = id,
        //         .type = "int",
        //         .array_size = 1,
        //         .is_array = false,
        //         .is_enum = true,
        //         .enum_val = cur_val
        //     };

        //     scope->SetVarContext(id, var);
        //     cur_val++;
        // }
        AddToContext(scope, false);
    }

    void AddToContext(ScopeContext *scope, bool is_global) const {
        int cur_val = 0;
        std::string id;

        for (auto enumNode : enum_list_->GetNodes()){
            // all enums of type int
            std::vector<int> result = enumNode->EvalExpression("int");
            if (result.size() > 0){
                cur_val = result[0];
            }

            id = enumNode->GetIdentifier();
            VariableContext var = {
                .id = id,
                .type = "int",
                .array_size = 1,
                .is_array = false,
                .is_global = is_global,
                .is_enum = true,
                .enum_val = cur_val
            };

            scope->SetVarContext(id, var);
            cur_val++;
        }
    }

    void EmitRISCGlobalVar (std::ostream &stream, Context &context) const {
        ScopeContext *scope = context.global_scope;
        AddToContext(scope, true);
    }

    std::string GetTypeSpecifier() const override {
        // enums are always of type int
        return "int";
    }

    void Print(std::ostream &stream) const override{
        stream << "enum " <<  id_ << "{ ";
        if(enum_list_ != nullptr) enum_list_->Print(stream);
        stream << " }" << std::endl;
    }

};

#endif
