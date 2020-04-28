#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    for (std::string line; std::getline(std::cin, line);)
    {
        int current_word_start = 0;
        for (size_t i = 0; i != line.size(); ++i)
        {
            if (std::isspace(line[i]))
            {
                if (current_word_start == i || line[i] == '\n')
                {
                    current_word_start = i + 1;
                    continue;
                }
                std::cout << std::string(line.begin() + current_word_start, line.begin() + i)
                          << " 1\n";
                current_word_start = i + 1;
            }
        }
        if (line.begin() + current_word_start != line.end())
        {
            std::cout << std::string(line.begin() + current_word_start, line.end()) << " 1\n";
        }
    }
    std::cout.flush();
    return 0;
}
