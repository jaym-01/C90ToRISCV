// Can't be imported in context, ast_node or ast_variable_context
#ifndef VAR_HELPERS_HPP
#define VAR_HELPERS_HPP

#include "context/ast_variable_context.hpp"
#include "ast_node.hpp"
#include "context/ast_context.hpp"

inline std::map<std::string, int> type_to_shift_amt = {
    // can also do log2(type_size)
    {"int", 2},
    {"char", 0},
    {"float", 2},
};


// GLOBAL VAR READ FUNCTIONS
inline void global_var_to_reg(VariableContext var, std::string id, std::ostream &stream, std::string dest_reg) {
    if (var.type == "int") {
        stream << "lui " << dest_reg << ", %hi(" << id << ")" << std::endl;
        stream << "lw " << dest_reg << ", %lo(" << id << ")(" << dest_reg << ")" << std::endl;
    }
}

inline void global_arr_elem_to_reg(Context &context, VariableContext var, std::string id, std::ostream &stream, std::string dest_reg, std::string index_reg) {
    // "lui a5, %hi(y)"
    // "addi a5, a5, %lo(y)" // addr of y
    // "add a5, a5, index_reg"
    // "lw dest_reg, 0(a5)"

    stream << "slli " << index_reg << ", " << index_reg << ", "<<type_to_shift_amt[var.type] << std::endl;
    std::string addr_reg = context.ReserveTempRegister();
    if (var.type == "int") {
        stream << "lui " << addr_reg << ", %hi(" << id << ")" << std::endl;
        stream << "addi " << addr_reg << ", " << addr_reg << ", %lo(" << id << ")" << std::endl;
        stream << "add " << addr_reg<< ", " << addr_reg << ", " << index_reg << std::endl;
        stream << "lw " << dest_reg << ", 0(" << addr_reg << ")" << std::endl;
    }
    context.FreeTempRegister(addr_reg);
}

inline void read_global_var(
    const Node* var_node, Context &context,
    VariableContext var, std::string id,
    std::ostream &stream, std::string &dest_reg
) {
    std::string index_reg = "";
    if (var.is_array) {
        var_node->GetIndexExpression()->EmitRISCWithDest(stream, context, index_reg);
        global_arr_elem_to_reg(context, var, id, stream, dest_reg, index_reg);
        context.FreeTempRegister(index_reg);
    } else {
        global_var_to_reg(var, id, stream, dest_reg);
    }
}

// GLOBAL VAR WRITE FUNCTIONS
inline void reg_to_global_var(Context &context, VariableContext var, std::string id, std::ostream &stream,
    std::string val_reg) {
    std::string addr_reg = context.ReserveTempRegister();

    if (var.type == "int") {
        stream << "lui " << addr_reg << ", %hi(" << id << ")" << std::endl;
        stream << "sw " << val_reg << ", %lo(" << id << ")(" << addr_reg << ")" << std::endl;
    }

    context.FreeTempRegister(addr_reg);
}

inline void reg_to_global_array_mem(Context &context, VariableContext var, std::string id,
    std::ostream &stream, std::string val_reg, std::string index_reg)
{
    // "lui a5, %hi(y)"
    // "addi a5, a5, %lo(y)" // addr of y
    // "add a5, a5, index_reg"
    // "sw val_reg, 0(a5)"
    stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
    std::string addr_reg = context.ReserveTempRegister();
    if (var.type == "int")
    {
        stream << "lui " << addr_reg << ", %hi(" << id << ")" << std::endl;
        stream << "addi " << addr_reg <<", "<<addr_reg<< ", %lo(" << id << ")" << std::endl;
        stream << "add " << addr_reg << ", " << addr_reg << ", " << index_reg << std::endl;
        stream << "sw " << val_reg << ", 0(" << addr_reg << ")" << std::endl;
    }

    context.FreeTempRegister(addr_reg);
}

inline void write_global_var(
    const Node *var_node,
    Context &context,
    VariableContext var,
    std::string id,
    std::ostream &stream,
    std::string val_reg)
{
    std::string index_reg = "";
    if (var.is_array)
    {
        var_node->GetIndexExpression()->EmitRISCWithDest(stream, context, index_reg);
        reg_to_global_array_mem(context, var, id, stream, val_reg, index_reg);
        context.FreeTempRegister(index_reg);
    } else
    {
        reg_to_global_var(context, var, id, stream, val_reg);
    }
}


// LOCAL VAR READ WRITE FUNCTIONS
inline void local_var_to_reg(std::ostream &stream, VariableContext var, int offset,
        std::string dest_reg, std::string addr_reg = "fp") {

    if (var.type == "int") {
        stream << "lw " << dest_reg << ", " << offset << "(" << addr_reg << ")" << std::endl;
    }
    else if (var.type == "char"){
        stream << "lbu" << dest_reg << ", " << offset << "(" << addr_reg << ")" << std::endl;
    }
}

inline void read_local_var(
    const Node *var_node, // won't modify var_node
    Context &context,
    std::ostream &stream, VariableContext var,
    std::string dest_reg)
{
    if (var.offset > 0) {
        std::string err_msg = "Variable was not initialized";
        throw std::runtime_error(err_msg);
    }

    int offset;

    if (var.is_array) {
        // Get index offset and store in index reg
        std::string index_reg = "";
        var_node->GetIndexExpression()->EmitRISCWithDest(stream, context, index_reg);

        // Store index offset + var offset + fp in index reg (location of array elem in memory)
        stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
        stream << "addi " << index_reg << ", " << index_reg << "," << var.offset << std::endl;
        stream << "add " << index_reg << ", " << index_reg << ", fp" << std::endl;
        local_var_to_reg(stream, var, 0, dest_reg, index_reg);
        context.FreeTempRegister(index_reg);
    } else {
        local_var_to_reg(stream, var, var.offset, dest_reg, "fp");
    }
}

inline void reg_to_local_var(
    std::ostream &stream, std::string var_type, int var_offset,
    std::string val_reg, std::string addr_reg = "fp") {
    if (var_type == "int") {
        stream << "sw " << val_reg << ", " << var_offset << "(" << addr_reg << ")" << std::endl;
    }
    else if (var_type == "char")
    {
        stream << "sb " << val_reg << ", " << var_offset << "(" << addr_reg << ")" << std::endl;
    }
}

inline void write_local_var(
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
        reg_to_local_var(stream, var.type, 0, val_reg, index_reg);

        context.FreeTempRegister(index_reg);
    } else {

        // Save variable to memory
        reg_to_local_var(stream, var.type, var.offset, val_reg);
    }
}


// TOP READ FUNCTION
inline void read_var_value(
    const Node *var_node,
    Context &context,
    std::ostream &stream, VariableContext var,
    std::string &dest_reg)
{
    if (var.is_global)
    {
        read_global_var(var_node, context, var, var.id, stream, dest_reg);
    }
    else
    {
        read_local_var(var_node, context, stream, var, dest_reg);
    }
}

// TOP WRITE FUNCTION
inline void write_var_value(
    Node *var_node,
    Context &context,
    std::ostream &stream,
    VariableContext var,
    std::string val_reg)
{
    if (var.is_global){
        write_global_var(var_node, context, var, var.id, stream, val_reg);
    }
    else{
        write_local_var(var_node, context, stream, var, val_reg);
    }
}
#endif
