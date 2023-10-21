#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <regex>
#include <optional>
#include <queue>

#include <common.h>

struct Machine {
    std::unordered_map<int, bool> tape{};
    int cursor{0};
    char state{};
};

const auto Noop = [](Machine&){};

struct State {
    std::function<void(Machine&)> on_zero{Noop};
    std::function<void(Machine&)> on_one{Noop};
};

struct Blueprint {
    char starting_state{};
    unsigned number_of_steps{};
    std::unordered_map<char, State> states;
};

namespace state_patters {
    constexpr auto begin_in_state{"Begin in state ([A-Z])."};
    constexpr auto number_of_steps{"Perform a diagnostic checksum after ([0-9]+) steps."};
    constexpr auto state_name{"In state ([A-Z]):"};
    constexpr auto current_value{"If the current value is (0|1):"};
    constexpr auto write_value{"- Write the value (0|1)."};
    constexpr auto move_direction{"- Move one slot to the (right|left)."};
    constexpr auto next_state{"- Continue with state ([A-Z])."};
}

std::vector<std::string> read_lines(std::ifstream& file) {
    std::vector<std::string> puzzle_input;
    for(std::string line; std::getline(file, line); ) {
        line = utils::trim(line);
        if (!line.empty()) {
            puzzle_input.push_back(line);
        }
    }
    std::cout << std::size(puzzle_input) << std::endl;
    return puzzle_input;
}

std::optional<std::string> match_regex(const std::regex& re, const std::string& to_match) {
    std::smatch matches;
    if(std::regex_search(to_match, matches, re)) {
        if(matches.size() == 2) {
            return matches.str(1);
        }
    }
    return std::nullopt;
}

std::function<void(Machine&)> create_condition(bool value_to_write, int move_direction, char next_state) {
    return[value_to_write, move_direction, next_state](Machine& machine) {
        machine.tape[machine.cursor] = value_to_write;
        machine.cursor += move_direction;
        machine.state = next_state;
    };
}

Blueprint parse_blueprint(const std::vector<std::string>& lines) {
    static const std::regex begin_in_state_regex{state_patters::begin_in_state};
    static const std::regex number_of_steps_regex{state_patters::number_of_steps};
    static const std::regex state_name_regex{state_patters::state_name};
    static const std::regex current_value_regex{state_patters::current_value};
    static const std::regex write_value_regex{state_patters::write_value};
    static const std::regex move_direction_regex{state_patters::move_direction};
    static const std::regex next_state_regex{state_patters::next_state};

    char starting_state{};
    unsigned number_of_steps{};
    std::vector<char> state_names;
    std::queue<bool> current_values;
    std::queue<bool> values_to_write;
    std::queue<int> move_directions;
    std::queue<char> next_states;
    for(const auto& line: lines) {
        std::cout << line << std::endl;
        if(const auto re_match = match_regex(begin_in_state_regex, line); re_match) {
            std::cout << *re_match << std::endl;
            starting_state = (*re_match)[0];
            continue;
        }
        if(const auto re_match = match_regex(number_of_steps_regex, line); re_match) {
            number_of_steps = std::stoul(*re_match);
            continue;
        }
        if(const auto re_match = match_regex(state_name_regex, line); re_match) {
            const auto state_name = (*re_match)[0];
            state_names.push_back(state_name);
            continue;
        }
        if(const auto re_match = match_regex(current_value_regex, line); re_match) {
            const auto current_value = static_cast<bool>(std::stoi(*re_match));
            current_values.push(current_value);
            continue;
        }
        if(const auto re_match = match_regex(write_value_regex, line); re_match) {
            const auto value_to_write = static_cast<bool>(std::stoi(*re_match));
            values_to_write.push(value_to_write);
            continue;
        }
        if(const auto re_match = match_regex(move_direction_regex, line); re_match) {
            const auto move_direction = *re_match == "right" ? 1 : -1;
            move_directions.push(move_direction);
            continue;
        }
        if(const auto re_match = match_regex(next_state_regex, line); re_match) {
            const auto next_state = (*re_match)[0];
            next_states.push(next_state);
            continue;
        }
    }

    std::unordered_map<char, State> states;
    for(const auto state_name: state_names) {
        State state{};
        for(int i = 0; i < 2; ++i) {
            const auto current_value = current_values.front();
            current_values.pop();
            const auto condition = create_condition(values_to_write.front(), move_directions.front(), next_states.front());
            if(current_value) {
                state.on_one = condition;
            } else {
                state.on_zero = condition;
            }
            values_to_write.pop();
            move_directions.pop();
            next_states.pop();
        }
        states[state_name] = state;
    }
    std::cout << starting_state << " " << number_of_steps << " " << std::size(states) << std::endl;

    return Blueprint{
        starting_state,
        number_of_steps,
        states
    };
}

Blueprint read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    const auto lines = read_lines(file);
    return parse_blueprint(lines);
}

unsigned compute_checksum(const Machine& machine) {
    unsigned checksum{};
    for(const auto& [_, value]: machine.tape) {
        checksum += value;
    }
    return checksum;
}

// TODO: fix reference
unsigned solve_part_one(Blueprint blueprint) {
    Machine machine{};
    machine.state = blueprint.starting_state;
    for(unsigned step = 0; step < blueprint.number_of_steps; ++step) {
        if (machine.tape[machine.cursor]) {
            blueprint.states[machine.state].on_one(machine);
        } else {
            blueprint.states[machine.state].on_zero(machine);
        }
    }
    return compute_checksum(machine);
}

int main() {
    const auto blueprint = read_puzzle_input("input.txt");
    std::cout << solve_part_one(blueprint) << std::endl;
    return 0;
}