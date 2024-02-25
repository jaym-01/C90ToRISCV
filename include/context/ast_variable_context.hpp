
#include <map>
#include <iostream>
#include "vector"
#include "string.h"

#ifndef AST_VARIABLE_CONTEXT_HPP
#define AST_VARIABLE_CONTEXT_HPP

struct VariableContext
{
    // std::string storage_class;
    std::string type;
    int offset;
    bool is_array;
    int array_size;
};


#endif
