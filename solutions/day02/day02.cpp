#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <numeric>

using Row = std::vector<unsigned>;
using Spreadsheet = std::vector<Row>;

Spreadsheet read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    Spreadsheet puzzle_input{};
    for(std::string input_line{}; std::getline(file, input_line); ) {
        std::stringstream input_line_stream{input_line};
        Row current_row{};
        for(unsigned input_entry{}; input_line_stream >> input_entry; ) {
            current_row.push_back(input_entry);
        }
        if(!current_row.empty()) {
            puzzle_input.push_back(current_row);
        }
    }
    return puzzle_input;
}

unsigned spreadsheet_checksum(const Spreadsheet& spreadsheet) {
    unsigned checksum{0};
    for(const auto& row: spreadsheet) {
        const auto[min_it, max_it] = std::minmax_element(std::begin(row), std::end(row));
        checksum += (*max_it - *min_it);
    }
    return checksum;
}

unsigned evenly_divisible_numbers_quotient(const Row& spreadsheet_row) {
    for(unsigned i = 0; i < spreadsheet_row.size() - 1; ++i) {
        for(unsigned j = i + 1; j < spreadsheet_row.size(); ++j) {
            unsigned greater = std::max(spreadsheet_row.at(i), spreadsheet_row.at(j));
            unsigned smaller = std::min(spreadsheet_row.at(i), spreadsheet_row.at(j));
            if(greater % smaller == 0) {
                return greater / smaller;
            }
        }
    }
    throw std::runtime_error{"Row contains no evenly divisible numbers"};
}

unsigned sum_evenly_divisible_numbers_quotients_for_each_row(const Spreadsheet& spreadsheet) {
    const auto accumulator = [](unsigned previous, const Row& current_row) {
        return previous + evenly_divisible_numbers_quotient(current_row);
    };
    return std::accumulate(std::begin(spreadsheet), std::end(spreadsheet), 0, accumulator);
}

int main(){
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << spreadsheet_checksum(puzzle_input) << std::endl;
    std::cout << "Part 2: " << sum_evenly_divisible_numbers_quotients_for_each_row(puzzle_input) << std::endl;
    return 0;
}
