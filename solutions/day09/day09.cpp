#include <iostream>
#include <fstream>
#include <format>
#include <sstream>
#include <common.h>

std::string read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::ostringstream file_content;
    file_content << file.rdbuf();
    return utils::trim(file_content.str());
}

std::pair<int, int> process_character_stream(const std::string& character_stream) {
    bool ignore_next_character{false};
    bool processing_garbage{false};
    int current_depth{0};
    int total_score{0};
    int characters_in_garbage{0};
    for (const auto character: character_stream) {
        if (ignore_next_character) {
            ignore_next_character = false;
            continue;
        }

        if (character == '!') {
            ignore_next_character = true;
            continue;
        }

        // any character in garbage is garbage (including '<'), excluding garbage
        // cancellation and cases above
        if (processing_garbage && character != '>') {
            ++characters_in_garbage;
            continue;
        }

        if (character == '>') {
            processing_garbage = false;
            continue;
        }

        if (character == '<') {
            processing_garbage = true;
            continue;
        }

        if (character == '{') {
            total_score += ++current_depth;
        }

        if (character == '}') {
            --current_depth;
        }
    }
    return std::make_pair(total_score, characters_in_garbage);
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    const auto [total_score, character_in_garbage] = process_character_stream(puzzle_input);
    std::cout << std::format("Part 1 solution: {}\n", total_score);
    std::cout << std::format("Part 2 solution: {}\n", character_in_garbage);
    return 0;
}