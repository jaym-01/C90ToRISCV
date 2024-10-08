#ifndef AST_POINTER_DECLARATION_HPP
#define AST_POINTER_DECLARATION_HPP

#include "../ast_node.hpp"

class PointerDeclarator : public Node {
private:
int pointer_depth_;
Node* direct_declarator_;

public:
    PointerDeclarator(int pointer_depth, Node* direct_declarator) : pointer_depth_(pointer_depth), direct_declarator_(direct_declarator){};

    ~PointerDeclarator(){
        delete direct_declarator_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override {
        direct_declarator_->EmitRISC(stream, context);
    }

    std::vector<VariableContext> InitVariableContext(std::string type) override {
        VariableContext var = direct_declarator_->InitVariableContext(type)[0];
        var.is_pntr = true;
        var.pntr_depth = pointer_depth_;
        var.working_pntr_depth = pointer_depth_;

        return {var};
    }

    DeclaratorType GetDeclaratorType() const override { return direct_declarator_->GetDeclaratorType(); }

    std::string GetIdentifier() const override { return direct_declarator_->GetIdentifier(); }

    void DefineConstantType(std::string type) override { direct_declarator_->DefineConstantType(type); }

    std::string GetType(Context &context) const override { return direct_declarator_->GetType(context); }

    int GetNumBranches() const override{ return direct_declarator_->GetNumBranches(); };

    bool IsPointer() const override { return true; }

    int GetPointerDepth() const override { return pointer_depth_; }

    void Print(std::ostream &stream) const override {
        stream << "pntr(";
        for(int i = 0; i < pointer_depth_; i++) stream << "*";
        stream << "){";
        direct_declarator_->Print(stream);
        stream << "}";
    }

};

#endif
