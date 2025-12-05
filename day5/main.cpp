#include <cassert>
#include <cstdint>
#include <fstream>
#include <print>
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);
    std::string line;

    struct Range { uint64_t start; uint64_t end; };

    std::vector<Range> ranges;

    while (std::getline(ifs, line))
    {
        if (line.size() == 0) break; // ranges ended

        uint64_t id_start, id_end;
        char dash;
        std::stringstream ss(line);
        ss >> id_start >> dash >> id_end;
        // std::println("{}{}{}", id_start, dash, id_end);
        ranges.emplace_back(id_start, id_end);
    }

    uint64_t fresh_count = 0;

    while (std::getline(ifs, line))
    {
        uint64_t ingredient_id;
        std::stringstream ss(line);
        ss >> ingredient_id;
        // std::println("{}", ingredient_id);
        bool is_fresh = false;
        for (const Range &range : ranges)
        {
            if (ingredient_id >= range.start && ingredient_id <= range.end)
            {
                is_fresh = true;
                fresh_count++;
                break;
            }
        }
    }

    std::println("Fresh count: {}", fresh_count);

    return 0;
}
