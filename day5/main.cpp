#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iterator>
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

    std::vector<Range> ranges, ranges_filter_aux;

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

    bool id_ranges_changed = true;
    size_t pass = 0;
    while (id_ranges_changed)
    {
        id_ranges_changed = false;
        // std::println("PASS {}", ++pass);
        std::vector<size_t> to_be_deleted_idx;
        for (size_t j = 0; j < ranges.size(); j++)
        {
            for (size_t i = 0; i < ranges.size(); i++)
            {
                if (i == j) continue;

                if (ranges[i].start >= ranges[j].start && ranges[i].start <= ranges[j].end)
                {
                    ranges[i].start = ranges[j].end + 1;
                    id_ranges_changed = true;
                }
                
                if (ranges[i].end >= ranges[j].start && ranges[i].end <= ranges[j].end)
                {
                    // Should check for underflow but there isnt any low numbers
                    // in ranges in input.txt so it doesnt matter
                    ranges[i].end = ranges[j].start - 1;
                    id_ranges_changed = true;
                }
            }
        }

        std::copy_if(
            ranges.begin(),
            ranges.end(),
            std::back_inserter(ranges_filter_aux),
            [](const Range &range) { return range.end >= range.start; }
        );

        std::swap(ranges_filter_aux, ranges);
        ranges_filter_aux.clear();
    }

    uint64_t fresh_ids_count = 0;
    for (const Range &range : ranges)
    {
        std::println("{}-{}", range.start, range.end);
        fresh_ids_count += range.end - range.start + 1;
    }

    std::println("Fresh IDs count: {}", fresh_ids_count);

    return 0;
}
