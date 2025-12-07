#include <cassert>
#include <cstdint>
#include <fstream>
#include <print>
#include <string>
#include <vector>

int64_t count_timelines(std::vector<std::string> &lines, size_t x, size_t y, std::vector<int64_t> &cache)
{
    if (cache[y * lines.front().length() + x] >= 0)
        return cache[y * lines.front().length() + x];

    if (lines[y][x] == '^')
    {
        assert(x+1 < lines.front().length() && "Right splitted beam went out of bounds.");
        assert(x-1 >= 0 && "Left splitted beam went out of bounds.");
        
        int64_t result = count_timelines(lines, x+1, y, cache) + count_timelines(lines, x-1, y, cache);
        cache[y * lines.front().length() + x] = result;
        return result;
    }
    else // lines[y][x] == '.' || lines[y][x] == 'S'
    {
        int64_t result = 1;
        if (y+1 < lines.size()) result = count_timelines(lines, x, y+1, cache);
        cache[y * lines.front().length() + x] = result;
        return result;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);

    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(ifs, line))
        {
            lines.emplace_back();
            std::swap(lines.back(), line);
        }
    }

    const size_t line_length = lines.front().length();
    
    // all lines should be the same length
    for (const auto &line : lines) assert(line.length() == line_length);

    size_t start;
    for (start = 0; start < lines.front().length(); start++)
        if (lines.front()[start] == 'S') break;

    std::vector<int64_t> cache(lines.front().length() * lines.size(), -1);

    int64_t timelines = count_timelines(lines, start, 0, cache);

    std::println("Times split: {}", timelines);
    // std::println("Times split (verification): {}", verification);

    return 0;
}
