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
        ScopeContext* arg_scope = new ScopeContext(); // root scope
        // TODO: add function args to arg_scope
        compound_statement_->BuildContext(context, arg_scope);
        context.InitRootScope(arg_scope);
        arg_scope->PrintTree(0);

        // 3. Move stack pointer down
        int total_var_size = context.GetFuncTotalVarSize();
        int total_frame_size = CalcFrameSize(total_var_size);

        stream<<"addi sp, sp, "<<-total_frame_size<<std::endl; // TODO: if total frame_size > imm num of bits (12 bits)?
        stream<<"sw ra, "<<total_frame_size - 4<<"(sp)"<<std::endl;
        stream<<"sw fp, "<<total_frame_size - 8<<"(sp)"<<std::endl;
        stream << "addi fp, "<< "sp, " << total_frame_size<< std::endl;

        // 4. Emit RISC for compound statement
        compound_statement_->EmitRISC(stream, context);

        // 5. Restore
        stream << "return:" << std::endl;
        stream << "lw ra, " << total_frame_size - 4 << "(sp)" << std::endl;
        stream << "lw fp, " << total_frame_size - 8 << "(sp)" << std::endl;
        stream << "addi sp, sp, " << total_frame_size << std::endl;
        stream << "jr ra" << std::endl;
    };


    void Print(std::ostream &stream) const override;
};

#endif
