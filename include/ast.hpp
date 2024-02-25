#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>


#include "ast_node_list.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include "ast_jump_statement.hpp"
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"
#include "ast_constant.hpp"

// Context
#include "./context/ast_context.hpp"
#include "./context/ast_variable_context.hpp"

// Declaration
#include "declaration/ast_declaration.hpp"
#include "declaration/ast_declaration_specifiers.hpp"

// Declarators
#include "declaration/ast_init_declarator.hpp"

// Expression
#include "expression/ast_postfix_expression.hpp"
#include "expression/ast_unary_expression.hpp"
#include "expression/ast_binary_expression.hpp"
#include "expression/ast_assignment_expression.hpp"

// Arrays
#include "declaration/ast_array_declarator.hpp"
#include "expression/ast_array_access.hpp"

// Statements
#include "statement/ast_compound_statement.hpp"

extern Node *ParseAST(std::string file_name);

#endif
