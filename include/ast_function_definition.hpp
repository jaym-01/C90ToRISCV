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
        stream << ".globl f" << std::endl;

        // 1. Initialise function context
        FunctionContext* f_context = new FunctionContext(declarator_->GetIdentifier());
        context.InitFunctionContext(f_context);

        // 2. Build contexts for compound statement
        compound_statement_->BuildContext(stream, context);
        // f_context->SetRootScope(compound_statement_->BuildContext(stream, context));

        // 3. Emit RISC for compound statement
        declarator_->EmitRISC(stream, context);

        if (compound_statement_ != nullptr)
        {
            compound_statement_->EmitRISC(stream, context);
        }

        // Add function to id2function
    };


    void Print(std::ostream &stream) const override;
};

#endif
