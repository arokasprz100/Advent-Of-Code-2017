#include <iostream>
#include <fstream>
#include <vector>

std::vector<unsigned> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    std::vector<unsigned> puzzle_input{};
    for(char input_character{}; file >> input_character; ) {
        puzzle_input.push_back(static_cast<unsigned>(input_character - '0'));
    }
    return puzzle_input;
}

unsigned sum_of_digits_that_match_next_digit(const std::vector<unsigned>& digit_sequence) {
    unsigned sum{0};
    for(unsigned index = 0; index < digit_sequence.size(); ++index) {
        unsigned next_digit_index = (index + 1) % digit_sequence.size();
        if(digit_sequence.at(index) == digit_sequence.at(next_digit_index)) {
            sum += digit_sequence.at(index);
        }
    }
    return sum;
}

unsigned sum_of_digits_that_match_digit_halfway_around_sequence(const std::vector<unsigned>& digit_sequence) {
    unsigned sum{0};
    for(unsigned index = 0; index < digit_sequence.size(); ++index) {
        unsigned digit_halfway_around_index = (index + digit_sequence.size() / 2) % digit_sequence.size();
        if(digit_sequence.at(index) == digit_sequence.at(digit_halfway_around_index)) {
            sum += digit_sequence.at(index);
        }
    }
    return sum;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << sum_of_digits_that_match_next_digit(puzzle_input) << std::endl;
    std::cout << "Part 2: " << sum_of_digits_that_match_digit_halfway_around_sequence(puzzle_input) << std::endl;
    return 0;
}
