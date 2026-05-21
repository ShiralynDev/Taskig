#pragma once

#include "../Task/Task.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace CacheUtils {

    class Cache {
        public:
        int taskCount = 0;
        std::unordered_map<std::string, std::string> hashes;
        std::vector<Task::Task> tasks;
        void load(std::filesystem::path& Path);
        void save(std::filesystem::path& Path);
    };

}