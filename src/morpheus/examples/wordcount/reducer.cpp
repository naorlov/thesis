#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

int main()
{
    std::map<std::string, int> result;

    std::string key;
    int count;

    while (std::cin >> key >> count)
    {
        result[key] += count;
    }

    for (auto [key, value] : result)
    {
        std::cout << key << " " << value << "\n";
    }
    std::cout.flush();
    return 0;
}
