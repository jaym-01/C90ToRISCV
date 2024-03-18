#ifndef AST_NODE_LIST_HPP
#define AST_NODE_LIST_HPP

#include "ast_node.hpp"
#include  <vector>

class NodeList : public Node
{
private:
    std::vector<Node *> nodes_;

public:
    NodeList(Node *first_node) : nodes_({first_node}) {}

    ~NodeList()
    {
        for (auto node : nodes_)
        {
            if(node != nullptr) delete node;
        }
    }

    std::vector<Node *> GetNodes()
    {
        return nodes_;
    };

    void DefineConstantType(std::string type) override {
        for(auto node : nodes_) {
            if(node != nullptr) node->DefineConstantType(type);
        }
    }

    std::string GetType() const override {
        std::string type = "int";
        for(auto node : nodes_){
            if(node == nullptr) continue;
            type = node->GetType();
            if(type == "unsigned") return "unsigned";
        }

        return type;
    }

    void PushBack(Node *item) {
        nodes_.push_back(item);
    };

    void EmitRISC(std::ostream &stream, Context &context) const override{
        for (auto node : nodes_)
        {
            if (node == nullptr) continue;
            node->EmitRISC(stream, context);
        }
    };

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string& dest_reg) const override {
        // std::cout << "here nl" << std::endl;
        // For single expr statements
        if (nodes_.size() == 1) {
            nodes_[0]->EmitRISCWithDest(stream, context, dest_reg);
            return;
        }

        // For multi expr lines like x = 5, y = 6;
        for (auto node : nodes_) {
            if (node == nullptr){
                continue;
            }

            // TODO: dest_reg can't be shared with multiple nodes...
            // Make these 3 lines more efficient?
            std::string regD = "";
            node->EmitRISCWithDest(stream, context, regD);
            if (regD != "") {
                context.FreeRegister(regD);
            }
        }
    };

    void Print(std::ostream &stream) const override{
        stream<<"node_list[";
        for (auto node : nodes_)
        {
            if (node == nullptr) {
                continue;
            }

            node->Print(stream);
            if(node != nodes_.back())stream<<", ";
        }
        stream<<"]";
    };

    std::vector<int> EvalExpression(std::string type) const override {
        if (nodes_.size() == 1) {
            return nodes_[0]->EvalExpression(type);
        } else {
            throw std::runtime_error("Cannot evaluate int expression for multi_expression line");
        }
    };
    // New
    virtual void Extend(NodeList *nodes) {
        for (auto node : nodes->nodes_)
        {
            nodes_.push_back(node);
        }
    };

    virtual std::string GetTypeSpecifier() const override {
        if (nodes_.size() == 1) {
            return nodes_[0]->GetTypeSpecifier();
        } else {
            throw std::runtime_error("Cannot get type specifier twice...");
        }
    };

    bool IsMemoryReference(Context &context) const override {
        for (auto node : nodes_) {
            if (node == nullptr) continue;
            if (node->IsMemoryReference(context)) return true;
        }
        return false;
    };

};

#endif
