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

    std::string line;

    constexpr size_t NUMS_PER_COL = 4;

    std::vector<std::array<uint64_t, NUMS_PER_COL>> numbers;
    uint64_t x_size = 0;

    for (size_t y = 0; y < NUMS_PER_COL; y++)
    {
        std::getline(ifs, line);

        char *cursor = line.data();
        const char *end = &line.back();
        int x = 0;
        while (cursor < end)
        {
            if (y == 0) numbers.emplace_back();
            uint64_t num = std::strtoul(cursor, &cursor, 10);
            // std::print("{} ", num);
            numbers[x][y] = num;
            x++;
        }
        // std::println();
    }

    // std::println();
    // for (const auto &col : numbers)
    // {
    //     for (const auto num : col)
    //     {
    //         std::print("{} ", num);
    //     }
    //     std::println();
    // }
    // std::println();

    uint64_t sum = 0;

    {
        std::getline(ifs, line);
        // std::println("{}", line);
        size_t col_idx = 0;
        auto it = line.begin();
        while (it != line.end())
        {
            while (it != line.end() && *it == ' ') it++;
            if (it == line.end()) break;

            uint64_t acc;
            switch (*it) {
            case '+':
                acc = 0;
                for (const auto num : numbers[col_idx]) acc += num;
                break;
            case '*':
                acc = 1;
                for (const auto num : numbers[col_idx]) acc *= num;
                break;
            default:
                std::println("??????");
                continue;
                break;
            }
            // std::print("{} ", acc);
            it++;
            sum += acc;
            col_idx++;
        }
        // std::println();
    }

    std::println("sum {}", sum);


    return 0;
}
