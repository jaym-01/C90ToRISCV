// Can't be imported in context, ast_node or ast_variable_context
#ifndef VAR_HELPERS_HPP
#define VAR_HELPERS_HPP

#include "context/ast_variable_context.hpp"
#include "ast_node.hpp"
#include "context/ast_context.hpp"
#include "memory_helpers.hpp"

inline std::map<std::string, int> type_to_shift_amt = {
    // can also do log2(type_size)
    {"int", 2},
    {"unsigned", 2},
    {"char", 0},
    {"float", 2},
    // TODO: check if double is right?
    {"double", 3},
};


// GLOBAL VAR READ FUNCTIONS
inline void global_var_to_reg(Context &context, VariableContext var, std::string id, std::ostream &stream, std::string dest_reg) {
    std::string ins;

    std::string addr_reg = context.ReserveRegister("int");

    // if (var.type == "int") ins = "lw";
    // else if(var.type == "float") ins ="flw";
    // else if(var.type == "double") ins = "fld";
    // else if(var.type == "char") ins = "lbu";

    stream << "lui " << addr_reg << ", %hi(" << id << ")" << std::endl;
    // TODO: check this
    stream << get_mem_read(var.type, var.is_pntr) << " " << dest_reg << ", %lo(" << id << ")(" << addr_reg << ")" << std::endl;

    context.FreeRegister(addr_reg);
}

inline void global_arr_elem_to_reg(Node* index_expr, Context &context, VariableContext var, std::string id, std::ostream &stream, std::string dest_reg, std::string index_reg) {
    // "lui a5, %hi(y)"
    // "addi a5, a5, %lo(y)" // addr of y
    // "add a5, a5, index_reg"
    // "lw dest_reg, 0(a5)"

    // getting the address of the array
    if(index_expr == nullptr){
        stream << "lui " << dest_reg << ", %hi(" << id << ")" << std::endl;
        stream << "addi " << dest_reg << ", " << dest_reg << ", %lo(" << id << ")" << std::endl;
    } else{

        std::string addr_reg = context.ReserveRegister("int");

        stream << "slli " << index_reg << ", " << index_reg << ", "<<type_to_shift_amt[var.type] << std::endl;
        stream << "lui " << addr_reg << ", %hi(" << id << ")" << std::endl;
        stream << "addi " << addr_reg << ", " << addr_reg << ", %lo(" << id << ")" << std::endl;

        // if pointer - load the address it's pointing to
        if(var.is_pntr) stream << "lw " << addr_reg << ", 0(" << addr_reg << ")" << std::endl;

        // adds the index offset
        stream << "add " << addr_reg<< ", " << addr_reg << ", " << index_reg << std::endl;

        stream << get_mem_read(var.type, var.is_pntr) << " " << dest_reg << ", 0(" << addr_reg << ")" << std::endl;

        context.FreeRegister(addr_reg);
    }
}

inline void read_global_var(
    const Node* var_node, Context &context,
    VariableContext var, std::string id,
    std::ostream &stream, std::string &dest_reg
) {
    std::string index_reg = "";
    Node* index_expr = var_node->GetIndexExpression();

    if (var.is_array || (var.is_pntr && index_expr != nullptr)) {
        if(index_expr != nullptr) index_expr->EmitRISCWithDest(stream, context, index_reg);
        global_arr_elem_to_reg(index_expr, context, var, id, stream, dest_reg, index_reg);
        context.FreeRegister(index_reg);
    } else {
        global_var_to_reg(context, var, id, stream, dest_reg);
    }
}

// GLOBAL VAR WRITE FUNCTIONS
inline void reg_to_global_var(Context &context, VariableContext var, std::string id, std::ostream &stream, std::string val_reg) {
    std::string addr_reg = context.ReserveRegister("int");

    stream << "lui " << addr_reg << ", %hi(" << id << ")" << std::endl;
    // TODO: Check this
    stream << get_mem_write(var.type, var.is_pntr) << " " << val_reg << ", %lo(" << id << ")(" << addr_reg << ")" << std::endl;

    context.FreeRegister(addr_reg);
}

inline void reg_to_global_array_mem(Context &context, VariableContext var, std::string id,
    std::ostream &stream, std::string val_reg, std::string index_reg)
{
    // "lui a5, %hi(y)"
    // "addi a5, a5, %lo(y)" // addr of y
    // "add a5, a5, index_reg"
    // "sw val_reg, 0(a5)"
    std::string addr_reg = context.ReserveRegister("int");
    stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
    stream << "lui " << addr_reg << ", %hi(" << id << ")" << std::endl;
    stream << "addi " << addr_reg <<", "<<addr_reg<< ", %lo(" << id << ")" << std::endl;

    // adds the index offset
    stream << "add " << addr_reg << ", " << addr_reg << ", " << index_reg << std::endl;

    // TODO: check this
    stream << get_mem_write(var.type, var.is_pntr) << " " << val_reg << ", 0(" << addr_reg << ")" << std::endl;

    context.FreeRegister(addr_reg);
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
        context.FreeRegister(index_reg);
    } else
    {
        reg_to_global_var(context, var, id, stream, val_reg);
    }
}

