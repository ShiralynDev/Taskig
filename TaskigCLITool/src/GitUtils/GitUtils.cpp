#include "GitUtils.hpp"
#include <cstdio>
#include <string>
#include <iostream>

std::string GitUtils::GitUtils::executeCommand(std::string Command) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen((Command + " 2>&1").c_str(), "r"); // to return instead of print the "no head" :skull: error 

    if (!pipe) {
        std::cout << "popen failed for execute command";
        return "ERR";
    }

    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}

std::string GitUtils::GitUtils::getAuthor(std::filesystem::path& File, int& Line) {
    std::string gitBlameCommand = std::string("git blame -L ") + 
        std::to_string(Line) + std::string(",") + std::to_string(Line) +
        std::string(" ") + File.string();

    std::string commandResult = executeCommand(gitBlameCommand);

    if (commandResult.find("fatal:") != std::string::npos)
        return "you (not on git)";

    std::string author = commandResult.substr(commandResult.find('(') + 1);
    author = author.substr(0, author.find(' '));

    return author;
}