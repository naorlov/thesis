#include <iostream>
#include <map>

std::map<std::string, std::string> parse_args(int argc, char ** argv)
{
    std::map<std::string, std::string> options;

    if (argc < 2)
    {
        std::cerr << "Not enough arguments" << std::endl;
        exit(1);
    }
    options["specfile"] = argv[1];
    options["platform"] = argv[2];
    return options;
}
