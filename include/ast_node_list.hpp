#ifndef AST_NODE_LIST_HPP
#define AST_NODE_LIST_HPP

#include "ast_node.hpp"

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
            delete node;
        }
    }

    std::vector<Node *> GetNodes()
    {
        return nodes_;
    };

    void PushBack(Node *item) {
        nodes_.push_back(item);
    };

    virtual void EmitRISC(std::ostream &stream, Context &context) const override{
        for (auto node : nodes_)
        {
            if (node == nullptr)
            {
                continue;
            }
            node->EmitRISC(stream, context);
        }
    };

    virtual void Print(std::ostream &stream) const override{
        for (auto node : nodes_)
        {
            if (node == nullptr)
            {
                continue;
            }
            node->Print(stream);
        }
    };

    // New
    virtual void Extend(NodeList *nodes) {
        for (auto node : nodes->nodes_)
        {
            nodes_.push_back(node);
        }
    };


};

#endif
