#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

namespace CacheUtils {

    class Cache {
        public:
        int taskCount = 0;
        std::unordered_map<std::string, std::string> hashes;
        void load(std::filesystem::path& path);
        void save(std::filesystem::path& path);
    };

}