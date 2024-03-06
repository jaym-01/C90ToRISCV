#ifndef MEM_HELPERS_HPP
#define MEM_HELPERS_HPP

#include <string>

// brings the offset to the start of the word
inline int align_word(int offset){
    int rem = (offset)%4;

    if(rem == 0) return offset;
    else return offset - rem - 4;
}

inline std::string get_mem_write(std::string type){
    if(type == "int") return "sw";
    else if(type == "char") return "sb";
    else if(type == "float") return "fsw";
    else if(type == "double") return "fsd";
    else throw std::runtime_error("invalid type loaded from memory");
}

inline std::string get_mem_read(std::string type){
    if(type == "int") return "lw";
    else if(type == "char") return "lbu";
    else if(type == "float") return "flw";
    else if(type == "double") return "fld";
    else throw std::runtime_error("invalid type loaded from memory");
}

#endif
