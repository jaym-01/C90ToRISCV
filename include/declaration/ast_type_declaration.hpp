#ifndef AST_TYPE_DECLARATION
#define AST_TYPE_DECLARATION

#include "../ast_node.hpp"
#include "../helpers/helpers.hpp"

class TypeDeclaration : public Node {
private:
    Node *type_specifier_;

public:
    TypeDeclaration(Node *type_specifier) : type_specifier_(type_specifier) {}

    ~TypeDeclaration() {
        delete type_specifier_;
    }

    std::string GetTypeSpecifier() const override {
        return type_specifier_->GetTypeSpecifier();
    }

    std::string GetType(Context &context) const override {
        return type_specifier_->GetType(context);
    }

    DeclaratorType GetDeclaratorType() const override {
        return DeclaratorType::TypeDef;

    }

    void Print(std::ostream &stream) const override {
        stream << "typedef (";
        type_specifier_->Print(stream);
        stream << ")";
    }
};

#endif
