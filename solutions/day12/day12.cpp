#include <iostream>
#include <fstream>
#include <format>
#include <map>
#include <set>

#include <common.h>

std::map<int, std::vector<int>>  read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::map<int, std::vector<int>> program_connections;
    for (std::string line; std::getline(file, line); ) {
        const auto parts = utils::split_by_delimiter(line, "<->");
        const auto source_program = std::stoi(parts.at(0));
        const auto target_programs = utils::split_by_delimiter(parts.at(1), ", ");
        for(const auto& target_program : target_programs) {
            program_connections[source_program].push_back(std::stoi(target_program));
        }
    }
    return program_connections;
}

std::set<int> get_all_programs_in_group(int starting_program, const std::map<int, std::vector<int>>& program_connections) {
    std::set<int> programs_in_group;
    std::stack<int> programs_to_process;
    programs_to_process.push(starting_program);
    while(!programs_to_process.empty()) {
        const auto current_program = programs_to_process.top();
        programs_to_process.pop();
        programs_in_group.insert(current_program);
        const auto& connected_programs = program_connections.at(current_program);
        std::for_each(std::begin(connected_programs), std::end(connected_programs), [&](int program){
            if (!programs_in_group.contains(program)) {
                programs_to_process.push(program);
            }
        });
    }
    return programs_in_group;
}

unsigned count_program_zero_group_members(const std::map<int, std::vector<int>>& program_connections) {
    return std::size(get_all_programs_in_group(0, program_connections));
}

unsigned count_number_of_groups(const std::map<int, std::vector<int>>& program_connections) {
    std::set<int> already_processed;
    unsigned number_of_groups = 0;
    for(const auto& [program, _]: program_connections) {
        if (!already_processed.contains(program)) {
            ++number_of_groups;
            const auto programs_in_group = get_all_programs_in_group(program, program_connections);
            already_processed.insert(std::begin(programs_in_group), std::end(programs_in_group));
        }
    }
    return number_of_groups;
}

int main() {
    const auto program_connections = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", count_program_zero_group_members(program_connections));
    std::cout << std::format("Part 2 solution: {}\n", count_number_of_groups(program_connections));
    return 0;
}