#include <cassert>
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

    std::vector<bool> rolls;
    int y_size = 0;
    while (std::getline(ifs, line))
    {
        for (int x = 0; x < line.length(); x++)
        {
            char c = line[x];
            bool forklift;
            switch (c) {
                case '.':
                    forklift = false;
                    break;
                case '@':
                    forklift = true;
                    break;
                default:
                    std::println(stderr, "Error: Invalid character: {}", c);
                    return 1;
            }
            rolls.push_back(forklift);
        }
        y_size++;
    }
    int x_size = rolls.size() / y_size;
    assert(rolls.size() % y_size == 0);

    int reachable_rolls_count = 0;

    for (int y = 0; y < y_size; y++)
    {
        for (int x = 0; x < x_size; x++)
        {
            // If there isnt a roll there dont even count the adjacent ones
            if (rolls[y * x_size + x] == false)
            {
                std::print(".");
                continue;
            }

            int adjacent_rolls_count = 0;
            for (int y_delta = -1; y_delta <= 1; y_delta++)
            {
                for (int x_delta = -1; x_delta <= 1; x_delta++)
                {
                    // Skip counting the center
                    if (y_delta == 0 && x_delta == 0) continue;

                    // Calculate adjacent positions global coordinates
                    int y_final = y + y_delta;
                    int x_final = x + x_delta;

                    // If the coordinates go outside continue
                    if (x_final < 0 || x_final >= x_size || y_final < 0 || y_final >= y_size) continue;

                    // If there is a roll in that adjacent position add 1 to count
                    if (rolls[y_final * x_size + x_final] == true) adjacent_rolls_count++;
                }
            }
            if (adjacent_rolls_count < 4)
            {
                reachable_rolls_count++;
                // std::print("x");
            }
            // else std::print("{}", rolls[y * x_size + x] ? '@' : '.');
            std::print("{}", adjacent_rolls_count);
        }
        std::println();
    }

    std::println("Reachable rolls: {}", reachable_rolls_count);

    return 0;
}
