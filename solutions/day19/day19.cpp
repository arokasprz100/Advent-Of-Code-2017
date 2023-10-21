#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <format>

std::vector<std::string> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::vector<std::string> diagram;
    for(std::string line; std::getline(file, line); ) {
        diagram.push_back(line);
    }
    return diagram;
}

int find_entrance(const std::string& first_row) {
    return static_cast<int>(first_row.find('|'));
}

bool out_of_bounds(const std::vector<std::string>& routing_diagram, const std::pair<int, int>& point) {
    if (point.first < 0 || point.first >= static_cast<int>(std::size(routing_diagram))) {
        return true;
    }
    return point.second < 0 || point.second >= static_cast<int>(std::size(routing_diagram[point.first]));
}

bool finished(const std::vector<std::string>& routing_diagram, const std::pair<int, int>& position) {
    return routing_diagram[position.first][position.second] == ' ';
}

std::pair<int, int> find_direction(const std::vector<std::string>& routing_diagram,
                                   const std::pair<int, int>& current_position,
                                   const std::pair<int, int>& current_direction) {
    const auto directions_to_check = std::vector<std::pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for(const auto& new_direction: directions_to_check) {
        if (current_direction.first == new_direction.first && current_direction.second == new_direction.second) {
            continue; // discard current direction
        }
        if(current_direction.first != 0 && (current_direction.first == -new_direction.first)) {
            continue; // discard opposite direction
        }
        if(current_direction.second != 0 && (current_direction.second == -new_direction.second)) {
            continue; // discard opposite direction
        }

        const std::pair<int, int> new_position{current_position.first + new_direction.first, current_position.second + new_direction.second};
        if(out_of_bounds(routing_diagram, new_position)) {
            continue; // discard out of bounds
        }
        if(routing_diagram[new_position.first][new_position.second] == '|' || routing_diagram[new_position.first][new_position.second] == '-') {
            return new_direction;
        }
    }
    throw std::runtime_error{"Could not find new direction"};
}

std::pair<std::string, unsigned> traverse_routing_diagram(const std::vector<std::string>& routing_diagram) {
    std::pair<int, int> current_position{0, find_entrance(routing_diagram[0])};
    std::pair<int, int> current_direction{1, 0}; // down
    std::string letters{};
    unsigned number_of_steps{0};
    while(!finished(routing_diagram, current_position)) {
        ++number_of_steps;
        const auto& [i, j] = current_position;
        if (routing_diagram[i][j] == '+') {
            current_direction = find_direction(routing_diagram, current_position, current_direction);
        }
        if (std::isalpha(routing_diagram[i][j])) {
            letters.push_back(routing_diagram[i][j]);
        }
        current_position = {i + current_direction.first, j + current_direction.second};
    }
    return {letters, number_of_steps};
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    const auto [found_letters, number_of_steps] = traverse_routing_diagram(puzzle_input);
    std::cout << std::format("Part 1 solution: {}\n", found_letters);
    std::cout << std::format("Part 2 solution: {}\n", number_of_steps);
    return 0;
}