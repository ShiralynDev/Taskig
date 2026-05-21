#include "Args.hpp"

#include <iostream>

void Commands::Args::printCommandsAndArgs() {
    for (auto& command : commands) {
        std::cout << command.second;
        for (int i = 40 - command.second.length(); i > 0; i--) {
            std::cout << ' ';
        }
        std::cout << command.first << std::endl;
    }
}