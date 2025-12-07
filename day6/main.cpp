#include <cassert>
#include <cstdint>
#include <fstream>
#include <print>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(ifs, line))
    {
        lines.emplace_back();
        std::swap(lines.back(), line);
    }
    std::println();

    const size_t line_length = lines.front().length();
    
    // all lines should be the same length
    for (const auto &line : lines) assert(line.length() == line_length);

    uint64_t sum = 0;

    std::vector<uint64_t> numbers;
    size_t char_idx = line_length - 1;
    for (int64_t char_idx = line_length - 1; char_idx >= 0; char_idx--)
    {
        std::string number_string;
        for (size_t i = 0; i < lines.size() - 1; i++)
        {
            char c = lines[i][char_idx];
            if (c != ' ') number_string.push_back(c);
        }

        // If its a blank column we should skip
        if (number_string.empty()) continue;

        // std::println("`{}`", number_string);
        uint64_t number = std::stoul(number_string);
        numbers.push_back(number);

        // Thankfully all the operations are always in the last column of the problem
        // so when there is an operator in the last row we can operate on all the numbers
        // collected and then free the `numbers` vector.
        char operation = lines.back()[char_idx];
        uint64_t acc;
        switch (operation) {
        case ' ':
            break;
        case '+':
            acc = 0;
            for (const uint64_t num : numbers) acc += num;
            numbers.clear();
            sum += acc;
            break;
        case '*':
            acc = 1;
            for (const uint64_t num : numbers) acc *= num;
            numbers.clear();
            sum += acc;
            break;
        }
    }

    std::println("Sum: {}", sum);

    return 0;
}
