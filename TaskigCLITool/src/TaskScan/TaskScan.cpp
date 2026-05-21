#include "TaskScan.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../GitUtils/GitUtils.hpp"

void Scan::TaskScan::scanFile(std::filesystem::path File, std::vector<Task::Task>& Tasks) {
    std::ifstream fileStream(File);

    if (!fileStream.is_open()) {
        std::cout << "Failed to open: " << File << std::endl;
        return;
    }

    {
        std::vector<int> tasksToRemove; // there is prob a much better way to do this
        for (int i = 0; i < Tasks.size(); i++) {
            if (Tasks.at(i).file == File)
                tasksToRemove.push_back(i);
        }
        for (int i = tasksToRemove.size() - 1; i >= 0; i--) {
            Tasks.erase(Tasks.begin() + tasksToRemove.at(i));
        }
    }

    GitUtils::GitUtils gitUtils;
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
        foundTask.file = File;
        foundTask.line = i;
        foundTask.text = comment.substr(comment.find(taskigMarker) + (sizeof(taskigMarker) / sizeof(char))); // [taskig] better handling, for psychopaths that don't wanna add a space after their taskigt marker
        foundTask.author = gitUtils.getAuthor(File, i);
        // [taskig] set task author based on who commited it or smth or if not commited, blame the current user
        Tasks.push_back(foundTask);
    }
    fileStream.close();

}