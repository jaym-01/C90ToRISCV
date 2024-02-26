
#include <map>
#include <iostream>
#include "vector"
#include "helpers.hpp"
#include <algorithm>

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

    ScopeContext(ScopeContext* parent) {
        var_map = {};
        for (auto var : parent->var_map) {
            var_map[var.first] = var.second;
            var_map[var.first].is_inherited = true;
        }
        child_scopes = {};
    };

    // Add a variable to the current scope
    // int AddVariable(std::string identifier, std::string type) {
    //     var_map[identifier] = { type: type, offset: 1 };
    // }

    void SetVarContext(std::string identifier, VariableContext context) {
        if (var_map.find(identifier) != var_map.end() && var_map[identifier].is_inherited == false)
            throw std::runtime_error("Error: variable " + identifier + " already exists");
        else if (var_map[identifier].is_inherited) {
            std::cout << "Replacing outerscope variable: " << identifier << std::endl;
            context.offset = 1; // TODO: Check if this is correct
            context.is_inherited = false;
        }

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
            std::cout<<var.first<<": ";
            var.second.Print();
            std::cout<<std::endl;
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

    std::vector<std::string> arg_registers_avail;
    std::vector<std::string> arg_registers_used;

    std::string return_label;
    int max_func_overflow = 0; // When calling functions, might have to store some args in memory

    FunctionContext(std::string id, std::string _return_label) {

        identifier = id;
        // root_scope = nullptr;
        total_var_size = 0;
        local_var_offset = -16;

        saved_registers_avail = {"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11"};
        saved_registers_used = {};

        return_label = _return_label;
        // arg_registers_avail = {"a7", "a6", "a5", "a4", "a3", "a2", "a1", "a0"};
        // arg_registers_used = {};
    }

    //    // Get a free register for temporary use
    // std::string ReserveTempRegister() {
    //     std::string reg = temp_registers_avail.back();
    //     temp_registers_avail.pop_back();
    //     temp_registers_used.push_back(reg);
    //     return reg;
    // }

    // // Free a register for temporary use
    // void FreeTempRegister(std::string &reg) {
    //     if (reg == "") {
    //         reg = "";
    //         return;
    //     }
    //     // std::cout<<"Freeing register: "<<reg<<"\n";
    //     temp_registers_avail.push_back(reg);
    //     temp_registers_used.erase(std::remove(temp_registers_used.begin(), temp_registers_used.end(), reg), temp_registers_used.end());
    //     reg = "";
    //     return;
    // }


    void SetRootScope(ScopeContext* scope) {
        root_scope = scope;
    }

    // int GetTotalVarSize() {
    //     return total_var_size;
    // }

    // void SetTotalVarSize(int size) {
    //     total_var_size = size;
    // }

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

struct FuncDefinition {
    std::vector<VariableContext> params;
    std::string return_type = "int";
};

struct SavedRegister {
    std::string reg;
    int offset;
};

class Context
{

public:

    // Root scope_context
    FunctionContext *f_context;
    std::map<std::string, FuncDefinition> id_to_func_def;


    std::vector<std::string> temp_registers_avail;
    std::vector<std::string> temp_registers_used;
    ScopeContext *cur_scope;
    ScopeContext* global_scope;
    std::string cur_contloop_label = "";
    std::string cur_breakloop_label = "";

    int label_id = 0;
    Context()
    {
        temp_registers_avail = {"t1", "t2", "t3", "t4", "t5", "t6"};
        temp_registers_used = {};
        global_scope = new ScopeContext();
    }

    ~Context() {
        delete f_context;
        delete global_scope;
    }

    void SetBreakContLabels(std::string break_label, std::string cont_label) {
        cur_breakloop_label = break_label;
        cur_contloop_label = cont_label;
    }

    std::string GetNewLabel(std::string label) {
        return label + "_" + std::to_string(label_id++);
    }

    void PrintAvailTempRegs() {
        std::cout<<"Avail temp registers: ";
        for (auto reg : temp_registers_avail) {
            std::cout<<reg + " ";
        }
        std::cout<<std::endl;
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

        // std::cout<<"Reserving register: "<<reg<<std::endl;
        temp_registers_avail.pop_back();
        temp_registers_used.push_back(reg);
        return reg;
    }

    // Free a register for temporary use
    void FreeTempRegister(std::string &reg) {
        if (reg == "") {
            reg = "";
            return;
        }
        // std::cout<<"Freeing register: "<<reg<<"\n";
        temp_registers_avail.push_back(reg);
        temp_registers_used.erase(std::remove(temp_registers_used.begin(), temp_registers_used.end(), reg), temp_registers_used.end());
        reg = "";
        return;
    }


    std::string GetFuncReturnLabel() {
        return f_context->return_label;
    }


    // // Manage func sizes
    // int GetFuncTotalVarSize() {
    //     return f_context->GetTotalVarSize();
    // }

    // void SetFuncTotalVarSize(int size) {
    //     f_context->SetTotalVarSize(size);
    // }
    void AddFuncDef(std::string id, FuncDefinition def) {
        id_to_func_def[id] = def;
    }

    int GetCurFuncOffset() {
        return f_context->GetLocalVarOffset();
    }

    void SetCurFuncOffset(int offset) {
        f_context->SetLocalVarOffset(offset);
        return;
    }

    int GetCurFuncMaxArgOverflow() {
        return f_context->max_func_overflow;
    }

    int CalcVarOffsetAndUpdate(VariableContext var) {
        int cur_func_offset = GetCurFuncOffset();
        int var_offset = calculate_var_offset(cur_func_offset, var);
        SetCurFuncOffset(var_offset);
        return var_offset;
    }

    int CalcOverflowOffsetAndUpdate(int &cur_offset, VariableContext var) {
        int aligned_offset = calculate_arg_overflow(cur_offset, var);
        if (cur_offset > f_context->max_func_overflow) {
            f_context->max_func_overflow = cur_offset;
        }

        return aligned_offset;
    }

    FuncDefinition GetFuncDef(std::string id) {
        if (id_to_func_def.find(id) == id_to_func_def.end())
            throw std::runtime_error("Error: function " + id + " not defined yet");

        return id_to_func_def[id];
    }

    std::vector<SavedRegister> SaveTempRegsToMemory(std::ostream &stream) {
        std::vector<SavedRegister> saved_regs = {};

        int cur_func_offset = GetCurFuncOffset();
        for (auto reg : temp_registers_used) {
            cur_func_offset = align_to_multiple_of_4(cur_func_offset - 4);
            stream << "sw " << reg << ", " << cur_func_offset << "(sp)" << std::endl;

            saved_regs.push_back({
                .reg = reg,
                .offset = cur_func_offset
            });
        }
        SetCurFuncOffset(cur_func_offset);
        return saved_regs;
    }

    void RestoreRegistersFromMem(std::ostream &stream, std::vector<SavedRegister> saved_regs)
    {
        for (auto reg : saved_regs) {
            stream << "lw " << reg.reg << ", " << reg.offset << "(sp)" << std::endl;
        }
    }
};

#endif
