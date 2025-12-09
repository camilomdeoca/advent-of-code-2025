#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <print>
#include <string>
#include <vector>
#include <sstream>

struct Vec3 {
    float x, y, z;

    Vec3 operator-(const Vec3 &v)
    {
        return Vec3 {
            x - v.x,
            y - v.y,
            z - v.z,
        };
    }

    float length2()
    {
        return x*x + y*y + z*z;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);

    std::vector<Vec3> positions;

    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        Vec3 pos;
        char comma;
        ss >> pos.x >> comma >> pos.y >> comma >> pos.z;
        positions.emplace_back(pos);
    }
    std::println("Loaded positions.");
    const size_t pos_count = positions.size();

    std::vector<bool> connected(pos_count*pos_count, false);

    for (size_t i = 0; i < pos_count; i++)
    {
        connected[i * pos_count + i] = true; // Nodes are connected to itself
    }

    struct PairOfPosIdxAndDist {
        size_t i, j;
        float dist;
    };

    // using i < j
    std::vector<PairOfPosIdxAndDist> pairs_and_dist;
    for (size_t i = 0; i < pos_count; i++)
    {
        for (size_t j = i + 1; j < pos_count; j++)
        {
            pairs_and_dist.emplace_back(i, j, (positions[i] - positions[j]).length2());
        }
    }
    std::println("Calculated distances.");

    std::sort(
        pairs_and_dist.begin(),
        pairs_and_dist.end(),
        [](const PairOfPosIdxAndDist &a, const PairOfPosIdxAndDist &b) {
            return a.dist < b.dist;
        }
    );
    
    constexpr size_t connection_count = 1000;

    for (size_t i = 0; i < connection_count; i++)
    {
        const PairOfPosIdxAndDist &p = pairs_and_dist[i];
        // std::println("{} ({}, {})", p.dist, p.i, p.j);

        if (!connected[p.i * pos_count + p.j])
        {
            // std::println("Connecting {} to {} \t({})", p.i, p.j, p.dist);
            // Connect i to all that is connected to j
            for (size_t p_idx = 0; p_idx < pos_count; p_idx++)
            {
                // vector<bool> is a bitfield so maybe this is optimized to bitwise or
                connected[p.i * pos_count + p_idx]
                    = connected[p.i * pos_count + p_idx] || connected[p.j * pos_count + p_idx];
            }

            // connect "everything that is connected to i" to i
            for (size_t p_idx = 0; p_idx < pos_count; p_idx++)
            {
                if (p.i == p_idx || !connected[p.i * pos_count + p_idx]) continue;
                for (size_t i = 0; i < pos_count; i++)
                    connected[p_idx * pos_count + i]
                        = connected[p.i * pos_count + i] || connected[p_idx * pos_count + i];
            }
        }
    }
    std::println("Calculated clusters");

    // Count clusters
    std::vector<uint64_t> cluster_sizes;
    std::vector<bool> already_counted_positions(pos_count, false);
    for (size_t i = 0; i < pos_count; i++) 
    {
        if (already_counted_positions[i]) continue;
        uint64_t cluster_size = 0;
        for (size_t j = 0; j < pos_count; j++)
        {
            if (connected[i * pos_count + j]) {
                cluster_size++;
                already_counted_positions[j] = true;
            }
        }

        cluster_sizes.emplace_back(cluster_size);
    }
    
    std::sort(
        cluster_sizes.begin(),
        cluster_sizes.end(),
        [] (uint64_t a, uint64_t b) { return a > b; }
    );

    uint64_t cluster_mult_result = 1;
    for (size_t i = 0; i < 3; i++) cluster_mult_result *= cluster_sizes[i];
    std::println("Cluster mult result: {}", cluster_mult_result);

    return 0;
}
