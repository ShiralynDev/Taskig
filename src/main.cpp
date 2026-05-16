#include "commands/args.hpp"
#include "commands/scan/scan.hpp"
#include "cacheUtils/cacheUtils.hpp"

#include <iostream>
#include <string>

CacheUtils::Cache cache;

int main(int argc, char*  argv[]) {

    if (argc < 2) {
        Commands::Args::printCommandsAndArgs();
        return 0;
    }

    std::string command = argv[1];

    if (command == "help") {
        Commands::Args::printCommandsAndArgs();
    } else if (command == "count") { // [taskig] add scan option
        std::cout << cache.taskCount << " tasks";
    } else if (command == "scan") {
        Commands::scanCommand(argc, argv, cache);
        std::cout << "Scan complete, you have " << cache.taskCount << " tasks";
    } else {
        std::cout << "Unkown command: " << command;
        Commands::Args::printCommandsAndArgs();
    }

    return 0;
}