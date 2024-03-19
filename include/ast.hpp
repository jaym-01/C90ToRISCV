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
#include "ast_enum.hpp"
#include "ast_string_constant.hpp"

// Context
#include "./context/ast_context.hpp"
#include "./context/ast_variable_context.hpp"

// Declaration
#include "declaration/ast_declaration.hpp"
#include "declaration/ast_declaration_specifiers.hpp"
#include "declaration/ast_global_variable.hpp"
#include "declaration/ast_param_declaration.hpp"

// Declarators
#include "declaration/ast_init_declarator.hpp"
#include "declaration/ast_function_declarator.hpp"
#include "declaration/ast_pointer_declarator.hpp"
#include "declaration/ast_struct_declarator.hpp"

// Expression
#include "expression/ast_postfix_expression.hpp"
#include "expression/ast_unary_expression.hpp"
#include "expression/ast_binary_expression.hpp"
#include "expression/ast_assignment_expression.hpp"
#include "expression/ast_function_call.hpp"
#include "expression/ast_ternary_operator.hpp"
#include "expression/ast_size_of.hpp"
#include "expression/ast_struct_access.hpp"

// Arrays
#include "declaration/ast_array_declarator.hpp"
#include "expression/ast_array_access.hpp"

// Statements
#include "statement/ast_compound_statement.hpp"
#include "statement/ast_empty_statement.hpp"
#include "statement/ast_if_else_statement.hpp"
#include "statement/ast_while_statement.hpp"
#include "statement/ast_do_while_statement.hpp"
#include "statement/ast_for_statement.hpp"
#include "statement/ast_break_continue_statement.hpp"
#include "statement/ast_switch_case_statement.hpp"

extern Node *ParseAST(std::string file_name);

#endif
