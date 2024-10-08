
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
    std::string return_type_;

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
        for (std::size_t i = 0; i < args.size(); i++)
        {
            type = params[i].GetType();
            if(params[i].is_array){
                // load the memory address of the array into a register
                // store it in memory if not enough 'a' registers
                if(cur_a_reg <= 7) dest_reg = "a" + std::to_string(cur_a_reg);
                else dest_reg = context.ReserveRegister("int");
                stream << "addi " << dest_reg << ",fp," << params[i].offset << std::endl;
                if(cur_a_reg <= 7) cur_a_reg++;
                else {
                    // stores the address of the array in memory
                    stream << "sw " << dest_reg << ", " << context.CalcOverflowOffsetAndUpdate(cur_sp_offset, params[i]) << "(sp)" << std::endl;
                    context.FreeRegister(dest_reg);
                }

            } else if ((type == "int" || type == "char" || type == "unsigned") && cur_a_reg <= 7 && !params[i].is_array) {
                dest_reg = "a" + std::to_string(cur_a_reg);
                args[i]->DefineConstantType(type);
                args[i]->EmitRISCWithDest(stream, context, dest_reg);
                cur_a_reg++;

            } else if((type == "float" || type == "double") && cur_fp_reg <= 7 && !params[i].is_array){
                dest_reg = "fa" + std::to_string(cur_fp_reg);
                args[i]->DefineConstantType(type);
                args[i]->EmitRISCWithDest(stream, context, dest_reg);
                cur_fp_reg++;
            } else {
                std::string temp_reg = "";
                args[i]->DefineConstantType(type);
                args[i]->EmitRISCWithDest(stream, context, temp_reg);
                // increase the stack size for the param
                int aligned_offset = context.CalcOverflowOffsetAndUpdate(cur_sp_offset, params[i]);

                // std::string ins;
                // if(params[i].type == "int") ins = "sw";
                // else if(params[i].type == "char") ins = "sb";
                // else if(params[i].type == "float") ins = "fsw";
                // else if(params[i].type == "double") ins = "fsd";
                // function arguements below the current stack
                stream << get_mem_write(type, params[i].is_pntr) << " " << temp_reg << ", " << aligned_offset << "(sp)" << std::endl;
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

    DeclaratorType GetDeclaratorType() const{
        return DeclaratorType::Function;
    }

    std::string GetIdentifier() const override {
        return identifier_->GetIdentifier();
    }

    void Print(std::ostream &stream) const
    {
        identifier_->Print(stream);
        std::cout << "(";
        if (argument_expression_list_ != nullptr)
            argument_expression_list_->Print(stream);
        std::cout << ")";
    };

    void DefineConstantType(std::string type) override { return_type_ = type; }
    std::string GetType(Context &context) const override{
        return return_type_;
    }

    int GetNumBranches() const override { return 1; };
};

#endif
