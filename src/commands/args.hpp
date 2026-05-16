#pragma once

#include <string>
#include <vector>

namespace Commands::Args {
    static const std::vector<std::pair<std::string, std::string>> commands = {
        {"Show commands", "help"},
        {"Show amount of tasks", "count"},
        {"Scan files for tasks", "scan (-F, --force ignore cache)"}
    };

    void printCommandsAndArgs();
}
