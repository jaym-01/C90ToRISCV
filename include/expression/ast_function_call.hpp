
#ifndef AST_FUNCTION_CALL_HPP
#define AST_FUNCTION_CALL_HPP

#include "../ast_node.hpp"
#include "../ast_node_list.hpp"

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
        int cur_sp_offset = 0;
        FuncDefinition func_def = context.GetFuncDef(id);

        std::vector<VariableContext> params = func_def.params;
        for (int i = 0; i < args.size(); i++)
        {
            if (i <= 7) {
                std::string dest_reg = "a" + std::to_string(i);
                args[i]->EmitRISCWithDest(stream, context, dest_reg);

            } else {
                std::string temp_reg = "";
                args[i]->EmitRISCWithDest(stream, context, temp_reg);
                int aligned_offset = context.CalcOverflowOffsetAndUpdate(cur_sp_offset, params[i]);
                stream << "sw " << temp_reg << ", " << aligned_offset << "(sp)" << std::endl;
                context.FreeRegister(temp_reg);
            }
        }

        // Call function
        // Save temp regs to memory
        std::vector<SavedRegister> saved_regs = context.SaveTempRegsToMemory(stream);
        EmitCallFunction(stream);
        context.RestoreRegistersFromMem(stream, saved_regs);

        if (dest == "") {
            // TODO: Fix this
            dest = context.ReserveRegister("int");
        }

        stream<<"add "<<dest<<", a0, "<<"zero"<<std::endl;
    };

    void Print(std::ostream &stream) const
    {
        identifier_->Print(stream);
        std::cout << "(";
        if (argument_expression_list_ != nullptr)
            argument_expression_list_->Print(stream);
        std::cout << ")";
    };

    int GetNumBranches() const override { return 1; };
};

#endif
