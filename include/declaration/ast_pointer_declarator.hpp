#ifndef AST_POINTER_DECLARATION_HPP
#define AST_POINTER_DECLARATION_HPP

#include "../ast_node.hpp"

class PointerDeclaration : public Node {
private:
Node* pointers_;
Node* direct_declarator_;

public:
    PointerDeclaration(Node* pointers, Node* direct_declarator) : pointers_(pointers), direct_declarator_(direct_declarator){};

    ~PointerDeclaration(){
        delete pointers_;
        delete direct_declarator_;
    };

};

#endif
