#include "commands/args.hpp"

#include <iostream>

int main(int argc, char*  argv[]) {
    if (argc == 1)
        Commands::Args::printCommandsAndArgs();

    std::cout << argc;

    return 0;
}