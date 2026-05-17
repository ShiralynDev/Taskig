#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace Scan {

    class Task { // [taskig] maybe move task from the Scan namespace
        public:
        std::string file;
        int line;
        std::string text;
        std::string author; // [taskig] get author from git history
    };
    
    class TaskScan {
        public:
        std::vector<Task> tasks;
        void ScanFile(std::filesystem::path file);
    };

}