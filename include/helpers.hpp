#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "./context/ast_variable_context.hpp"

inline std::map<std::string, int> type_size = {
    {"int", 4},
    {"char", 1},
    {"float", 4},
};

inline void load_var_to_reg(std::ostream &stream, std::string var_type, int var_offset, std::string reg){
    // stream << "lw " << reg << " " << offset << "($fp)" << std::endl;
    if (var_type == "int") {
        stream << "lw " << reg << ", " << var_offset << "(fp)" << std::endl;
    }
    else if (var_type == "char") {
        stream << "lbu" << reg << ", " << var_offset << "(fp)" << std::endl;
    }
    // else if (var_type == "float") {
    //     stream << "l.s " << reg << " " << var_offset << "($fp)" << std::endl;
    // }
    // else if (var_type == "double") {
    //     stream << "l.d " << reg << " " << var_offset << "($fp)" << std::endl;
    // }
}

inline void store_var_to_reg(std::ostream &stream, std::string var_type, int var_offset, std::string reg){
    if (var_type == "int") {
        stream << "sw " << reg << ", " << var_offset << "(fp)" << std::endl;
    }
    else if (var_type == "char") {
        stream << "sb " << reg << ", " << var_offset << "(fp)" << std::endl;
    }
    // else if (var_type == "float") {
    //     stream << "s.s " << reg << " " << var_offset << "($fp)" << std::endl;
    // }
    // else if (var_type == "double") {
    //     stream << "s.d " << reg << " " << var_offset << "($fp)" << std::endl;
    // }
}

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
