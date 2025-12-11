#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <print>
#include <string>
#include <unordered_set>
#include <vector>
#include <sstream>

#include <signal.h>
#define DEBUGTRAP raise(SIGTRAP)

struct Problem {
    std::vector<bool> lights;
    std::vector<std::vector<int>> buttons;
};

uint16_t convert_lights_state_to_bitfield(const std::vector<bool> &lights)
{
    assert(lights.size() < sizeof(uint16_t) * 8);
    uint16_t result = 0;
    for (size_t i = 0; i < lights.size(); i++)
        if (lights[i]) result |= 1 << i;

    return result;
}

std::vector<uint16_t> convert_buttons_to_bitfields(const std::vector<std::vector<int>> &buttons)
{
    std::vector<uint16_t> result;
    for (const std::vector<int> &button : buttons)
    {
        assert(button.size() < sizeof(uint16_t) * 8);
        uint16_t button_bitfield = 0;
        for (size_t i = 0; i < button.size(); i++)
            button_bitfield |= 1 << button[i];
        result.emplace_back(button_bitfield);
    }

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);

    std::vector<Problem> problems;

    std::string line;
    while (std::getline(ifs, line))
    {
        Problem &problem = problems.emplace_back();
        std::stringstream ss(line);
        assert(ss.get() == '[');
        while (!ss.eof())
        {
            const char initial_button_state = ss.get();

            if(initial_button_state == '.') problem.lights.emplace_back(false);
            else if(initial_button_state == '#') problem.lights.emplace_back(true);
            else assert(false && "Invalid button character");

            if (ss.peek() == ']') break;
        }
        assert(ss.get() == ']');
        assert(ss.get() == ' ');
        while (!ss.eof())
        {
            std::vector<int> &button = problem.buttons.emplace_back();
            assert(ss.get() == '(');
            while (!ss.eof())
            {
                int toggled_light = ss.get() - '0';
                button.emplace_back(toggled_light);

                const char c = ss.get();
                if (c == ')') break;
                assert(c == ',');
            }
            assert(ss.get() == ' ');
            if (ss.peek() == '{') break;
        }
        // assert(ss.get() == '{');
        // while (!ss.eof())
        // {
        //     char c;
        //     int jolts;
        //     ss >> jolts >> c;
        //     problem.jolts.emplace_back(jolts);
        //     if (c == '}') break;
        //     assert(c == ',');
        // }
    }

    uint64_t sum = 0;

    // BFS
    for (const Problem &problem : problems)
    {
        int presses = 0;
        uint16_t final_light_state = convert_lights_state_to_bitfield(problem.lights);
        std::vector<uint16_t> buttons = convert_buttons_to_bitfields(problem.buttons);

        // maybe use abseil
        std::unordered_set<uint16_t> possible_states, possible_states_prev;
        possible_states_prev.emplace(0);
        while (true)
        {
            for (const uint16_t state : possible_states_prev)
            {
                for (uint16_t button : buttons)
                    possible_states.emplace(state ^ button);
            }
            presses++;
            if (possible_states.contains(final_light_state)) break;
            std::swap(possible_states, possible_states_prev);
            possible_states.clear();
        }
        std::println("{}", presses);

        sum += presses;
    }
    std::println();

    std::println("Sum of presses: {}", sum);

    return 0;
}
