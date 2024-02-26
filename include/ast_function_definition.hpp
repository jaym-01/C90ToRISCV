#ifndef AST_FUNCTION_DEFINITION_HPP
#define AST_FUNCTION_DEFINITION_HPP

#include "ast_node.hpp"
#include "statement/ast_compound_statement.hpp"

class FunctionDefinition : public Node
{
private:
    Node *declaration_specifiers_;
    Node *declarator_;

    CompoundStatement *compound_statement_;

private:
    int CalcFrameSize(int total_var_size) const
    {
        int pointers_size = 8;
        int buffer = 8;
        return pointers_size + buffer + total_var_size;
    }

public:
    FunctionDefinition(Node *declaration_specifiers, Node *declarator, CompoundStatement *compound_statement) : declaration_specifiers_(declaration_specifiers), declarator_(declarator), compound_statement_(compound_statement){};
    ~FunctionDefinition()
    {
        delete declaration_specifiers_;
        delete declarator_;
        delete compound_statement_;
    };

    // Context should be function scoped?
    void EmitRISC(std::ostream &stream, Context &context) const override
    {
        std::cout<<"Emitting RISC for func def"<<std::endl;
        // Emit assembler directives.
        // TODO: these are just examples ones, make sure you understand
        // the concept of directives and correct them.
        stream << ".text" << std::endl;
        stream << ".globl "<<declarator_->GetIdentifier()<< std::endl;

        declarator_->EmitRISC(stream, context);

        // 1. Initialise function context
        FunctionContext* f_context = new FunctionContext(declarator_->GetIdentifier());
        context.InitFunctionContext(f_context);

        // 2. Build context for arguments
        ScopeContext* arg_scope = new ScopeContext(context.global_scope);
        // TODO: add arguments to arg_scope
        context.SetCurScope(arg_scope);

        std::stringstream compound_stream;
        std::string empty_reg = "";
        compound_statement_->EmitRISCWithExistingContext(compound_stream, context, empty_reg);

        context.f_context->SetRootScope(arg_scope);
        // arg_scope->PrintTree(0);


        // 3. Pre function calling procedure
        int cur_func_offset = context.GetCurFuncOffset();
        int total_frame_size = -cur_func_offset;
        stream << "addi sp, sp, " << -total_frame_size << std::endl; // TODO: if total frame_size > imm num of bits (12 bits)?
        stream<<"sw ra, "<<total_frame_size - 4<<"(sp)"<<std::endl;
        stream<<"sw fp, "<<total_frame_size - 8<<"(sp)"<<std::endl;
        stream << "addi fp, "<< "sp, " << total_frame_size<< std::endl;

        // 4. Compound statement RISC
        stream << compound_stream.str();

        // 5. Post function calling procedure
        stream << "return:" << std::endl;
        stream << "lw ra, " << total_frame_size - 4 << "(sp)" << std::endl;
        stream << "lw fp, " << total_frame_size - 8 << "(sp)" << std::endl;
        stream << "addi sp, sp, " << total_frame_size << std::endl;
        stream << "jr ra" << std::endl;
    };


    void Print(std::ostream &stream) const override {
        std::cout<<"func_def: "<<std::endl;
        declaration_specifiers_->Print(stream);
        stream << " ";
        declarator_->Print(stream);
        std::cout<<std::endl;
        // std::cout<<"\nfunc_comp_stmt: "<<std::endl;
        compound_statement_->Print(stream);

    };
};

#endif
