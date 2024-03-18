
#ifndef AST_INIT_DECLARATOR_HPP
#define AST_INIT_DECLARATOR_HPP

#include "../ast_node.hpp"
#include "../helpers/memory_helpers.hpp"

// Declarator can either be a direct declarator or a pointer declarator
// Direct declarator can either be an identifier or a function declarator

class InitDeclarator : public Node
{
private:
    Node *declarator_;
    // value to initialize with
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

        VariableContext var_context = cur_scope->GetVarFromId(id);
        int cur_func_offset = context.GetCurFuncOffset();
        declarator_->DefineConstantType(var_context.type);

        // gets the offset relative to the fp
        int var_offset = calculate_var_offset(cur_func_offset, var_context);
        // stores the offset for the variable so it can be accessed again
        // This part allocates memory to the variable
        cur_scope->SetVarOffset(id, var_offset);
        context.SetCurFuncOffset(var_offset);

        int t_size = type_size[var_context.type];
        if (initializer_ != nullptr)
        {

            // NEW:
            std::vector<Node*> initializers = initializer_->GetNodes();

            // For each initializer:
            for (std::size_t i = 0; i < initializers.size(); i++) {
                std::string dest_reg = "";
                // AT THIS STAGE YOU HAVE VARIABLE CONTEXT -- NEED TO PASS IT THROUGH TO TELL IF DOUBLE OR FLOAT NEEDED
                initializers[i]->DefineConstantType(var_context.GetType());

                initializers[i]->EmitRISCWithDest(stream, context, dest_reg);

                stream << get_mem_write(var_context.type, var_context.is_pntr) << " " << dest_reg << ", " << var_offset << "(fp)" << std::endl;

                context.FreeRegister(dest_reg);

                // Go to next offset
                var_offset += t_size;
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

    void EmitRISCGlobalVar(std::ostream &stream, Context &context) const override {
        std::cout<<"Emitting RISC for glo var: ";
        Print(std::cout);
        std::cout<<std::endl;

        if (GetDeclaratorType() == DeclaratorType::Function) {
            declarator_->EmitRISCGlobalVar(stream, context);
            return;
        }

        ScopeContext *cur_scope = context.global_scope;
        std::string id = declarator_->GetIdentifier();
        VariableContext var_context = cur_scope->GetVarFromId(id);
        stream << id << ":" << std::endl;

        if (initializer_ != nullptr)
        {
            std::vector<Node *> initializers = initializer_->GetNodes();

            // For each initializer:
            for (std::size_t i = 0; i < initializers.size(); i++)
            {
                initializers[i]->DefineConstantType(var_context.type);
                if(var_context.is_pntr && initializers[i]->IsMemoryReference(context)){
                    // it's a pointer so is always an int
                    int offset = initializers[i]->EvalExpression(var_context.type)[0];
                    if(offset != 0) stream << ".word " << initializers[i]->DFSIdentifier() << "+" << (offset*4) << std::endl;
                    else stream << ".word " << initializers[i]->DFSIdentifier() << std::endl;
                } else{
                    std::vector<int> eval = initializers[i]->EvalExpression(var_context.type);

                    for(int constant : eval){
                        if(var_context.type == "char") stream << ".byte ";
                        else stream << ".word ";
                        stream << constant << std::endl;
                    }
                }

                // std::string dest_reg = "";
                // initializers[i]->EmitRISCWithDest(stream, context, dest_reg);
                // stream << "sw " << dest_reg << ", " << var_offset << "(fp)" << std::endl;
                // context.FreeRegister(dest_reg);

                // // Go to next offset
                // var_offset += type_size[var_context.type];
            }
        } else {
            stream << ".zero " << type_size[var_context.GetType()] << std::endl;
        }
        stream<<std::endl;
    };

    DeclaratorType GetDeclaratorType() const override {
        return declarator_->GetDeclaratorType();
    };
};

#endif
