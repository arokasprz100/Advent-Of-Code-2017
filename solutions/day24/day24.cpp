#include <iostream>
#include <fstream>
#include <vector>
#include <format>
#include <common.h>

struct Component {
    int pins_a{0};
    int pins_b{0};
    bool is_used{false};
};

std::vector<Component> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::vector<Component> puzzle_input;
    for(std::string line; std::getline(file, line); ) {
        const auto component_parts = utils::split_by_delimiter(line, "/");
        Component component{
            .pins_a = std::stoi(component_parts.at(0)),
            .pins_b = std::stoi(component_parts.at(1)),
            .is_used = false
        };
        puzzle_input.push_back(component);
    }
    return puzzle_input;
}

bool component_fits_the_port(const Component& component, int number_of_pins) {
    return component.pins_a == number_of_pins || component.pins_b == number_of_pins;
}

int get_next_port(const Component& component, int previous_port) {
    return component.pins_a == previous_port ? component.pins_b : component.pins_a;
}

int find_strongest_bridge(std::vector<Component>& components, int number_of_pins) {
    int max_strength_gain = 0;
    for(auto& component_to_fit: components) {
        if (component_to_fit.is_used || !component_fits_the_port(component_to_fit, number_of_pins)) {
            continue;
        }
        component_to_fit.is_used = true;
        const auto component_strength = component_to_fit.pins_a + component_to_fit.pins_b;
        const auto new_number_of_pins = get_next_port(component_to_fit, number_of_pins);
        const auto new_strength_gain = component_strength + find_strongest_bridge(components,new_number_of_pins);
        max_strength_gain = std::max(new_strength_gain, max_strength_gain);
        component_to_fit.is_used = false;
    }
    return max_strength_gain;
}

std::pair<int, int> find_longest_bridge(std::vector<Component>& components, int number_of_pins) {
    int max_strength_gain = 0;
    int max_length_gain = 0;
    for(auto& component_to_fit: components) {
        if (component_to_fit.is_used || !component_fits_the_port(component_to_fit, number_of_pins)) {
            continue;
        }

        component_to_fit.is_used = true;
        const auto component_strength = component_to_fit.pins_a + component_to_fit.pins_b;
        const auto new_number_of_pins = get_next_port(component_to_fit, number_of_pins);

        auto [new_strength_gain, new_length_gain] = find_longest_bridge(components,new_number_of_pins);
        new_strength_gain += component_strength;    // include current component
        new_length_gain += 1;                       // include current component

        if (new_length_gain > max_length_gain) {
            max_length_gain = new_length_gain;
            max_strength_gain = new_strength_gain;
        }
        else if (new_length_gain == max_length_gain) {
            if (new_strength_gain > max_strength_gain) {
                max_length_gain = new_length_gain;
                max_strength_gain = new_strength_gain;
            }
        }

        component_to_fit.is_used = false;
    }
    return {max_strength_gain, max_length_gain};
}

int solve_part_one(std::vector<Component> components) {
    return find_strongest_bridge(components, 0);
}

int solve_part_two(std::vector<Component> components) {
    const auto [max_strength, _] = find_longest_bridge(components, 0);
    return max_strength;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(puzzle_input));
    return 0;
}