#include <iostream>
#include <string>

// Prime numbers, generated by OpenSSL.
const uint64_t base_multiplier = 16391811141419416477ull;
const uint64_t char_multiplier = 13901687573247793217ull;
const uint64_t modulus = 18409643914347218969ull;

uint64_t hash(const std::string & string)
{
    uint64_t result = 0;
    for (uint64_t c : string)
    {
        result = (result * base_multiplier) ^ (c * char_multiplier);
    }
    return result % modulus;
}


int main()
{
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    for (std::string line; std::getline(std::cin, line);)
    {
        std::cout << std::hex << hash(line) << '\n';
    }
    return 0;
}