#include <iostream>
#include <fstream>
#include <vector>
#include <Utils.h>

/*
 * In this task, I use cube coordinates to keep track of child process position.
 * They are explained in detail here: https://www.redblobgames.com/grids/hexagons/
 * Basically it means that we look at our hex grid using three (instead of two)
 * coordinates. Coordinates are connected like this: x + y + z = 0
 * I calculate distance using following formula: d = max( abs(x2 - x1), abs(y2 - y1), abs(z2 - z1) )
*/
struct Coordinates {
    int x = 0;
    int y = 0;
    int z = 0;
};

std::vector<std::string> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    std::string input_line{};
    std::getline(file, input_line);
    return utils::split_by_delimiter(utils::trim(input_line), ",");
}

int distance_from_origin(const Coordinates& coordinates) {
    return std::max(std::abs(coordinates.x), std::max(std::abs(coordinates.y), std::abs(coordinates.z)));
}

Coordinates make_single_step(const Coordinates& position, const std::string& step_direction) {
    if(step_direction == "n") return {position.x + 1, position.y - 1, position.z };
    else if(step_direction == "s") return {position.x - 1, position.y + 1, position.z };
    else if(step_direction == "ne") return {position.x, position.y - 1, position.z + 1 };
    else if(step_direction == "nw") return {position.x + 1, position.y, position.z - 1 };
    else if(step_direction == "se") return {position.x - 1, position.y, position.z + 1 };
    else if(step_direction == "sw") return {position.x, position.y + 1, position.z - 1 };
    else throw std::runtime_error{"Invalid direction description"};
}

int solve_part_one(const std::vector<std::string>& steps) {
    Coordinates current_position{0, 0, 0};
    for(const auto& step: steps) {
        current_position = make_single_step(current_position, step);
    }
    return distance_from_origin(current_position);
}

int solve_part_two(const std::vector<std::string>& steps) {
    int max_distance = std::numeric_limits<int>::min();
    Coordinates current_position{0, 0, 0};
    for(const auto& step: steps) {
        current_position = make_single_step(current_position, step);
        max_distance = std::max(max_distance, distance_from_origin(current_position));
    }
    return max_distance;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << solve_part_one(puzzle_input) << std::endl;
    std::cout << "Part 2: " << solve_part_two(puzzle_input) << std::endl;
    return 0;
}