#include <algorithm>
#include <fstream>
#include <format>
#include <iostream>
#include <map>
#include <set>
#include <vector>


std::vector<int> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not read input file"};
    }
    std::vector<int> puzzle_input{};
    for(int input{0}; file >> input; ) {
        puzzle_input.emplace_back(input);
    }
    return puzzle_input;
}

void next_step(std::vector<int>& memory) {
    const auto max_elem_it = std::max_element(std::begin(memory), std::end(memory));
    const auto to_distribute = *max_elem_it;
    *max_elem_it = 0;
    size_t index_to_start = (std::distance(std::begin(memory), max_elem_it) + 1) % std::size(memory);
    for(int i{0}; i < to_distribute; ++i) {
        size_t current_index = (index_to_start + i) % std::size(memory);
        ++memory[current_index];
    }
}

int solve_part_one(std::vector<int> memory) {
    int step_number{0};
    std::set<std::vector<int>> states{};
    do {
        states.insert(memory);
        ++step_number;
        next_step(memory);
    } while (!states.contains(memory));
    return step_number;
}

int solve_part_two(std::vector<int> memory) {
    int step_number{0};
    std::map<std::vector<int>, int> states_with_step_numbers{};
    do {
        states_with_step_numbers[memory] = step_number++;
        next_step(memory);
    } while (!states_with_step_numbers.contains(memory));
    return step_number - states_with_step_numbers[memory];
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(puzzle_input));
    return 0;
}