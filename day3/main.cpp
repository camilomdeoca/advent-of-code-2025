#include <cassert>
#include <fstream>
#include <print>
#include <string>
#include <vector>

constexpr size_t BATTERY_COUNT = 12;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);
    std::string line;

    unsigned long long total_output_joltage = 0;

    std::vector<uint8_t> digits;
    while (std::getline(ifs, line))
    {
        digits.clear();
        for (const char &c : line)
        {
            digits.push_back(c - '0');
        }
        // for (const auto &digit : digits)
        // {
        //     std::print("{}", static_cast<unsigned int>(digit));
        // }
        // std::println();
        
        size_t where_to_start_searching = 0;
        unsigned long long joltage_for_bank = 0;
        for (
            int batteries_we_need_to_have_left = BATTERY_COUNT - 1;
            batteries_we_need_to_have_left >= 0;
            batteries_we_need_to_have_left--
        ) {
            size_t index_of_max_digit = -1;
            uint8_t max_digit = 0;
            // `- battery_count` because we have to leave at least
            // the ammount of digits for next batteries
            for (size_t i = where_to_start_searching; i < digits.size() - batteries_we_need_to_have_left; i++)
            {
                if (digits[i] > max_digit)
                {
                    index_of_max_digit = i;
                    max_digit = digits[i];
                }

                //if (max_digit == 9) break; // there wont be more than 9
            }
            assert(index_of_max_digit >= 0);
            assert(max_digit > 0);
            // std::println("IDX {}", index_of_max_digit);
            where_to_start_searching = index_of_max_digit + 1;
            joltage_for_bank = joltage_for_bank * 10 + max_digit;
        }

        // std::println("{}", joltage_for_bank);
        total_output_joltage += joltage_for_bank;
    }
    std::println("TOTAL SUM: {}", total_output_joltage);

    return 0;
}
