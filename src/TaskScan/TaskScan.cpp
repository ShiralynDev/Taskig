#include "TaskScan.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void Scan::TaskScan::ScanFile(std::filesystem::path file) {
    std::ifstream fileStream(file);

    if (!fileStream.is_open()) {
        std::cout << "Failed to open: " << file << std::endl;
        return;
    }

    {
        std::vector<int> tasksToRemove; // there is prob a much better way to do this
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks.at(i).file == file)
                tasksToRemove.push_back(i);
        }
        for (int i = tasksToRemove.size(); i > 0; i--) {
            tasks.erase(tasks.begin() + tasksToRemove.at(i));
        }
    }

    std::string line;
    int i = 0;
    while (std::getline(fileStream, line)) {
        i++;
        size_t commentPosition = line.find("//"); // [taskig] support other languages and support /* */ comments
        if (commentPosition == std::string::npos)
            continue;

        char taskigMarker[] = "[taskig]";
        std::string comment = line.substr(commentPosition + 2);
        if (comment.find(taskigMarker) == std::string::npos) // [taskig] tolower to make sure taskig is found regardless of way it's written
            continue;

        Task::Task foundTask;
        foundTask.file = file;
        foundTask.line = i;
        foundTask.text = comment.substr(comment.find(taskigMarker) + (sizeof(taskigMarker) / sizeof(char))); // [taskig] better handling, for psychopaths that don't wanna add a space after their taskigt marker
        foundTask.author = "WIP";
        // [taskig] set task author based on who commited it or smth or if not commited, blame the current user
        tasks.push_back(foundTask);
    }
    fileStream.close();

}