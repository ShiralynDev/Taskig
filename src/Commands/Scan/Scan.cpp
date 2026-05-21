#include "Scan.hpp"

#include "../../CacheUtils/CacheUtils.hpp"

#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include "../../../external/SHA/include/sha.h"
#include <unordered_map>

std::vector<std::filesystem::path> findAllFiles(std::filesystem::path path) {
    std::vector<std::filesystem::path> foundFiles;
    for (auto const& entry : std::filesystem::directory_iterator{path}) {
        if (entry.is_regular_file())
            foundFiles.push_back(entry);
        else if (entry.is_directory()) {
            std::vector<std::filesystem::path> newFoundFiles = findAllFiles(entry);
            foundFiles.insert(foundFiles.end(), newFoundFiles.begin(), newFoundFiles.end());
        }
    }
    return foundFiles;
};

int Commands::scanCommand(int argc, char *argv[], CacheUtils::Cache& cache, Scan::TaskScan& taskScan) {

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

    cache.load(workingDirectory);

    if (!forceScan) {
        for (auto& hash : pathHashes) {
            if (cache.hashes.find(hash.first) == cache.hashes.end()) {
                taskScan.ScanFile(hash.first);
                cache.hashes.emplace(hash);
            }
        }
    } else {
        for (auto& hash : pathHashes)
            taskScan.ScanFile(hash.first);
        cache.hashes = pathHashes;
    }

    cache.save(workingDirectory, taskScan.tasks);

    return 0;
}