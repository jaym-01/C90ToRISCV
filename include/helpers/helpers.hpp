#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "../context/ast_variable_context.hpp"
#include "memory_helpers.hpp"

enum DeclaratorType
{
    Variable,
    Function,
    Array,
    None
};

inline std::map<std::string, int> type_size = {
    {"int", 4},
    {"char", 1},
    {"float", 4},
    {"double", 8}
};

// inline int align_to_multiple_of_4(int offset)
// {
//     int remainder = offset % 4;
//     if (remainder == 0) {
//         return offset;
//     }
//     else {
//         return offset + (4 - remainder);
//     }
// }

// inline int align_to_multiple_of_4(int offset)
// {
//     int remainder = offset % 4;
//     if (remainder == 0)
//     {
//         return offset;
//     }
//     else
//     {
//         if (offset < 0)
//         {
//             return -(offset + (4 - remainder));
//         }
//         else
//         {
//             return offset + (4 - remainder);
//         }
//     }
// }

inline int calculate_var_offset(int cur_offset, VariableContext var_context) {
    int size = type_size[var_context.type];
    int offset;

    offset = cur_offset - size * var_context.array_size;

    std::string type = var_context.type;

    if (type == "int") {
        return align_word(offset);
    }

    else if (type == "char") {
        return offset;
    }

    else if (type == "float") {
        return align_word(offset);
    }

    else if (type == "double") {
        // return cur_offset - 8;
    }

    else {
        return align_word(offset);
    }
}

inline int calculate_arg_overflow(int &cur_offset, VariableContext var_context) {
    int size = type_size[var_context.type] * var_context.array_size;
    std::string type = var_context.type;

    // First calculate aligned offset of last
    int aligned_offset;
    if (type == "int") {
        aligned_offset = align_word(cur_offset);
    } else if (type == "char") {
        aligned_offset = cur_offset;
    } else if (type == "float") {
        aligned_offset = align_word(cur_offset);
    } else if (type == "double") {
        // return cur_offset - 8;
    } else {
        aligned_offset = align_word(cur_offset);
    }

    // New cur_offset = aligned_offset + size
    cur_offset = aligned_offset + size;
    return aligned_offset;

}
inline int calculate_var_size(VariableContext var)
{
    if (var.is_array) {
        return type_size[var.type] * var.array_size;
    }
    else {
        return type_size[var.type];
    }
}

#endif
