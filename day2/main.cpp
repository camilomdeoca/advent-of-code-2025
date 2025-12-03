#include <cstddef>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

bool is_repeated_for_count(const std::vector<int> &id_digits, int times_repeated)
{
    // if digit count is not divisible by the times_repeated_candidate skip.
    if (id_digits.size() % times_repeated != 0) return false;

    size_t repeated_part_size = id_digits.size() / times_repeated;

    // If we find a digit that doesnt repeat we set it to false
    // and continue with next times_repeated_candidate.
    bool is_repeated_for_this_part_size = true;
    for (int i = 0; i < repeated_part_size; i++)
    {
        bool is_this_digit_repeated = true;
        // Check every nth digit of every part equals the nth from the first part
        for (int j = 1; j < times_repeated; j++)
        {
            if (id_digits[i] != id_digits[repeated_part_size * j + i])
            {
                is_this_digit_repeated = false;
                break;
            }
        }

        if (is_this_digit_repeated == false)
        {
            is_repeated_for_this_part_size = false;
            break;
        }
    }

    return is_repeated_for_this_part_size;
}

bool is_valid(const std::vector<int> &id_digits)
{
    // If we find the digits are repeated for some times_repeated_candidate we set is_repeated
    // to true and return that, if it isnt repeated for any times_repeated_candidate it remains
    // false and we return that.
    bool is_repeated = false;
    for (
        int times_repeated_candidate = id_digits.size();
        times_repeated_candidate >= 2;
        times_repeated_candidate--
    ) {
        if (is_repeated_for_count(id_digits, times_repeated_candidate))
        {
            is_repeated = true;
            break; 
        }
    }

    return !is_repeated;
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

    std::vector<int> id_digits;

    unsigned long long sum_invalid_ids = 0;

    while(std::getline(ifs, line, ','))
    {
        // std::println("{}", line);
        unsigned long long id_start, id_end;
        char dash;
        std::stringstream ss(line);
        ss >> id_start >> dash >> id_end;
        // std::println("{}#{}#{}", id_start, dash, id_end);

        for (unsigned long id = id_start; id <= id_end; id++)
        {
            id_digits.clear();
            unsigned long long n = id;
            while (n > 0)
            {
                id_digits.push_back(n % 10);
                n /= 10;
            }

            if (!is_valid(id_digits)) sum_invalid_ids += id;
        }
    }

    std::println("Sum invalid ids: {}", sum_invalid_ids);

    return 0;
}
