#include <iostream>
#include <fstream>
#include <array>
#include <map>
#include <vector>
#include <numeric>

using Point = std::pair<int, int>;

int load_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    int puzzle_input{0};
    file >> puzzle_input;
    return puzzle_input;
}

unsigned manhattan_distance(const Point& point_a, const Point& point_b) {
    auto x_distance = static_cast<unsigned>(std::abs(point_a.first - point_b.first));
    auto y_distance = static_cast<unsigned>(std::abs(point_a.second - point_b.second));
    return x_distance + y_distance;
}

Point move_point_by_vector(const Point& point, const Point& vector) {
    return {point.first + vector.first, point.second + vector.second};
}

std::vector<Point> get_neighbours(const Point& position) {
    std::vector<Point> neighbours{};
    for(int i = -1; i <= 1; ++i) {
        for(int j = -1; j <= 1; ++j) {
            if(i == 0 && j == 0) continue;
            neighbours.emplace_back(position.first + i, position.second + j);
        }
    }
    return neighbours;
}

template<typename NewValueCalculationPolicy>
int move_until_value_reached_or_exceeded(Point& position, int desired_value, const NewValueCalculationPolicy& calculate_new_value) {
    std::array<Point, 4> direction_vectors = {{ {1, 0}, {0, 1}, {-1, 0}, {0, -1} }};
    auto[current_value, step_size, remaining_step, direction] = std::tuple{1, 1, 1, 0};
    while(current_value < desired_value) {
        position = move_point_by_vector(position, direction_vectors.at(direction));
        current_value = calculate_new_value(current_value);
        --remaining_step;
        if(remaining_step == 0) {
            direction = (direction + 1) % 4;
            if(direction % 2 == 0) {
                ++step_size;
            }
            remaining_step = step_size;
        }
    }
    return current_value;
}

unsigned solve_part_one(int puzzle_input) {
    Point position{0, 0};
    move_until_value_reached_or_exceeded(position, puzzle_input, [](int value) { return value + 1; });
    return manhattan_distance({0, 0}, position);
}

unsigned solve_part_two(int puzzle_input) {
    Point position{0, 0};
    std::map<Point, int> previous_values{ {position, 1} };
    const auto next_value_calculator = [&position, &previous_values](unsigned) {
        const auto neighbours = get_neighbours(position);
        int value = std::accumulate(std::begin(neighbours), std::end(neighbours), 0, [&previous_values](int sum, const auto& entry) {
            return sum + previous_values[entry];
        });
        previous_values[position] = value;
        return value;
    };
    return move_until_value_reached_or_exceeded(position, puzzle_input, next_value_calculator);
}

int main() {
    const auto puzzle_input = load_puzzle_input("input.txt");
    std::cout << "Part 1: " << solve_part_one(puzzle_input) << std::endl;
    std::cout << "Part 2: " << solve_part_two(puzzle_input) << std::endl;
    return 0;
}