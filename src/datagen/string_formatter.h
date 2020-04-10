#ifndef STRING_FORMATTER_H 
#define STRING_FORMATTER_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

const int ALPHABET_LENGTH = 26;
inline char rand_char();
static unsigned long x=123456789, y=362436069, z=521288629;

unsigned long xorshf96(void);

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;


class Randomizer {
private:
    static const bool debug_mode = false;
    boost::random::mt19937 rng_;

    // The private constructor so that the user can not directly instantiate
    Randomizer() {
        if(debug_mode==true){
            this->rng_ = boost::random::mt19937();
        } else{
            this->rng_ = boost::random::mt19937(current_time_nanoseconds());
        }
    };

    int current_time_nanoseconds(){
        struct timespec tm;
        clock_gettime(CLOCK_REALTIME, &tm);
        return tm.tv_nsec;
    }

    Randomizer(Randomizer const&) = delete;
    void operator=(Randomizer const&) = delete;

public:
    static Randomizer& get_instance(){
        static Randomizer instance;
        return instance;
    }

    int rand(unsigned int floor, unsigned int ceil){
        random::uniform_int_distribution<> rand_ = random::uniform_int_distribution<> (floor,ceil);
        return (rand_(rng_));
    }
};


class StringFormatter {
public:
    StringFormatter() = default;
    StringFormatter(std::string command_string);
    virtual std::string format() = 0;
    virtual ~StringFormatter() = default;
};

class IdentityStringFormatter : public StringFormatter {
private:
    std::string string;
public:

    IdentityStringFormatter() = default;
    IdentityStringFormatter(std::string command_string);
    std::string format();
};


class RandomStringFormatter : public StringFormatter {
private:
    int64_t string_length_min;
    int64_t string_length_max;
public:
    RandomStringFormatter() = default;
    RandomStringFormatter(int64_t string_length_min = 10, int64_t string_length_max = -1); 

    std::string format();

private:
    std::string genereate_fixed_length_string();
    std::string genereate_variable_length_string();
    
};


class IntStringFormatter : public StringFormatter {
private:
    int64_t int_min;
    int64_t int_max;
public:
    IntStringFormatter() = default;
    IntStringFormatter(int64_t int_min, int64_t int_max); 

    std::string format();
};



class StringGenerator {
private:
    std::string template_string;
    std::vector<std::shared_ptr<StringFormatter>> formatters;
public:
    StringGenerator() = default;

    StringGenerator(std::string &string);

    void compile();
    
    std::string run(std::ostream &stream, int64_t count = 10);
};

#endif // STRING_FORMATTER_H