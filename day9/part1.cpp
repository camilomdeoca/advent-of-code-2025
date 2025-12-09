#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <print>
#include <string>
#include <vector>
#include <sstream>

struct Vec2 {
    uint64_t x, y;

    uint64_t area(const Vec2 &v)
    {
        uint64_t side_x = (x > v.x ? x - v.x : v.x - x) + 1;
        uint64_t side_y = (y > v.y ? y - v.y : v.y - y) + 1;
        return side_x * side_y;
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

    std::vector<Vec2> positions;

    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        Vec2 pos;
        char comma;
        ss >> pos.x >> comma >> pos.y;
        positions.emplace_back(pos);
    }
    std::println("Loaded positions.");
    const size_t pos_count = positions.size();

    uint64_t max_area = 0;
    // std::array<Vec2, 2> corners_max_area;
    for (size_t i = 0; i < pos_count; i++)
    {
        for (size_t j = i+1; j < pos_count; j++)
        {
            uint64_t area = positions[i].area(positions[j]);
            if (area > max_area)
            {
                max_area = area;
                // corners_max_area[0] = positions[i];
                // corners_max_area[1] = positions[j];
            }
        }
    }

    std::println("Largest area: {}", max_area);

    return 0;
}
