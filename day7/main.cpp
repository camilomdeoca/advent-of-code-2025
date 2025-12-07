#include <cassert>
#include <cstdint>
#include <fstream>
#include <print>
#include <string>
#include <vector>

uint64_t times_split(std::vector<std::string> &lines, size_t x, size_t y)
{
    if (lines[y][x] == '^')
    {
        assert(x+1 < lines.front().length() && "Right splitted beam went out of bounds.");
        assert(x-1 >= 0 && "Left splitted beam went out of bounds.");
        
        uint64_t result = 1;
        // I right wasm't already visited
        if (lines[y][x+1] != '|')
        {
            result += times_split(lines, x+1, y);
            lines[y][x+1] = '|';
        }
        // I left wasm't already visited
        if (lines[y][x-1] != '|')
        {
            result += times_split(lines, x-1, y);
            lines[y][x-1] = '|';
        }
        return result;
    }
    else // lines[y][x] == '.' || lines[y][x] == 'S'
    {
        if (lines[y][x] == '.') lines[y][x] = '|';
        if (y+1 >= lines.size()) return 0;
        if (lines[y+1][x] != '|') return times_split(lines, x, y+1);
        else return 0;
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

    uint64_t splits = times_split(lines, start, 0);

    // uint64_t verification = 0;
    for (size_t y = 0; y < lines.size(); y++)
    {
        std::println("{}", lines[y]);
        // for (size_t x = 0; x < lines.front().length(); x++)
        // {
        //     if (lines[y][x] == '^' && lines[y-1][x] == '|') verification++;
        // }
    }
    std::println();

    std::println("Times split: {}", splits);
    // std::println("Times split (verification): {}", verification);

    return 0;
}
