#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "context/ast_variable_context.hpp"

inline std::map<std::string, int> type_size = {
    {"int", 4},
    {"char", 1},
    {"float", 4},
};

// inline void load_var_to_reg(std::ostream &stream, std::string var_type, int var_offset,
//     std::string dest_reg, std::string addr_reg = "fp"){

//     // stream << "lw " << reg << " " << offset << "($fp)" << std::endl;
//     if (var_type == "int") {
//         stream << "lw " << dest_reg << ", " << var_offset << "("<<addr_reg<<")" << std::endl;
//     }
//     else if (var_type == "char") {
//         stream << "lbu" << dest_reg << ", " << var_offset << "("<<addr_reg<<")" << std::endl;
//     }
//     // else if (var_type == "float") {
//     //     stream << "l.s " << reg << " " << var_offset << "($fp)" << std::endl;
//     // }
//     // else if (var_type == "double") {
//     //     stream << "l.d " << reg << " " << var_offset << "($fp)" << std::endl;
//     // }
// }

// inline void load_array_elem_to_reg(std::ostream &stream, VariableContext var,
//     std::string index_reg, std::string dest_reg){
//     int offset;

//     std::map<std::string, int> type_to_shift_amt = {
//         // can also do log2(type_size)
//         {"int", 2},
//         {"char", 0},
//         {"float", 2},
//     };

//     // Set index offset amt in index reg
//     stream << "slli " << index_reg << ", " << index_reg << ", "<<type_to_shift_amt[var.type]<< std::endl;
//     // Add var offset to index reg
//     stream << "addi " << index_reg << ", " << index_reg << "," << var.offset << std::endl;
//     // Add total offset to fp
//     stream << "add " << index_reg << ", " << index_reg << ", fp" << std::endl;

//     load_var_to_reg(stream, var.type, 0, dest_reg, index_reg);
// }



inline int align_to_multiple_of_4(int offset)
{
    int remainder = offset % 4;
    if (remainder == 0) {
        return offset;
    }
    else {
        return offset + (4 - remainder);
    }
}

inline int calculate_var_offset(int cur_offset, VariableContext var_context) {
    int size = type_size[var_context.type];
    int offset = cur_offset - size * var_context.array_size;
    std::string type = var_context.type;

    if (type == "int") {
        return align_to_multiple_of_4(offset);
    }

    else if (type == "char") {
        return offset;
    }

    else if (type == "float") {
        return align_to_multiple_of_4(offset);
    }

    else if (type == "double") {
        // return cur_offset - 8;
    }

    else {
        return align_to_multiple_of_4(offset);
    }
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
