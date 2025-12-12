#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include <signal.h>
#define DEBUGTRAP raise(SIGTRAP)

uint64_t path_count(uint64_t from, uint64_t to, std::vector<std::vector<uint64_t>> &connected_to)
{
    uint64_t acc = 0;
    for (uint64_t id_next_neighbor : connected_to[from])
    {
        acc += path_count(id_next_neighbor, to, connected_to);
    }
    return acc;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);

    std::unordered_map<std::string, uint64_t> ids;
    std::vector<std::vector<uint64_t>> connected_to;
    uint64_t next_id = 0;

    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        std::string input;
        if(!std::getline(ss, input, ':')) return 1;
        
        if (!ids.contains(input)) {
            ids.emplace(input, next_id++);
            connected_to.emplace_back();
        }
        uint64_t input_id = ids.find(input)->second;

        assert(ss.get() == ' ');

        std::string output;
        while (std::getline(ss, output, ' '))
        {
            if (!ids.contains(output)) {
                ids.emplace(output, next_id++);
                connected_to.emplace_back();
            }

            uint64_t output_id = ids.find(output)->second;
            connected_to[input_id].emplace_back(output_id);
        }
    }

    std::println("{}", path_count(ids["you"], ids["out"], connected_to));

    return 0;
}