// writes the memory address of the element to be accessed in the array into index_reg
inline void get_array_address(Context &context, std::ostream &stream, VariableContext &var, std::string &index_reg){
    // Store index offset + var offset + fp in index reg (location of array elem in memory)
    stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
    // if param, get load address of first element then find the offset
    if(var.is_param || var.is_pntr) {
        // tmp holds the first element of the array address to the array
        std::string tmp = context.ReserveRegister("int");
        stream << "lw " << tmp << ", "<< var.offset << "(fp)" << std::endl;
        // add it to index_reg so index_reg stores memory address of value to be accessed
        stream << "add " << index_reg << ", " << index_reg << ", " << tmp << std::endl;
        context.FreeRegister(tmp);
    }
    else {
        stream << "addi " << index_reg << ", " << index_reg << "," << var.offset << std::endl;
        stream << "add " << index_reg << ", " << index_reg << ", fp" << std::endl;
    }
}


// LOCAL VAR READ WRITE FUNCTIONS
inline void local_var_to_reg(std::ostream &stream, VariableContext var, int offset, std::string dest_reg, std::string addr_reg = "fp") {
    stream << get_mem_read(var.type, var.is_pntr) << " " << dest_reg << ", " << offset << "(" << addr_reg << ")" << std::endl;
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
    // this could be bad is trying to dereference point?
    // TODO: CHECK THIS IS OK
    if ((var.is_array || var.is_pntr) && var_node->IsDereference()) {
        // Get index offset and store in index reg
        // std::cout << "here" << std::endl;
        std::string index_reg = "";
        var_node->GetIndexExpression()->EmitRISCWithDest(stream, context, index_reg);

        // // Store index offset + var offset + fp in index reg (location of array elem in memory)
        // stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
        // // if param, get load address of first element then find the offset
        // if(var.is_param) {
        //     // tmp holds the first element of the array address to the array
        //     std::string tmp = context.ReserveRegister("int");
        //     stream << "lw " << tmp << ", "<< var.offset << "(fp)" << std::endl;

        //     // add it to index_reg so index_reg stores memory address of value to be accessed
        //     stream << "add " << index_reg << ", " << index_reg << ", " << tmp << std::endl;
        //     context.FreeRegister(tmp);
        // }
        // else {
        //     stream << "addi " << index_reg << ", " << index_reg << "," << var.offset << std::endl;
        //     stream << "add " << index_reg << ", " << index_reg << ", fp" << std::endl;
        // }
        get_array_address(context, stream, var, index_reg);
        local_var_to_reg(stream, var, 0, dest_reg, index_reg);
        context.FreeRegister(index_reg);
    } else if(var.is_array){
        // array is not being dereferenced here
        stream << "addi " << dest_reg << ", fp, " << var.offset << std::endl;
    } else {
        local_var_to_reg(stream, var, var.offset, dest_reg, "fp");
    }
}

inline void reg_to_local_var(std::ostream &stream, VariableContext var, int var_offset, std::string val_reg, std::string addr_reg = "fp") {
    stream << get_mem_write(var.type, var.is_pntr) << " " << val_reg << ", " << var_offset << "(" << addr_reg << ")" << std::endl;
}

inline void write_local_var(Node *var_node, Context &context, std::ostream &stream, VariableContext var, std::string val_reg) {
    if ((var.is_array || var.is_pntr) && var_node->GetIndexExpression() != nullptr)
    {
        std::string index_reg = "";
        Node *index_expr = var_node->GetIndexExpression();
        index_expr->EmitRISCWithDest(stream, context, index_reg);

        // Store index offset + var offset + fp in index reg (location of array elem in memory)
        // stream << "slli " << index_reg << ", " << index_reg << ", " << type_to_shift_amt[var.type] << std::endl;
        // stream << "addi " << index_reg << ", " << index_reg << "," << var.offset << std::endl;
        // stream << "add " << index_reg << ", " << index_reg << ", fp" << std::endl;
        get_array_address(context, stream, var, index_reg);
        reg_to_local_var(stream, var, 0, val_reg, index_reg);

        context.FreeRegister(index_reg);
    } else if(var.is_pntr && var_node->IsDereference()) {
        // check if writing to dereference pointer
        // must load address is register and write to that address
        std::string addr_reg = context.ReserveRegister("int");
        // stream << "here" << std::endl;
        stream << "lw " << addr_reg << ", " << var.offset << "(fp)" << std::endl;
        reg_to_local_var(stream, var, 0, val_reg, addr_reg);
        context.FreeRegister(addr_reg);
    } else {

        // Save variable to memory
        reg_to_local_var(stream, var, var.offset, val_reg);
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
