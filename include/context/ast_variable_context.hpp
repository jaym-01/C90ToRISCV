
#include <map>
#include <iostream>
#include "vector"
#include "string.h"

#ifndef AST_VARIABLE_CONTEXT_HPP
#define AST_VARIABLE_CONTEXT_HPP

struct VariableContext
{
    // std::string storage_class;
    std::string id;
    std::string type;
    int array_size;
    bool is_array;
    bool is_inherited = false;
    bool is_global = false;
    int offset = 1;


    void Print() {
        std::cout<<"id: "<<id<<", ";
        std::cout<<"type: "<<type<<", ";
        std::cout<<"array_size: "<<array_size<<", ";
        std::cout<<"is_array: "<<is_array<<", ";
        std::cout<<"is_inherited: "<<is_inherited<<", ";
        std::cout<<"is_global: "<<is_global<<", ";
        std::cout<<"offset: "<<offset;
    }

    std::string ToString() {
        std::string str = "id: " + id + ", ";
        str += "type: " + type + ", ";
        str += "array_size: " + std::to_string(array_size) + ", ";
        str += "is_array: " + std::to_string(is_array) + ", ";
        str += "is_inherited: " + std::to_string(is_inherited) + ", ";
        str += "is_global: " + std::to_string(is_global) + ", ";
        str += "offset: " + std::to_string(offset);
        return str;
    }
    // // Constructor for the Person struct
    // VariableContext(VariableContextParams p)
    //     :   type(p.type), array_size(p.array_size), is_array(p.is_array),
    //         is_inherited(p.is_inherited), is_global(p.is_global), offset(p.offset) {}
};


#endif
