#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

int main()
{
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    std::vector<int> numbers;

    for (std::string line; std::getline(std::cin, line);)
    {
        numbers.push_back(std::stoi(line));
    }
    std::sort(numbers.begin(), numbers.end());

    for (auto number : numbers)
    {
        std::cout << number << '\n';
    }

    return 0;
}