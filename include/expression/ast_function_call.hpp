
#ifndef AST_FUNCTION_CALL_HPP
#define AST_FUNCTION_CALL_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"
#include "../helpers/memory_helpers.hpp"

class FunctionCall : public Node
{
private:
    Node *identifier_;
    NodeList *argument_expression_list_;

public:
    FunctionCall(
        Node *identifier,
        NodeList *argument_expression_list) : identifier_(identifier),
                                              argument_expression_list_(argument_expression_list){};

    ~FunctionCall()
    {
        delete identifier_;
        delete argument_expression_list_;
    };

    void EmitCallFunction(std::ostream &stream) const {
        stream << "call " << identifier_->GetIdentifier() << std::endl;
    }

    void EmitRISCWithDest(std::ostream &stream, Context &context, std::string &dest) const override{
        // std::cout<<"Emitting RISC for function call: "<<std::endl;
        // Print(std::cout);
        // std::cout<<std::endl;

        // If function has no args
        std::string id = identifier_->GetIdentifier();
        if (argument_expression_list_ == nullptr) {
            EmitCallFunction(stream);
            return;
        }

        // If function has args
        std::vector<Node*> args = argument_expression_list_->GetNodes();
        int cur_sp_offset = 0, cur_a_reg = 0, cur_fp_reg = 0;
        FuncDefinition func_def = context.GetFuncDef(id);

        std::vector<VariableContext> params = func_def.params;
        std::string type, dest_reg;
        for (int i = 0; i < args.size(); i++)
        {
            type = params[i].type;
            if ((type == "int" || type == "char") && cur_a_reg <= 7) {
                dest_reg = "a" + std::to_string(cur_a_reg);
                args[i]->DefineConstantType(params[i].type);
                args[i]->EmitRISCWithDest(stream, context, dest_reg);
                cur_a_reg++;

            } else if((type == "float" || type == "double") && cur_fp_reg <= 7){
                dest_reg = "fa" + std::to_string(cur_fp_reg);
                args[i]->DefineConstantType(params[i].type);
                args[i]->EmitRISCWithDest(stream, context, dest_reg);
                cur_fp_reg++;
            } else {
                std::string temp_reg = "";
                args[i]->DefineConstantType(params[i].type);
                args[i]->EmitRISCWithDest(stream, context, temp_reg);
                // increase the stack size for the param
                int aligned_offset = context.CalcOverflowOffsetAndUpdate(cur_sp_offset, params[i]);

                // std::string ins;
                // if(params[i].type == "int") ins = "sw";
                // else if(params[i].type == "char") ins = "sb";
                // else if(params[i].type == "float") ins = "fsw";
                // else if(params[i].type == "double") ins = "fsd";
                // function arguements below the current stack
                stream << get_mem_write(type) << " " << temp_reg << ", " << aligned_offset << "(sp)" << std::endl;
                context.FreeRegister(temp_reg);
            }
        }

        // Call function
        // Save temp regs to memory
        std::vector<SavedRegister> saved_regs = context.SaveTempRegsToMemory(stream);
        EmitCallFunction(stream);
        context.RestoreRegistersFromMem(stream, saved_regs);

        // do you need to store the result in a register if a register has not been passed through?
        // ^ shows that the return value is not needed?
        if (dest == "") {
            dest = context.ReserveRegister(func_def.return_type);
        }

        // return value of the function stored in reg
        stream<<"add "<<dest<<", a0, "<<"zero"<<std::endl;

        // TODO: load in a1 if double is used
    };

    void Print(std::ostream &stream) const
    {
        identifier_->Print(stream);
        std::cout << "(";
        if (argument_expression_list_ != nullptr)
            argument_expression_list_->Print(stream);
        std::cout << ")";
    };

    void DefineConstantType(std::string type) override {}

    int GetNumBranches() const override { return 1; };
};

#endif
