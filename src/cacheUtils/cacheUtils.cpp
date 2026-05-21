#include "cacheUtils.hpp"
#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>
#include <iostream>
#include "../Task/Task.hpp"

void CacheUtils::Cache::load(std::filesystem::path& path) {
    std::filesystem::path configFilePath = path / ".taskig";
    if (!std::filesystem::is_regular_file(configFilePath))
        return; // [taskig] avoid silent return

    std::ifstream configFile(configFilePath);

    if (!configFile.is_open())
        return; // [taskig] avoid silent return

    bool taskPart = false;
    std::string line;
    int i = 0;
    while (std::getline(configFile, line)) {
        if (i == 0) {
            taskCount = stoi(line);
            i++;
            continue;
        }

        if (line.size() == 0) {
            taskPart = true;
            continue;
        }

        if (!taskPart) {
            std::string path = line.substr(0, line.find(' '));
            std::string hash = line.substr(path.length() + 1);
            hashes.emplace(path, hash);
        } else {
            std::string file = line.substr(0, line.find("----")); // [taskig] clean ts up
            line = line.substr(file.length()+4);

            std::string taskFileLine = line.substr(0, line.find("----"));
            line = line.substr(taskFileLine.length()+4);

            std::string text = line.substr(0, line.find("----"));
            line = line.substr(text.length()+4);

            std::string author = line.substr(0, line.find("----"));

            tasks.push_back(Task::Task{file, std::stoi(taskFileLine), text, author});
        }

        i++;
    }
    configFile.close();
}

void CacheUtils::Cache::save(std::filesystem::path& path, std::vector<Task::Task>& tasks) {
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

    configFile << "\n";

    for (auto& task : tasks) {
        configFile << task.file << "----" << task.line << "----" << task.text << "----" << task.author << '\n'; // [taskig] switch to a seperator that can't be fucked over by the user
    }

    configFile.close();
}