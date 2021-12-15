#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

using OffsetUpdatePolicy = std::function<int(int)>;

std::vector<int> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    std::vector<int> puzzle_input{};
    for(int offset{}; file >> offset; ) {
        puzzle_input.emplace_back(offset);
    }
    return puzzle_input;
}

inline int increase_offset_by_one(int offset) {
    return offset + 1;
}

inline int increase_or_decrease_offset_by_one(int offset) {
    return offset < 3 ? offset + 1 : offset - 1;
}

unsigned count_number_of_steps_to_reach_exit(const std::vector<int>& initial_state, const OffsetUpdatePolicy& calculate_new_offset_value) {
    int position{0};
    unsigned step_number{0};
    std::vector<int> offsets = initial_state;
    while(position < static_cast<int>(offsets.size()) && position >= 0) {
        int current_offset = offsets.at(position);
        offsets.at(position) = calculate_new_offset_value(current_offset);
        position += current_offset;
        ++step_number;
    }
    return step_number;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << count_number_of_steps_to_reach_exit(puzzle_input, increase_offset_by_one) << std::endl;
    std::cout << "Part 2: " << count_number_of_steps_to_reach_exit(puzzle_input, increase_or_decrease_offset_by_one) << std::endl;
    return 0;
}