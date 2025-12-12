#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <print>
#include <string>
#include <unordered_set>
#include <vector>
#include <sstream>

#include <signal.h>
#define DEBUGTRAP raise(SIGTRAP)

constexpr size_t SHAPE_COUNT = 6;

struct Shape {
    std::array<bool, 9> cells;
    size_t cell_count;
};

struct Problem {
    uint w, h;
    std::array<uint, SHAPE_COUNT> present_counts;
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);

    std::array<Shape, SHAPE_COUNT> shapes;
    std::vector<Problem> problems;

    std::string line;
    for (size_t shape_idx = 0; shape_idx < shapes.size(); shape_idx++)
    {
        if (!std::getline(ifs, line)) return 1;
        assert(line.length() == 2);
        assert(line[0] >= '0' && line[0] <= '9');
        assert(line[1] == ':');

        shapes[shape_idx].cell_count = 0;
        for (size_t i = 0; i < 3; i++)
        {
            if (!std::getline(ifs, line)) return 1;
            assert(line.length() == 3);

            for (size_t j = 0; j < 3; j++)
            {
                bool cell_here = line[j] == '#';
                if (cell_here) shapes[shape_idx].cell_count++;
                shapes[shape_idx].cells[i*3 + j] = cell_here;
            }
        }
        if (!std::getline(ifs, line)) return 1; // remove blank row
    }

    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);

        Problem &problem = problems.emplace_back();

        ss >> problem.w;
        assert(ss.get() == 'x');
        ss >> problem.h;
        assert(ss.get() == ':');

        for (size_t shape_idx = 0; shape_idx < SHAPE_COUNT; shape_idx++)
        {
            assert(ss.get() == ' ');
            ss >> problem.present_counts[shape_idx];
        }
    }

    size_t regions_that_fit_presents = 0;

    for (const Problem &problem : problems)
    {
        size_t region_cell_count = problem.w*problem.h;

        size_t presents_cell_count = 0;
        for (size_t shape_idx = 0; shape_idx < shapes.size(); shape_idx++)
        {
            presents_cell_count
                += shapes[shape_idx].cell_count * problem.present_counts[shape_idx];
        }
        std::println("{} {}", region_cell_count, presents_cell_count);
        if (presents_cell_count <= region_cell_count)
        {
            regions_that_fit_presents++;
        }
    }

    std::println("Regions that fit presents: {}", regions_that_fit_presents);

    return 0;
}
