#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <print>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <sstream>

#include <signal.h>
#define DEBUGTRAP raise(SIGTRAP)

// Source - https://stackoverflow.com/a
// Posted by Yakk - Adam Nevraumont, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-12, License - CC BY-SA 4.0
size_t hash_combine(size_t lhs, size_t rhs) {
    if constexpr (sizeof(size_t) >= 8)
    {
        lhs ^= rhs + 0x517cc1b727220a95 + (lhs << 6) + (lhs >> 2);
    }
    else
    {
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    }

    return lhs;
}


struct PairHash 
{
    uint64_t operator()(const std::pair<uint64_t, uint64_t> &x) const noexcept
    {
        return hash_combine(std::hash<uint64_t>{}(x.first), std::hash<uint64_t>{}(x.second));
    }
};

uint64_t path_count(
    uint64_t from,
    uint64_t to,
    std::vector<std::vector<uint64_t>> &connected_to,
    std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> &cache
) {
    if (from == to) return 1;
    uint64_t acc = 0;
    for (uint64_t id_next_neighbor : connected_to[from])
    {
        if (cache.contains(std::make_pair(id_next_neighbor, to)))
            acc += cache.at(std::make_pair(id_next_neighbor, to));
        else
            acc += path_count(id_next_neighbor, to, connected_to, cache);
    }

    cache.emplace(std::make_pair(from, to), acc);

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
    std::println("Parsed file.");

    std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> cache;

    uint64_t first_fft_then_dac = 1;

    uint64_t from_svr_to_fft = path_count(ids["svr"], ids["fft"], connected_to, cache);
    std::println("1/6 from_svr_to_fft = {}", from_svr_to_fft);
    first_fft_then_dac *= from_svr_to_fft;
    cache.clear();

    if (first_fft_then_dac > 0)
    {
        uint64_t from_fft_to_dac = path_count(ids["fft"], ids["dac"], connected_to, cache);
        std::println("2/6 from_fft_to_dac = {}", from_fft_to_dac);
        first_fft_then_dac *= from_fft_to_dac;
        cache.clear();
    }

    if (first_fft_then_dac> 0)
    {
        uint64_t from_dac_to_out = path_count(ids["dac"], ids["out"], connected_to, cache);
        std::println("3/6 from_dac_to_out = {}", from_dac_to_out);
        first_fft_then_dac *= from_dac_to_out;
        cache.clear();
    }
    
    uint64_t first_dac_then_fft = 1;

    uint64_t from_svr_to_dac = path_count(ids["svr"], ids["dac"], connected_to, cache);
    std::println("4/6 from_svr_to_dac = {}", from_svr_to_dac);
    first_dac_then_fft *= from_svr_to_dac;
    cache.clear();

    if (first_dac_then_fft > 0)
    {
        uint64_t from_dac_to_fft = path_count(ids["dac"], ids["fft"], connected_to, cache);
        std::println("5/6 from_dac_to_fft = {}", from_dac_to_fft);
        first_dac_then_fft *= from_dac_to_fft;
        cache.clear();
    }
   
    if (first_dac_then_fft > 0)
    {
        uint64_t from_fft_to_out = path_count(ids["fft"], ids["out"], connected_to, cache);
        std::println("6/6 from_fft_to_out = {}", from_fft_to_out);
        first_dac_then_fft *= from_fft_to_out;
        cache.clear();
    }

    uint64_t result = first_dac_then_fft + first_fft_then_dac;

    std::println("Result: {}", result);

    return 0;
}
