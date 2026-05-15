#pragma once

#include <string>
#include <unordered_map>

namespace Commands::Args {

    static const std::unordered_map<std::string, std::string> commands = {
        {"Show amount of tasks", "-C, --count"}
    };

    void printCommandsAndArgs();

}