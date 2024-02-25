
#ifndef AST_INIT_DECLARATOR_HPP
#define AST_INIT_DECLARATOR_HPP

#include "../ast_node.hpp"

// Declarator can either be a direct declarator or a pointer declarator
// Direct declarator can either be an identifier or a function declarator

class InitDeclarator : public Node
{
private:
    Node *declarator_;
    NodeList *initializer_;

public:
    InitDeclarator(Node *declarator, NodeList *initializer) :
        declarator_(declarator), initializer_(initializer) {};

    ~InitDeclarator()
    {
        delete declarator_;
        delete initializer_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override {
        // Declarator, initializer
        // declarator_->EmitRISC(stream, context);

        ScopeContext* cur_scope = context.GetCurScope();
        std::string id = declarator_->GetIdentifier();
        if (initializer_ != nullptr)
        {

            // NEW:

            VariableContext var_context = cur_scope->GetVarFromId(id);
            std::vector<Node*> initializers = initializer_->GetNodes();
            int cur_func_offset = context.GetCurFuncOffset();


            int var_offset = calculate_var_offset(cur_func_offset, var_context);
            cur_scope->SetVarOffset(id, var_offset);
            context.SetCurFuncOffset(var_offset);

            // For each initializer:
            for (int i = 0; i < initializers.size(); i++) {
                std::string dest_reg = "";
                initializers[i]->EmitRISCWithDest(stream, context, dest_reg);
                stream << "sw " << dest_reg << ", " << var_offset << "(fp)" << std::endl;
                context.FreeTempRegister(dest_reg);

                // Go to next offset
                var_offset += type_size[var_context.type];
            }
        }
    };

    VariableContext InitVariableContext(std::string type) override {

        VariableContext var_context = declarator_->InitVariableContext(type);

        if (!var_context.is_array && initializer_ != nullptr && initializer_->GetNodes().size() > 1)
            throw std::runtime_error("Error: variable " + declarator_->GetIdentifier() + " is not an array but has multiple initializers");

        if (var_context.is_array && var_context.array_size == -1) {
            if (initializer_ == nullptr)
                throw std::runtime_error("Error: array variable " + declarator_->GetIdentifier() + " has no size specified");

            var_context.array_size = initializer_->GetNodes().size();
        }

        return var_context;
    };

    void Print(std::ostream &stream) const override {
        std::cout<<"init_decl{ ";
        declarator_->Print(stream);
        if (initializer_ != nullptr)
        {
            std::cout << " = ";
            initializer_->Print(stream);
            // std::cout << ", ";
        }
        std::cout<<" }";
    };

    std::string GetIdentifier() const override {
        return declarator_->GetIdentifier();
    }
};

#endif
