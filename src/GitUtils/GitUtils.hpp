#pragma once

#include <filesystem>
#include <string>

namespace GitUtils {

    class GitUtils {
        private:
            std::string executeCommand(std::string Command);

        public:
            std::string getAuthor(std::filesystem::path& File, int& Line);
    };

}