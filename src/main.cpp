#include "Commands/Args.hpp"
#include "Commands/Scan/Scan.hpp"
#include "CacheUtils/CacheUtils.hpp"
#include "TaskScan/TaskScan.hpp"

#include <filesystem>
#include <iostream>
#include <string>

std::filesystem::path cacheLocation = std::filesystem::current_path();

CacheUtils::Cache Cache;
Scan::TaskScan TaskScan;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        Commands::Args::printCommandsAndArgs();
        return 0;
    }

    std::string command = argv[1];

    if (command == "help") {
        Commands::Args::printCommandsAndArgs();
    } else if (command == "count") { // [taskig] add scan option
        std::cout << TaskScan.tasks.size() << " tasks";
    } else if (command == "scan") {
        Commands::scanCommand(argc, argv, Cache, TaskScan);
        std::cout << "Scan complete, you have " << TaskScan.tasks.size() << " tasks";
    } else if (command == "list") {
        Cache.load(cacheLocation);
        for (auto& task : Cache.tasks) // [taskig] move to function with better standard printing for use in multiple functions
            std::cout << std::filesystem::relative(task.file) << ":" << task.line << " " << task.text << " - " << task.author << std::endl;
        std::cout << Cache.tasks.size() << " tasks";
    } else {
        std::cout << "Unkown command: " << command;
        Commands::Args::printCommandsAndArgs();
    }

    return 0;
}