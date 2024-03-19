#ifndef AST_STRUCT_DECLARATOR
#define AST_STRUCT_DECLARATOR

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"
#include "../context/ast_variable_context.hpp"

class StructSpecifier : public Node {
private:
    std::string identifier_;
    NodeList *declaration_list_;

public:
    StructSpecifier(std::string identifier, NodeList *declaration_list) : identifier_(identifier), declaration_list_(declaration_list) {}

    ~StructSpecifier() {
        if(declaration_list_ != nullptr) delete declaration_list_;
    }

    void EmitRISCGlobalVar(std::ostream &stream, Context &context) const override {
        // this is for defining a struct
        // no risc is emitted
        StructContext struct_context;
        struct_context.id = identifier_;
        struct_context.type = "struct";
        struct_context.array_size = 1;
        struct_context.is_array = false;

        std::vector<Node *> vars = declaration_list_->GetNodes();

        for(auto var : vars){
            var->EmitRISCGlobalVar(stream, context);
        }

    }

    void Print(std::ostream &stream) const override{
        stream << "struct " << identifier_ << " { ";
        declaration_list_->Print(stream);
        stream << " }" << std::endl;
    }

};


class StructDeclarator : public Node {
private:
    NodeList *qualifier_list_;
    NodeList *declarator_list_;

public:
    StructDeclarator(NodeList *qualifier_list, NodeList *declarator_list) : qualifier_list_(qualifier_list), declarator_list_(declarator_list) {}

    ~StructDeclarator() {
        if(qualifier_list_ != nullptr) delete qualifier_list_;
        if(declarator_list_ != nullptr) delete declarator_list_;
    }

    VariableContext InitVariableContext(std::string type) override {
        std::string type = qualifier_list_->GetNodes()[0]->GetTypeSpecifier();
        std::vector<VariableContext> var_contexts;
        std::vector<Node *> declarators = declarator_list_->GetNodes();

        for(auto declarator : declarators){
            var_contexts.push_back(declarator->InitVariableContext(type));
        }
    }

    void Print(std::ostream &stream) const override{
        std::cout << "struct_dec( ";
        qualifier_list_->Print(stream);
        stream << " ";
        declarator_list_->Print(stream);
        stream << " )";
    }
};

#endif
