#include <knot_hashes.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <format>
#include <iomanip>

std::string read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::stringstream file_content;
    file_content << file.rdbuf();
    return file_content.str();
}

std::vector<unsigned> puzzle_input_to_numbers(const std::string& puzzle_input) {
    std::istringstream input_stream{puzzle_input};
    std::vector<unsigned> puzzle_input_as_numbers{};
    for(unsigned value; input_stream >> value; ) {
        puzzle_input_as_numbers.push_back(value);
        if (input_stream.peek() == ',') {
            input_stream.ignore();
        }
    }
    return puzzle_input_as_numbers;
}

std::string hash_to_hex(const std::vector<unsigned>& hash) {
    std::stringstream hex_hash;
    for(const auto& character: hash) {
        hex_hash << std::setfill('0') << std::setw(2) << std::hex << character;
    }
    return hex_hash.str();
}

unsigned solve_part_one(const std::string& puzzle_input) {
    const auto lengths = puzzle_input_to_numbers(puzzle_input);
    auto state = create_knot_hash_state();
    knot_hash_single_round(lengths, state);
    return state.numbers[0] * state.numbers[1];
}

std::string solve_part_two(const std::string& puzzle_input) {
    const auto lengths = prepare_input_lengths_as_ascii(puzzle_input);
    const auto sparse_hash = compute_sparse_hash(lengths);
    const auto dense_hash = compute_dense_hash(sparse_hash);
    return hash_to_hex(dense_hash);
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2: {}\n", solve_part_two(puzzle_input));
    return 0;
}