#pragma once

#include <filesystem>
#include <vector>
#include "../Task/Task.hpp"

namespace Scan {

    class TaskScan {
        public:
        std::vector<Task::Task> tasks;
        void ScanFile(std::filesystem::path file);
    };

}