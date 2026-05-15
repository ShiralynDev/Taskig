#include "args.hpp"

#include <iostream>

void Commands::Args::printCommandsAndArgs() {
    for (auto& command : commands) {
        std::cout << command.second << "    " << command.first << std::endl;
    }
}