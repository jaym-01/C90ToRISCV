#ifndef AST_POINTER_HELPERS
#define AST_POINTER_HELPERS

#include "../ast_node.hpp"
#include "var_helpers.hpp"
#include <cmath>

inline int one_pointer_offset(Context &context, Node *expr){
    std::string e_id = expr->GetIdentifier();
    if(e_id != "" && context.cur_scope->var_map.find(e_id) != context.cur_scope->var_map.end() && context.cur_scope->GetVarFromId(e_id).is_pntr) return pow(2, type_to_shift_amt[context.cur_scope->var_map[e_id].type]);
    return 1;
}

#endif
