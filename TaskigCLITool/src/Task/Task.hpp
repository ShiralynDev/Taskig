#pragma once

#include <string>

namespace Task {
    class Task {
        public:
        std::string file;
        int line;
        std::string text;
        std::string author; // [taskig] get author from git history
    };
    
}