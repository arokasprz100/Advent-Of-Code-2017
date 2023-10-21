#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <format>
#include <ranges>
#include <cmath>

#include <common.h>


std::unordered_map<std::string, std::string> read_enhancement_rules(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::unordered_map<std::string, std::string> enhancement_rules;
    for(std::string line; std::getline(file, line); ) {
        const auto parts = utils::split_by_delimiter(line, " => ");
        enhancement_rules[utils::trim(parts.at(0))] = utils::trim(parts.at(1));
    }
    return enhancement_rules;
}

std::string extract_single_square(const std::vector<std::string>& pattern, unsigned square_size, unsigned y_square, unsigned x_square) {
    std::string extracted_square;
    unsigned row_number = 0;
    for(unsigned i = y_square * square_size; i < (y_square + 1) * square_size; ++i) {
        for(unsigned j = x_square * square_size; j < (x_square + 1) * square_size; ++j) {
            extracted_square += pattern.at(i).at(j);
        }
        if (row_number != square_size - 1) {
            extracted_square += '/';
        }
        ++row_number;
    }
    return extracted_square;
}

std::vector<std::string> split_pattern_to_squares(const std::vector<std::string>& pattern, unsigned square_size) {
    std::vector<std::string> squares;
    for(unsigned y = 0; y < std::size(pattern) / square_size; ++y) {
        for(unsigned x = 0; x < std::size(pattern) / square_size; ++x) {
            const auto square = extract_single_square(pattern, square_size, y, x);
            squares.push_back(square);
        }
    }
    return squares;
}


std::vector<std::string> apply_enhancement_rules(const std::vector<std::string>& squares, const std::unordered_map<std::string, std::string>& enhancement_rules) {
    std::vector<std::string> enhanced_squares;
    for(const auto& square: squares) {
        const auto& enhanced_square = enhancement_rules.at(square);
        enhanced_squares.push_back(enhanced_square);
    }
    return enhanced_squares;
}

std::vector<std::string> rebuild_pattern(const std::vector<std::string>& squares, unsigned square_size) {
    const auto new_pattern_side = static_cast<unsigned>(std::sqrt(std::size(squares))) * square_size;
    std::vector<std::string> pattern(new_pattern_side, "");
    for(unsigned i = 0; i < std::size(squares); ++i) {
        unsigned square_row = i / static_cast<unsigned>(std::sqrt(std::size(squares)));
        const auto square_parts = utils::split_by_delimiter(squares.at(i), "/");
        for(unsigned j = 0; j < std::size(square_parts); ++j) {
            pattern.at(j + square_size * square_row) += square_parts.at(j);
        }
    }
    return pattern;
}

std::string join_rows(const std::vector<std::string>& pattern_rows) {
    std::string result;
    for(unsigned i = 0; i < std::size(pattern_rows); ++i) {
        result += pattern_rows[i];
        if (i != std::size(pattern_rows) - 1) {
            result += "/";
        }
    }
    return result;
}

void reverse_rows(std::vector<std::string>& pattern_rows) {
    std::ranges::reverse(pattern_rows);
}

void reverse_columns(std::vector<std::string>& pattern_rows) {
    for(auto& row: pattern_rows) {
        std::ranges::reverse(row);
    }
}

void transpose(std::vector<std::string>& pattern_rows) {
    for (unsigned i = 0; i < std::size(pattern_rows); ++i) {
        for (unsigned j = i; j < std::size(pattern_rows); ++j) {
            std::swap(pattern_rows[i][j], pattern_rows[j][i]);
        }
    }
}

std::string flip_up(std::vector<std::string> pattern_rows) {
    reverse_rows(pattern_rows);
    return join_rows(pattern_rows);
}

std::string flip_right(std::vector<std::string> pattern_rows) {
    reverse_columns(pattern_rows);
    return join_rows(pattern_rows);
}

std::string rotate_180(std::vector<std::string> pattern_rows) {
    reverse_rows(pattern_rows);
    reverse_columns(pattern_rows);
    return join_rows(pattern_rows);
}

std::string rotate_90(std::vector<std::string> pattern_rows) {
    reverse_columns(pattern_rows);
    transpose(pattern_rows);
    return join_rows(pattern_rows);
}

// TODO: refactor those functions
std::string rotate_270(std::vector<std::string> pattern_rows) {
    reverse_columns(pattern_rows);
    transpose(pattern_rows);
    reverse_columns(pattern_rows);
    reverse_rows(pattern_rows);
    return join_rows(pattern_rows);
}

std::string flip_up_and_rotate_90(std::vector<std::string> pattern_rows) {
    reverse_rows(pattern_rows);
    reverse_columns(pattern_rows); // rotate
    transpose(pattern_rows);
    return join_rows(pattern_rows);
}

std::string flip_up_and_rotate_270(std::vector<std::string> pattern_rows) {
    reverse_rows(pattern_rows);
    reverse_columns(pattern_rows); // rotate
    transpose(pattern_rows);
    reverse_columns(pattern_rows);
    reverse_rows(pattern_rows);
    return join_rows(pattern_rows);
}

unsigned count_enabled_pixels(const std::vector<std::string>& pattern) {
    unsigned enabled_pixels = 0;
    for(const auto& row: pattern) {
        enabled_pixels += std::count(std::begin(row), std::end(row), '#');
    }
    return enabled_pixels;
}

// TODO: refactor this solution
std::vector<std::string> expand_rule_pattern(const std::string& pattern) {
    const auto pattern_rows = utils::split_by_delimiter(pattern, "/");
    return {
        pattern,
        flip_up(pattern_rows),
        flip_right(pattern_rows),
        rotate_180(pattern_rows),
        rotate_90(pattern_rows),
        rotate_270(pattern_rows),
        flip_up_and_rotate_90(pattern_rows),
        flip_up_and_rotate_270(pattern_rows)
    };
}

std::unordered_map<std::string, std::string> expand_enhancement_rules(const std::unordered_map<std::string, std::string>& rules) {
    std::unordered_map<std::string, std::string> expanded_rules;
    for(const auto& [pattern, value]: rules) {
        for(const auto& expanded_pattern: expand_rule_pattern(pattern)) {
            expanded_rules[expanded_pattern] = value;
        }
    }
    return expanded_rules;
}

unsigned run_enhancement(const std::unordered_map<std::string, std::string>& enhancement_rules, unsigned number_of_iterations) {
    std::vector<std::string> pattern = {".#.", "..#", "###"};
    for (unsigned i = 0; i < number_of_iterations; ++i) {
        const auto square_size = std::size(pattern) % 2 == 0 ? 2 : 3;
        const auto squares = split_pattern_to_squares(pattern, square_size);
        const auto enhanced_squares = apply_enhancement_rules(squares, enhancement_rules);
        pattern = rebuild_pattern(enhanced_squares, square_size + 1);
    }
    return count_enabled_pixels(pattern);
}

unsigned solve_part_one(const std::unordered_map<std::string, std::string>& enhancement_rules) {
    return run_enhancement(enhancement_rules, 5);
}

unsigned solve_part_two(const std::unordered_map<std::string, std::string>& enhancement_rules) {
    return run_enhancement(enhancement_rules, 18);
}

int main() {
    const auto enhancement_rules = read_enhancement_rules("input.txt");
    const auto expanded_enhancement_rules = expand_enhancement_rules(enhancement_rules);
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(expanded_enhancement_rules));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(expanded_enhancement_rules));
    return 0;
}