#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "../context/ast_variable_context.hpp"
#include "memory_helpers.hpp"
#include <cstring>
#include <vector>

enum DeclaratorType
{
    Variable,
    Function,
    Array,
    None
};

inline std::map<std::string, int> type_size = {
    {"int", 4},
    {"unsigned", 4},
    {"char", 1},
    {"float", 4},
    {"double", 8}
};

// used for calculating the actual value
template <typename T>
std::vector<int> CalcVal(std::vector<int> left, std::vector<int> right, std::string b_op){
    int n = left.size();
    int l[n], r[n];
    for(std::size_t i = 0; i < left.size(); i++){
        l[i] = left[i];
        r[i] = right[i];
    }

    T x, y, res;
    int* tmp_out = new int[n];
    std::memcpy(&x, &l, sizeof(T));
    std::memcpy(&y, &r, sizeof(T));

    if (b_op == "+") {
        res = x + y;
    } else if (b_op == "-") {
        res = x - y;
    } else if (b_op == "*") {
        res = x * y;
    } else if (b_op == "/") {
        res = x / y;
    } else {
        throw std::runtime_error("Error: Invalid binary operator");
    }

    std::vector<int> out;
    std::memcpy(tmp_out, &res, sizeof(T));
    for(int i = 0; i < n; i++) out.push_back(tmp_out[i]);
    delete[] tmp_out;

    return out;
}

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
    std::string type = var_context.type;
    // always an int as it passes the address for array parameters / pointers
    if((var_context.is_param && var_context.is_array) || var_context.is_pntr) type = "int";
    int size = type_size[type];
    int offset;

    // if param - store address if array so array size must be 1
    offset = cur_offset - size * (var_context.is_param || var_context.is_pntr? 1: var_context.array_size);

    if (type == "char") {
        return offset;
    } else {
        return align_word(offset);
    }
}

inline int calculate_arg_overflow(int &cur_offset, VariableContext var_context) {
    // int size = type_size[var_context.type] * var_context.array_size;
    int size = type_size[var_context.type];

    // First calculate aligned offset of last
    int aligned_offset;
    // ensures it aligns words for char arrays too
    if (var_context.is_array) {
        aligned_offset = align_word(cur_offset);
    } else if (var_context.type == "char") {
        aligned_offset = cur_offset;
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
