#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <format>

unsigned read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    unsigned input{};
    file >> input;
    return input;
}

size_t move_forward(size_t current_index, size_t buffer_size, unsigned number_of_steps) {
    return (current_index + number_of_steps) % buffer_size;
}

size_t insert_element(size_t current_index, std::list<unsigned>& buffer, unsigned value) {
    auto position = std::begin(buffer);
    std::advance(position, current_index + 1); // must insert after position, not before
    buffer.insert(position, value);
    return current_index + 1;
}

unsigned get_value_after_last_inserted(unsigned current_index, const std::list<unsigned>& buffer) {
    auto position = std::begin(buffer);
    std::advance(position, current_index + 1);
    return *position;
}

size_t spinlock(std::list<unsigned>& buffer, unsigned step_size, unsigned number_of_steps) {
    size_t current_index = 0;
    for(unsigned i = 0; i < number_of_steps; ++i) {
        current_index = move_forward(current_index, std::size(buffer), step_size);
        current_index = insert_element(current_index, buffer, i + 1);
    }
    return current_index;
}

unsigned solve_part_one(unsigned input) {
    std::list<unsigned> circular_buffer{0};
    size_t current_index = spinlock(circular_buffer, input, 2017);
    return get_value_after_last_inserted(current_index, circular_buffer);
}

unsigned solve_part_two(unsigned input) {
    size_t current_index = 0;
    unsigned last_value_after_zero = 0;
    for(unsigned buffer_size = 1; buffer_size <= 50'000'000; ++buffer_size) {
        current_index = move_forward(current_index, buffer_size, input);
        if (current_index == 0) {
            // value to insert is equal to current buffer size (size before new element is inserted)
            last_value_after_zero = buffer_size;
        }
        ++current_index; // value is inserted, move current position by one forward
    }
    return last_value_after_zero;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(puzzle_input));
    return 0;
}
