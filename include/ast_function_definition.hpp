#ifndef AST_FUNCTION_DEFINITION_HPP
#define AST_FUNCTION_DEFINITION_HPP

#include "ast_node.hpp"
#include "statement/ast_compound_statement.hpp"
#include "helpers/ast_type_helpers.hpp"

class FunctionDefinition : public Node
{
private:
    Node *declaration_specifiers_;
    Node *declarator_;
    NodeList* declaration_list_;
    CompoundStatement *compound_statement_;

private:
    int CalcFrameSize(int total_var_size) const
    {
        int pointers_size = 8;
        int buffer = 8;
        return pointers_size + buffer + total_var_size;
    }

public:
    FunctionDefinition(
        Node *declaration_specifiers,
        Node *declarator,
        NodeList* declaration_list,
        CompoundStatement *compound_statement
    ) : declaration_specifiers_(declaration_specifiers),
        declarator_(declarator),
        declaration_list_(declaration_list),
        compound_statement_(compound_statement){};

    ~FunctionDefinition()
    {
        delete declaration_specifiers_;
        delete declarator_;
        delete compound_statement_;
        if (declaration_list_ != nullptr)
            delete declaration_list_;
    };

    // Context should be function scoped?
    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std::cout<<"Emitting RISC for func def"<<std::endl;

        // 1. Directives for function
        std::string id = declarator_->GetIdentifier();
        stream << ".text" << std::endl;
        stream << ".globl " << id << std::endl;
        stream << id << ": " << std::endl;

        std::string type = declaration_specifiers_->GetTypeSpecifier();
        TypeDefContext type_context = resolve_type(type, context.global_scope);
        type = type_context.type;

        // 1. Initialise function context and root scope context
        FunctionContext *f_context = new FunctionContext(id, context.GetNewLabel("return"), type, declaration_specifiers_->IsPointer());
        context.InitFunctionContext(f_context);
        ScopeContext* arg_scope = new ScopeContext(context.global_scope);
        context.SetCurScope(arg_scope);
        // 2. Emit RISC for function declarator (build params)
        std::stringstream declarator_stream;
        declarator_->EmitRISC(declarator_stream, context);
        context.id_to_func_def[id].return_type = type; // set return type of function

        if (declaration_list_ != nullptr)
            declaration_list_->Print(stream);

        if (compound_statement_ == nullptr) return;

        std::stringstream compound_stream;
        std::string empty_reg = "";
        compound_statement_->EmitRISCWithExistingContext(compound_stream, context, empty_reg);
        context.f_context->SetRootScope(arg_scope); // Needed for destructor?



        // 3. Pre function calling procedure
        int cur_func_offset = context.GetStackOffset();
        int max_func_arg_overflow = context.GetCurFuncMaxArgOverflow();
        int total_frame_size = -cur_func_offset + max_func_arg_overflow;
        stream << "addi sp, sp, " << -total_frame_size << std::endl; // TODO: if total frame_size > imm num of bits (12 bits)?
        //at the top of the stack
        stream<<"sw ra, "<<total_frame_size - 4<<"(sp)"<<std::endl;
        // in the word below
        stream<<"sw fp, "<<total_frame_size - 8<<"(sp)"<<std::endl;
        stream << "addi fp, "<< "sp, " << total_frame_size<< std::endl;

        // 4. Compound statement RISC
        stream << declarator_stream.str();
        stream << compound_stream.str();

        // 5. Post function calling procedure
        // Return label should be unique
        stream << context.GetFuncReturnLabel()<<":"<< std::endl;
        stream << "lw ra, " << total_frame_size - 4 << "(sp)" << std::endl;
        stream << "lw fp, " << total_frame_size - 8 << "(sp)" << std::endl;
        stream << "addi sp, sp, " << total_frame_size << std::endl;
        stream << "jr ra" << std::endl<<std::endl;

        stream << f_context->extern_declns << std::endl;

        std::cout << declarator_->GetIdentifier() << " has offset: " << total_frame_size << " | " << cur_func_offset << std::endl;
        // Delete f_context?
        delete f_context;
        context.f_context = nullptr;
    };


    void Print(std::ostream &stream) const override {
        std::cout<<"\nfunc_def: "<<std::endl;
        declaration_specifiers_->Print(stream);
        stream << " ";
        declarator_->Print(stream);
        std::cout<<std::endl;
        // std::cout<<"\nfunc_comp_stmt: "<<std::endl;
        compound_statement_->Print(stream);

    };
};

#endif
