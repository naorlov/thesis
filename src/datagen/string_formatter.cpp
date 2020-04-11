#include "string_formatter.h"

#include <random>
#include <vector>
#include <sstream>
#include <memory>
#include <iostream>

IdentityStringFormatter::IdentityStringFormatter(std::string command_string) {
    this->string = command_string;
}

std::string IdentityStringFormatter::format() {
    return this->string;
}

RandomStringFormatter::RandomStringFormatter(int64_t string_length_min, int64_t string_length_max) {
    this->string_length_min = string_length_min;
    this->string_length_max = string_length_max;
} 

unsigned long xorshf96(void)  {          //period 2^96-1
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

inline char rand_char() {
    return 'a' + xorshf96() % 26;
}

std::string RandomStringFormatter::format() {
    if (this->string_length_max == -1) {
        return this->genereate_fixed_length_string();
    }
    return this->genereate_variable_length_string();
}

std::string RandomStringFormatter::genereate_fixed_length_string() {
    auto result_string_size = this->string_length_min;
    std::stringstream ss;

    for (size_t i = 0; i != result_string_size; ++i) {
        ss << rand_char();
    }
    return ss.str();
}

std::string RandomStringFormatter::genereate_variable_length_string() {
    auto result_string_size = this->string_length_min;
    std::stringstream ss;

    for (size_t i = 0; i != result_string_size; ++i) {
        ss << rand_char();
    }
    return ss.str();
}

StringGenerator::StringGenerator(std::string &template_string) {
    this->template_string = template_string;
}

void StringGenerator::compile() {
    this->formatters.push_back(
        std::make_shared<IdentityStringFormatter>(this->template_string)
    );
    this->formatters.push_back(
        std::make_shared<RandomStringFormatter>(10)
    );
}

std::string StringGenerator::run(std::ostream &stream, int64_t count) {
    std::stringstream ss;
    for (size_t i = 0; i < count; ++i) {
        for (auto formatter : this->formatters) {
            ss << formatter->format() << ' ';
        }
        stream << ss.str() << '\n';
        ss.clear();
        ss.seekp(0); // for outputs: seek put ptr to start
        ss.seekg(0); // for inputs: seek get ptr to start
    }
    return ss.str();
}