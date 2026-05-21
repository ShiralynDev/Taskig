#pragma once

#include <filesystem>
#include <vector>
#include "../Task/Task.hpp"

namespace Scan {

    class TaskScan {
        public:
        void scanFile(std::filesystem::path File, std::vector<Task::Task>& Tasks);
    };

}