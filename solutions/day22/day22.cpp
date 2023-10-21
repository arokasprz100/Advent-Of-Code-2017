#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <format>
#include <functional>

constexpr auto Number_Of_Bursts_First_Part{10'000};
constexpr auto Number_Of_Bursts_Second_Part{10'000'000};
using Point = std::pair<int, int>;

enum class NodeState : char {
    clean = 0,
    weakened = 1,
    infected = 2,
    flagged = 3
};

struct point_hash {
    std::size_t operator()(const Point& to_hash) const
    {
        // TODO: ensure if this is correct, switch to types with known width for whole program
        long long value = static_cast<long long>(to_hash.first) + (static_cast<long long>(INT_MAX) + 1) * static_cast<long long>(to_hash.second);
        return std::hash<long long>()(value);
    }
};

using MapType = std::unordered_map<Point, NodeState, point_hash>;

struct GridState {
    MapType nodes_infection_state;
    Point virus_position{0, 0};
    Point virus_direction{0, 0};
};

using InfectionStrategy = std::function<bool(GridState&)>;

std::vector<std::string> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::vector<std::string> puzzle_input{};
    for(std::string row; std::getline(file, row); ) {
        puzzle_input.push_back(row);
    }
    return puzzle_input;
}

std::pair<unsigned, unsigned> find_grid_center(const std::vector<std::string>& starting_grid) {
    const auto height = starting_grid.size();
    const auto wight = starting_grid[0].size();
    return {height / 2, wight / 2};
}

MapType init_grid(const std::vector<std::string>& starting_grid, const Point & grid_center) {
    MapType grid{};
    for(unsigned row_number{0}; row_number < std::size(starting_grid); ++row_number) {
        const auto& row = starting_grid[row_number];
        for(unsigned column_number{0}; column_number < std::size(row); ++column_number) {
            if(row[column_number] == '#') {
                grid[{row_number - grid_center.first, column_number - grid_center.second}] = NodeState::infected;
            }
        }
    }
    return grid;
}

Point turn_right(const Point& current_direction) {
    // {-1, 0} -> {0, 1} -> {1, 0} -> {0, -1}
    // x -> y;
    // y -> -x;
    return {current_direction.second, -current_direction.first};
}

Point turn_left(const Point& current_direction) {
    // {-1, 0} -> {0, -1} -> {1, 0} -> {0, 1}
    // x -> -y
    // y -> x
    return {-current_direction.second, current_direction.first};
}

Point turn_around(const Point& current_direction) {
    return {-current_direction.first, -current_direction.second};
}

bool simulate_single_burst(GridState& grid_state) {
    bool caused_infection{false};
    if(grid_state.nodes_infection_state[grid_state.virus_position] == NodeState::infected) {
        grid_state.virus_direction = turn_right(grid_state.virus_direction);
        grid_state.nodes_infection_state[grid_state.virus_position] = NodeState::clean;
    } else {
        grid_state.virus_direction = turn_left(grid_state.virus_direction);
        grid_state.nodes_infection_state[grid_state.virus_position] = NodeState::infected;
        caused_infection = true;
    }
    grid_state.virus_position.first += grid_state.virus_direction.first;
    grid_state.virus_position.second += grid_state.virus_direction.second;
    return caused_infection;
}

bool simulate_single_burst_for_evolved_virus(GridState& grid_state) {
    bool caused_infection{false};
    auto& current_state = grid_state.nodes_infection_state[grid_state.virus_position];
    switch(current_state) {
        case NodeState::infected:
            grid_state.virus_direction = turn_right(grid_state.virus_direction);
            current_state = NodeState::flagged;
            break;
        case NodeState::flagged:
            grid_state.virus_direction = turn_around(grid_state.virus_direction);
            current_state = NodeState::clean;
            break;
        case NodeState::clean:
            grid_state.virus_direction = turn_left(grid_state.virus_direction);
            current_state = NodeState::weakened;
            break;
        default:
            current_state = NodeState::infected;
            caused_infection = true;
            break;
    }
    grid_state.virus_position.first += grid_state.virus_direction.first;
    grid_state.virus_position.second += grid_state.virus_direction.second;
    return caused_infection;
}

unsigned simulate_virus(const std::vector<std::string>& starting_grid, const InfectionStrategy& infection_strategy, unsigned number_of_bursts) {
    const auto grid_center = find_grid_center(starting_grid);
    GridState grid_state {
        .nodes_infection_state{init_grid(starting_grid, grid_center)},
        .virus_position{0, 0},  // grid center
        .virus_direction{-1, 0} // up
    };
    unsigned infected{0};
    for(unsigned burst{0}; burst < number_of_bursts; ++burst) {
        if(infection_strategy(grid_state)) {
            ++infected;
        }
    }
    return infected;
}

unsigned solve_part_one(const std::vector<std::string>& starting_grid) {
    return simulate_virus(starting_grid, simulate_single_burst, Number_Of_Bursts_First_Part);
}

unsigned solve_part_two(const std::vector<std::string>& starting_grid) {
    return simulate_virus(starting_grid, simulate_single_burst_for_evolved_virus, Number_Of_Bursts_Second_Part);
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(puzzle_input));
    return 0;
}