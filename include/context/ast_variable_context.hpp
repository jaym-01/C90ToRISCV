
#include <map>
#include <iostream>
#include "vector"
#include "string.h"

#ifndef AST_VARIABLE_CONTEXT_HPP
#define AST_VARIABLE_CONTEXT_HPP

inline std::map<std::string, int> type_size_ = {
    {"int", 4},
    {"unsigned", 4},
    {"char", 1},
    {"float", 4},
    {"double", 8}
};

struct VariableContext
{
    // std::string storage_class;
    // general
    std::string id;
    std::string type;

    // different parts can be seperated
    // but polymorphism needs pointers
    // change all other places to pointer to seperate

    // arrays
    int array_size;
    bool is_array;

    // scopes
    bool is_inherited = false;
    bool is_global = false;
    bool is_param = false;

    // enum
    bool is_enum = false;
    int enum_val = 0;

    // pointers
    bool is_pntr = false;
    int pntr_depth = 0;
    int working_pntr_depth = 0;

    // memory
    int offset = 1;

    // struct
    // must be a vector to preserve order
    std::vector<VariableContext> members;
    std::string struct_name;

    std::string GetType() const {
        if(is_pntr || (is_array && is_param) || is_enum) return "int";
        else return type;
    }


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

    std::vector<VariableContext> GetStructMembers() {
        if(type != "struct") throw std::runtime_error("Error: this is not a struct, GetStructMembers is not implemented");
        else return members;
    }

    int GetSize() const {
        if(type == "struct") {
            // std::cout << "in here" << std::endl;
            int size = 0;
            for(auto const& member : members) {
                // std::cout << "this is the size: " << member.GetSize() << std::endl;
                size += member.GetSize();
            }
            return size;
        }
        else if(is_array && !is_param) return array_size * type_size_[type];
        else return type_size_[GetType()];
    }

    // performs linear search
    // vector used to maintain order
    VariableContext GetMemberById(std::string id) {
        if(type == "struct") {
            for(auto const& member : members) {
                if(member.id == id) return member;
            }
            throw std::runtime_error("Error: member not found");
        } else throw std::runtime_error("Error: this is not a struct, GetMemberById is not implemented");
    }

    std::string GetStructName() {
        if(type != "struct") throw std::runtime_error("Error: this is not a struct, GetStructName is not implemented");
        else return struct_name;
    }
};


// struct StructContext : public VariableContext {
//     std::map<std::string, VariableContext> members;
//     std::string struct_name;

//     std::map<std::string, VariableContext> GetStructMembers() override {
//         return members;
//     }

//     int GetSize() const override {
//         std::cout << "in here" << std::endl;
//         int size = 0;
//         for(auto const& member : members) {
//             std::cout << "this is the size: " << member.second.GetSize() << std::endl;
//             size += member.second.GetSize();
//         }
//         return size;
//     }

//     std::string GetStructName() {
//         return struct_name;
//     }
// };


#endif
