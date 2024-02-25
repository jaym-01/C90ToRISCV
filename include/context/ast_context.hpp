
#include <map>
#include <iostream>
#include "vector"
#include "helpers.hpp"

#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP

#include "ast_variable_context.hpp"
// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).

class ScopeContext {
private:

public:
    std::map<std::string, VariableContext> var_map;
    std::vector<ScopeContext*> child_scopes;

    ScopeContext() {
        var_map = {};
        child_scopes = {};
    };

    // Add a variable to the current scope
    // int AddVariable(std::string identifier, std::string type) {
    //     var_map[identifier] = { type: type, offset: 1 };
    // }

    void SetVarContext(std::string identifier, VariableContext context) {
        if (var_map.find(identifier) != var_map.end())
            throw std::runtime_error("Error: variable " + identifier + " already exists");

        var_map[identifier] = context;
    }

    void AddChildScope(ScopeContext* scope) {
        child_scopes.push_back(scope);
    }

    std::string GetVarType(std::string identifier) {
        return var_map[identifier].type;
    }

    void SetVarOffset(std::string identifier, int offset) {
        var_map[identifier].offset = offset;
    }

    int GetVarOffset(std::string identifier) {
        return var_map[identifier].offset;
    }

    VariableContext GetVarFromId(std::string identifier)
    {
        if (var_map.find(identifier) == var_map.end())
            throw std::runtime_error("Error: variable " + identifier + " not found in scope");

        return var_map[identifier];
    }

    void PrintTree(int level)
    {
        std::cout<<"Context Level "<<level<<std::endl;

        for (auto var : var_map) {
            for (int i = 0; i < level; i++) {
                std::cout<<"  ";
            }
            std::cout<<var.first<<":{ type: "<<var.second.type<<", offset: "<<var.second.offset;
            std::cout<<", array_size: "<<var.second.array_size<<", is_array: "<<var.second.is_array<<" }"<<std::endl;
        }

        for (auto scope : child_scopes) {
            scope->PrintTree(level + 1);
        }
    }

    ~ScopeContext() {
        for (auto scope : child_scopes) {
            delete scope;
        }
    }
};

class FunctionContext {

public:
    std::string identifier;
    int total_var_size; // num variables * 4 | 8?
    int local_var_offset; // used when emitting RISC and storing variables to memory
    ScopeContext* root_scope;
    std::vector<std::string> saved_registers_avail;
    std::vector<std::string> saved_registers_used;


    FunctionContext(std::string id) {

        identifier = id;
        // root_scope = nullptr;
        total_var_size = 0;
        local_var_offset = -16;

        saved_registers_avail = {"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11"};
        saved_registers_used = {};

    }

    void SetRootScope(ScopeContext* scope) {
        root_scope = scope;
    }

    int GetTotalVarSize() {
        return total_var_size;
    }

    void SetTotalVarSize(int size) {
        total_var_size = size;
    }

    int GetLocalVarOffset() {
        return local_var_offset;
    }

    void SetLocalVarOffset(int offset) {
        local_var_offset = offset;
    }

    ~FunctionContext()
    {
        if (root_scope != nullptr)
            delete root_scope;
    }
};

class Context
{

public:
    FunctionContext *f_context;
    std::map<std::string, FunctionContext *> id2function;
    std::vector<std::string> temp_registers_avail;
    std::vector<std::string> temp_registers_used;
    ScopeContext *cur_scope;

    Context()
    {
        temp_registers_avail = {"t1", "t2", "t3", "t4", "t5", "t6"};
        temp_registers_used = {};
    }

    ~Context() {
        delete f_context;
    }

    std::string GetReturnRegister()
    {
        return "a0";
    }

    ScopeContext *GetCurScope()
    {
        return cur_scope;
    }

    void SetCurScope(ScopeContext *scope)
    {
        cur_scope = scope;
    }

    void InitFunctionContext(FunctionContext* context) {
        f_context = context;
    }

    void InitRootScope(ScopeContext* scope) {
        f_context->SetRootScope(scope);
    }

    // Get a free register for temporary use
    std::string ReserveTempRegister() {
        std::string reg = temp_registers_avail.back();
        temp_registers_avail.pop_back();
        temp_registers_used.push_back(reg);
        return reg;
    }

    // Free a register for temporary use
    void FreeTempRegister(std::string reg) {
        // std::cout<<"Freeing register: "<<reg<<"\n";
        temp_registers_avail.push_back(reg);
        temp_registers_used.erase(std::remove(temp_registers_used.begin(), temp_registers_used.end(), reg), temp_registers_used.end());
    }


    // Manage func sizes
    int GetFuncTotalVarSize() {
        return f_context->GetTotalVarSize();
    }

    void SetFuncTotalVarSize(int size) {
        f_context->SetTotalVarSize(size);
    }

    int GetCurFuncOffset() {
        return f_context->GetLocalVarOffset();
    }

    void SetCurFuncOffset(int offset) {
        f_context->SetLocalVarOffset(offset);
        return;
    }
};

#endif
