module;

// исправить в clang++21
#include <string>
#include <memory>
#include <cstdio>


export module comand_exec;

export std::string execute(const std::string& command) {
    // FILE* raw_pipe = popen(command.c_str(), "r");
    // if (!raw_pipe) 
    //     throw std::runtime_error("popen() failed!");

    // std::unique_ptr<FILE, decltype(&pclose)> pipe(raw_pipe, pclose);
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    
    std::string result;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
        result += buffer;
    return result;
}