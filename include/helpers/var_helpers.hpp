// Can't be imported in context, ast_node or ast_variable_context
#ifndef VAR_HELPERS_HPP
#define VAR_HELPERS_HPP

#include "context/ast_variable_context.hpp"
#include "ast_node.hpp"
#include "context/ast_context.hpp"

inline void var_mem_to_reg(std::ostream &stream, std::string var_type, int var_offset,
                            std::string dest_reg, std::string addr_reg = "fp") {

    if (var_type == "int") {
        stream << "lw " << dest_reg << ", " << var_offset << "(" << addr_reg << ")" << std::endl;
    }
    else if (var_type == "char"){
        stream << "lbu" << dest_reg << ", " << var_offset << "(" << addr_reg << ")" << std::endl;
    }
    // else if (var_type == "float") {
    // }
    // else if (var_type == "double") {
    // }
}

inline void read_var_value(
    const Node *var_node, // won't modify var_node
    Context &context,
    std::ostream &stream, VariableContext var,
    std::string dest_reg)
{
    int offset;
    std::map<std::string, int> type_to_shift_amt = {
        // can also do log2(type_size)
        {"int", 2},
        {"char", 0},
        {"float", 2},
    };

    if (var.is_array) {
        // Get index offset and store in index reg
        std::string index_reg = "";
        var_node->GetIndexExpression()->EmitRISCWithDest(stream, context, index_reg);

        // Store index offset + var offset + fp in index reg (location of array elem in memory)
        stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
        stream << "addi " << index_reg << ", " << index_reg << "," << var.offset << std::endl;
        stream << "add " << index_reg << ", " << index_reg << ", fp" << std::endl;
        var_mem_to_reg(stream, var.type, 0, dest_reg, index_reg);
    } else {
        var_mem_to_reg(stream, var.type, var.offset, dest_reg, "fp");
    }
}

inline void reg_to_var_mem(
    std::ostream &stream, std::string var_type, int var_offset,
    std::string val_reg, std::string addr_reg = "fp") {
    if (var_type == "int") {
        stream << "sw " << val_reg << ", " << var_offset << "(" << addr_reg << ")" << std::endl;
    }
    else if (var_type == "char")
    {
        stream << "sb " << val_reg << ", " << var_offset << "(" << addr_reg << ")" << std::endl;
    }
    // else if (var_type == "float") {
    //     stream << "s.s " << reg << " " << var_offset << "($fp)" << std::endl;
    // }
    // else if (var_type == "double") {
    //     stream << "s.d " << reg << " " << var_offset << "($fp)" << std::endl;
    // }
}

inline void set_var_value(
    Node *var_node,
    Context &context,
    std::ostream &stream, VariableContext var,
    std::string val_reg) {

    int offset;
    std::map<std::string, int> type_to_shift_amt = {
        // can also do log2(type_size)
        {"int", 2},
        {"char", 0},
        {"float", 2},
    };

    if (var.is_array)
    {
        std::string index_reg = "";
        Node *index_expr = var_node->GetIndexExpression();
        index_expr->EmitRISCWithDest(stream, context, index_reg);

        // Store index offset + var offset + fp in index reg (location of array elem in memory)
        stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
        stream << "addi " << index_reg << ", " << index_reg << "," << var.offset << std::endl;
        stream << "add " << index_reg << ", " << index_reg << ", fp" << std::endl;
        reg_to_var_mem(stream, var.type, 0, val_reg, index_reg);

        context.FreeTempRegister(index_reg);
    } else {

        // Save variable to memory
        reg_to_var_mem(stream, var.type, var.offset, val_reg);
    }
}


#endif
