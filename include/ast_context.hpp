
#include <map>
#include <iostream>
// #include <string.h>
// #include <vector>
#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP

// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).

// struct VariableContext {
//     std::string type;
//     int offset;
// };

struct ScopeContext {

public:
    std::map<std::string, std::string> var_map;
    std::vector<ScopeContext*> child_scopes;

    ScopeContext() {
        var_map = {};
        child_scopes = {};
    };
};

struct FunctionContext {
private:
    std::string identifier_;

public:
    ScopeContext* root_scope;
    int local_var_ptr;

    FunctionContext(std::string identifier) {
        identifier_ = identifier;
        root_scope = nullptr;
        local_var_ptr = -16;
    }

    ~FunctionContext() {
        if (root_scope != nullptr)
            delete root_scope;
    }

    void SetRootScope(ScopeContext* scope) {
        root_scope = scope;
    }
};

class Context
{
private:
    FunctionContext* f_context_;

    // Vector for function definitions
    std::map<std::string, FunctionContext*> id2function_;

public:
    Context() {}

    ~Context() {
        delete f_context_;
    }

    void InitFunctionContext(FunctionContext* f_context) {
        f_context_ = f_context;
    }

    int GetNumberOfScopes() {
        // return f_context_->scope_stack.size();
    }
};

#endif
