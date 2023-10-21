#include <knot_hashes.h>

#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <set>
#include <stack>

constexpr auto Number_Of_Rows{128};
constexpr auto Grid_Side = Number_Of_Rows;

std::string read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::ostringstream file_content;
    file_content << file.rdbuf();
    return file_content.str();
}

unsigned count_set_bits(const std::vector<unsigned>& numbers) {
    unsigned number_of_set_bits{0};
    std::for_each(std::begin(numbers), std::end(numbers), [&](unsigned number) {
        number_of_set_bits += std::popcount(number);
    });
    return number_of_set_bits;
}

std::vector<unsigned> compute_dense_hash_for_row(const std::string& puzzle_input, unsigned row) {
    const auto row_input = std::format("{}-{}", puzzle_input, row);
    const auto row_input_as_ascii = prepare_input_lengths_as_ascii(row_input);
    const auto sparse_hash = compute_sparse_hash(row_input_as_ascii);
    return compute_dense_hash(sparse_hash);
}

std::string convert_dense_hash_to_binary_string(const std::vector<unsigned>& hash) {
    std::stringstream hash_stream{};
    for (const unsigned number: hash) {
        hash_stream << std::bitset<8>{number};
    }
    return hash_stream.str();
}

unsigned solve_part_one(const std::string& puzzle_input) {
    unsigned number_of_set_bits{0};
    for(unsigned row{0}; row < Number_Of_Rows; ++row) {
        const auto dense_hash = compute_dense_hash_for_row(puzzle_input, row);
        number_of_set_bits += count_set_bits(dense_hash);
    }
    return number_of_set_bits;
}

using Square = std::pair<int, int>;

struct RegionsState {
    std::set<Square> checked_squares;
    unsigned number_of_regions{0};
};

bool in_bounds(int to_check) {
    return to_check >= 0 && to_check < Number_Of_Rows;
}

bool in_bounds(const Square& to_check) {
    return in_bounds(to_check.first) && in_bounds(to_check.second);
}

void process_region(const Square& to_check, const std::vector<std::string>& grid, RegionsState& state) {
    std::stack<Square> squares_to_check{};
    squares_to_check.push(to_check);
    while (!squares_to_check.empty()) {
        const auto current_square = squares_to_check.top();
        squares_to_check.pop();

        if(!in_bounds(current_square) || state.checked_squares.contains(current_square)) {
            continue;
        }

        state.checked_squares.insert(current_square);
        if (grid[current_square.first][current_square.second] == '0') {
            continue;
        }

        std::vector<Square> adjacent_squares = {
            {current_square.first - 1, current_square.second},
            {current_square.first + 1, current_square.second},
            {current_square.first, current_square.second - 1},
            {current_square.first, current_square.second + 1}
        };
        for(const auto& square: adjacent_squares) {
            if (in_bounds(square) && grid[square.first][square.second] == '1') {
                squares_to_check.push(square);
            }
        }
    }
}

unsigned solve_part_two(const std::string& puzzle_input) {
    std::vector<std::string> dense_hashes_in_binary{};
    for(unsigned row{0}; row < Number_Of_Rows; ++row) {
        const auto dense_hash = compute_dense_hash_for_row(puzzle_input, row);
        const auto dense_hash_in_binary = convert_dense_hash_to_binary_string(dense_hash);
        dense_hashes_in_binary.push_back(dense_hash_in_binary);
    }
    RegionsState regions_state{};
    for(int i = 0; i < Grid_Side; ++i) {
        for(int j = 0; j < Grid_Side; ++j) {
            const Square current_square{i, j};
            if (regions_state.checked_squares.contains(current_square)) {
                continue;
            }
            if (dense_hashes_in_binary[i][j] == '0') {
                regions_state.checked_squares.insert(current_square);
                continue;
            }
            ++regions_state.number_of_regions;
            process_region(current_square, dense_hashes_in_binary, regions_state);
        }
    }
    return regions_state.number_of_regions;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(puzzle_input));
    return 0;
}