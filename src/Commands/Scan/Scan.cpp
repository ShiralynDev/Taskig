#include "Scan.hpp"

#include "../../CacheUtils/CacheUtils.hpp"

#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include "../../../external/SHA/include/sha.h"
#include <unordered_map>

std::vector<std::filesystem::path> findAllFiles(std::filesystem::path Path) {
    std::vector<std::filesystem::path> foundFiles;
    for (auto const& entry : std::filesystem::directory_iterator{Path}) {
        if (entry.is_regular_file())
            foundFiles.push_back(entry);
        else if (entry.is_directory()) {
            std::vector<std::filesystem::path> newFoundFiles = findAllFiles(entry);
            foundFiles.insert(foundFiles.end(), newFoundFiles.begin(), newFoundFiles.end());
        }
    }
    return foundFiles;
};

int Commands::scanCommand(int argc, char *argv[], CacheUtils::Cache& Cache, Scan::TaskScan& TaskScan) {

    bool forceScan = true;
    if (argc > 2) {
        std::string arg = argv[2];
        if (arg == "-F" || arg == "--force")
            forceScan = true;
    }

    std::filesystem::path workingDirectory = std::filesystem::current_path();
    std::vector<std::filesystem::path> files = findAllFiles(workingDirectory);

    std::unordered_map<std::string, std::string> pathHashes;

    for (auto& file : files) {
        if (!file.has_extension() || file.extension() != ".cpp" && file.extension() != ".hpp") // [taskig] compare to list instead
            continue;

        std::ifstream fileStream(file, std::ios::binary);
        fileStream.seekg(0, std::ios::end);
        std::streamsize fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        std::vector<char> buffer(fileSize);
        fileStream.read(buffer.data(), fileSize);

        sha::SHA512 sha512;
        std::string hash = sha512.hash(buffer.data());
        pathHashes.emplace(file, hash);
    }

    Cache.load(workingDirectory);

    if (!forceScan) {
        for (auto& hash : pathHashes) {
            if (Cache.hashes.find(hash.first) == Cache.hashes.end()) {
                TaskScan.scanFile(hash.first);
                Cache.hashes.emplace(hash);
            }
        }
    } else {
        for (auto& hash : pathHashes)
            TaskScan.scanFile(hash.first);
        Cache.hashes = pathHashes;
    }

    Cache.save(workingDirectory, TaskScan.tasks);

    return 0;
}