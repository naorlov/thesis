#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include "string_formatter.h"
namespace po = boost::program_options;

po::variables_map parse_command_line(int argc, char **argv) {
    po::variables_map variables_map;
    try {
        po::options_description options("Allowed options");

        options.add_options()
            ("help", "Print help message")
            ("format-string", po::value<std::string>(), "Format string to use")
            ("output-file", po::value<std::string>(), "File to write result")
            ("count", po::value<int64_t>(), "Number of lines to generate");
        
        po::positional_options_description pos_options;

        pos_options.add("format-string", 1);

        auto parsed = 
            po::command_line_parser(argc, argv)
                .options(options)
                .positional(pos_options)
                .run();


        po::store(parsed, variables_map);

        if (variables_map.count("help")) {
            std::cerr << options << std::endl;
            exit(1);
        }

        if (variables_map.count("format-string") == 0) {
            std::cerr << "format-string option is required" << std::endl;
            exit(1);
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return variables_map;
}



int main(int argc, char **argv) {
    auto options = parse_command_line(argc, argv);

    std::ofstream output_file;

    if (options.count("output-file")) {
        output_file.open(options["output-file"].as<std::string>());
    }
    std::ostream & stream = (options.count("output-file") ? output_file : std::cout);

    std::string format_string = options["format-string"].as<std::string>();
    StringGenerator generator(format_string);
    generator.compile();

    // generator.run() appends newline at the end of output datas
    generator.run(stream, options["count"].as<int64_t>());
    stream.flush();
    return 0;
}
