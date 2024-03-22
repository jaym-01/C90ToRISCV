#include "../context/ast_context.hpp"
#include <unordered_map>

inline std::unordered_map<std::string, int> types_l = {
    {"int", 1},
    {"char", 1},
    {"unsigned", 1},
    {"float", 1},
    {"double", 1},
    {"void", 1},
    {"struct", 1},
};

inline std::string resolve_type(std::string type, ScopeContext *scope) {
    if(types_l.find(type) != types_l.end()){
        return type;
    } else {
        TypeDefContext tmp_type = scope->GetTypeDef(type);
        return resolve_type(tmp_type.type, scope);
    }
}
