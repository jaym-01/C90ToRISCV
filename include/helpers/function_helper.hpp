#ifndef FUNCTION_HELPER
#define FUNCTION_HELPER

#include<vector>
#include<string>

void GetArgRegister(){
    std::vector<std::string> n_args, f_args;
    for(int i = 0; i < 7; i++) n_args.push_back("a" + std::to_string(i));
    for(int i = 0; i < 7; i++) n_args.push_back("fp" + std::to_string(i));
}

#endif
