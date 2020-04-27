#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    for (std::string line; std::getline(std::cin, line);)
    {
        try
        {
            int number = std::stoi(line);
            std::cout << number + 1 << '\n';
        }
        catch (std::invalid_argument & argument)
        {
            std::cerr << "fuck on line: " << line << '\n';
            continue;
        }
    }
    std::cout << std::endl;
    return 0;
}
