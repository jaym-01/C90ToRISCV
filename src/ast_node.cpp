#include "ast_node.hpp"

// Node::~Node()
// {
//     for (auto branch : branches_)
//     {
//         delete branch;
//     }
// }

// void NodeList::PushBack(Node *item)
// {
//     nodes_.push_back(item);
// }

// void NodeList::EmitRISC(std::ostream &stream, Context &context) const
// {
//     for (auto node : nodes_)
//     {
//         if (node == nullptr)
//         {
//             continue;
//         }
//         node->EmitRISC(stream, context);
//     }
// }

// void NodeList::Print(std::ostream &stream) const
// {
//     for (auto node : nodes_)
//     {
//         if (node == nullptr)
//         {
//             continue;
//         }
//         node->Print(stream);
//     }
// }

// void NodeList::Extend(NodeList* new_nodes)
// {
//     for (auto node : new_nodes->nodes_)
//     {
//         nodes_.push_back(node);
//         // std::cout<<"Inserting new node"<<std::endl;
//     }
// }
