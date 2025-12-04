#include <cassert>
#include <fstream>
#include <print>
#include <string>
#include <vector>

// removes reachable and returns how many where removed
// We change the matrix while checking what we can remove and this could make us remove some rolls
// that would be removed in a later step so it doesnt matter
int remove_reachable(std::vector<bool> &rolls, int y_size, int x_size)
{
    int reachable_rolls_count = 0;
    for (int y = 0; y < y_size; y++)
    {
        for (int x = 0; x < x_size; x++)
        {
            // If there isnt a roll there dont even count the adjacent ones
            if (rolls[y * x_size + x] == false) continue;

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
                rolls[y * x_size + x] = false;
                reachable_rolls_count++;
            }
        }
    }

    return reachable_rolls_count;
}

int remove_all_posible_rolls(std::vector<bool> &rolls, int y_size, int x_size)
{
    int removed_rolls = 0;
    while (int removed_rolls_in_this_step = remove_reachable(rolls, y_size, x_size))
    {
        removed_rolls += removed_rolls_in_this_step;
    }
    return removed_rolls;
}

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

    int total_removed_rolls = remove_all_posible_rolls(rolls, y_size, x_size);
    std::println("removed_rolls: {}", total_removed_rolls);

    return 0;
}
