#include "cacheUtils.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <ostream>
#include <string>
#include <iostream>

void CacheUtils::Cache::load(std::filesystem::path& path) {
    std::filesystem::path configFilePath = path / ".taskig";
    if (!std::filesystem::is_regular_file(configFilePath))
        return; // [taskig] avoid silent return

    std::ifstream configFile(configFilePath);

    if (!configFile.is_open())
        return; // [taskig] avoid silent return

    std::string line;
    int i = 0;
    while (std::getline(configFile, line)) {
        if (i == 0) {
            taskCount = stoi(line);
            i++;
            continue;
        }

        std::string path = line.substr(0, line.find(' '));
        std::string hash = line.substr(path.length() + 1);
        hashes.emplace(path, hash);

        i++;
    }
    configFile.close();
}

void CacheUtils::Cache::save(std::filesystem::path& path) {
    std::filesystem::path configFilePath = path / ".taskig";
    std::ofstream configFile(configFilePath);

    if (!configFile.is_open()) {
        std::cout << "failed to open configFile\n";
        return;
    }

    configFile << taskCount << '\n';

    for (auto& hash : hashes) {
        configFile << hash.first << " " << hash.second << '\n';
    }
    configFile.close();
}