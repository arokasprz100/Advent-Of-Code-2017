#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <optional>
#include <common.h>

struct TowerOfPrograms {
    std::map<std::string, unsigned> weights_by_programs{};
    std::multimap<std::string, std::string> programs_by_supporters{};
};

std::pair<std::string, unsigned> parse_program_name_and_weight(const std::string& input) {
    const auto parts = utils::split_by_delimiter(utils::trim(input), " (");
    return std::make_pair(parts.at(0), std::stoul(parts.at(1)));
}

std::vector<std::string> parse_programs_list(const std::string& input) {
    return utils::split_by_delimiter(utils::trim(input), ", ");
}

TowerOfPrograms read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    TowerOfPrograms tower_of_programs{};
    for(std::string input_line{}; std::getline(file, input_line); ) {
        if(input_line.find("->") != std::string::npos) {
            const auto input_parts = utils::split_by_delimiter(input_line, "->");
            const auto base_program = parse_program_name_and_weight(input_parts.at(0));
            const auto supported_programs = parse_programs_list(input_parts.at(1));
            tower_of_programs.weights_by_programs.insert(base_program);
            for(const auto& supported_program: supported_programs) {
                tower_of_programs.programs_by_supporters.insert(std::make_pair(base_program.first, supported_program));
            }
        }
        else {
            const auto base_program = parse_program_name_and_weight(input_line);
            tower_of_programs.weights_by_programs.insert(base_program);
        }
    }
    return tower_of_programs;
}

std::string first_not_supported(const std::set<std::string>& programs, const std::set<std::string>& supported_programs) {
    const auto not_in_supported = [&supported_programs](const std::string& program) {
        return !supported_programs.contains(program);
    };
    const auto not_supported_it = std::find_if(std::begin(programs), std::end(programs), not_in_supported);
    if(not_supported_it == std::end(programs)) {
        throw std::runtime_error{"There are not programs that are not supported by others, possible cycle."};
    }
    return *not_supported_it;
}

std::string find_base_program(const TowerOfPrograms& tower_of_programs) {
    std::set<std::string> base_programs{};
    std::set<std::string> supported_programs{};
    for(const auto& [base, supported]: tower_of_programs.programs_by_supporters) {
        supported_programs.insert(supported);
        base_programs.insert(base);
    }
    return first_not_supported(base_programs, supported_programs);
}

unsigned program_stack_weight(const std::string& base_program_name, const TowerOfPrograms& tower_of_programs) {
    unsigned stack_weight = 0;
    std::vector<std::string> programs_to_process{base_program_name};
    while(!programs_to_process.empty()) {
        const auto current_program = programs_to_process.back();
        programs_to_process.pop_back();
        stack_weight += tower_of_programs.weights_by_programs.at(current_program);
        const auto supported_range = tower_of_programs.programs_by_supporters.equal_range(current_program);
        for(auto it = supported_range.first; it != supported_range.second; ++it) {
            programs_to_process.push_back(it->second);
        }
    }
    return stack_weight;
}

std::vector<std::string> program_disc_content(const std::string& program, const TowerOfPrograms& tower_of_programs) {
    const auto[start_it, end_it] = tower_of_programs.programs_by_supporters.equal_range(program);
    std::vector<std::string> disc_content{};
    for(auto it = start_it; it != end_it; ++it) {
        disc_content.emplace_back(it->second);
    }
    return disc_content;
}

bool is_program_disc_balanced(const std::string& base_program_name, const TowerOfPrograms& tower_of_programs) {
    const auto weight_calculator = [&tower_of_programs](const std::string& program) {
        return program_stack_weight(program, tower_of_programs);
    };
    const auto disc_content = program_disc_content(base_program_name, tower_of_programs);
    std::vector<unsigned> disc_weights{};
    std::transform(std::begin(disc_content), std::end(disc_content), std::back_inserter(disc_weights), weight_calculator);
    auto unequal_weight_it = std::adjacent_find(std::begin(disc_weights), std::end(disc_weights), std::not_equal_to<>());
    return unequal_weight_it == std::end(disc_weights);
}

std::optional<std::string> unbalanced_program_on_disc(const std::string& base_program_name, const TowerOfPrograms& tower_of_programs) {
    const auto disc_content = program_disc_content(base_program_name, tower_of_programs);
    for(const auto& program: disc_content) {
        if(!is_program_disc_balanced(program, tower_of_programs)) {
            return program;
        }
    }
    return std::nullopt;
}

std::map<unsigned, unsigned> program_stack_weights_histogram(const std::vector<std::string>& programs, const TowerOfPrograms& tower_of_programs) {
    std::map<unsigned, unsigned> weight_counts{};
    for(const auto& program: programs) {
        ++weight_counts[program_stack_weight(program, tower_of_programs)];
    }
    return weight_counts;
}

unsigned proper_weight_on_disc(const std::vector<std::string>& disc_content, const TowerOfPrograms& tower_of_programs) {
    const auto weight_counts_comparator = [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    };
    std::map<unsigned, unsigned> weight_counts = program_stack_weights_histogram(disc_content, tower_of_programs);
    auto most_frequent_weight_it = std::max_element(std::begin(weight_counts), std::end(weight_counts), weight_counts_comparator);
    return most_frequent_weight_it->first;
}

std::string solve_part_one(const TowerOfPrograms& tower_of_programs) {
    return find_base_program(tower_of_programs);
}

int solve_part_two(const TowerOfPrograms& tower_of_programs) {
    std::string current_program = find_base_program(tower_of_programs);
    while(true) {
        const auto maybe_unbalanced_on_disc = unbalanced_program_on_disc(current_program, tower_of_programs);
        if(maybe_unbalanced_on_disc.has_value()) {
            current_program = maybe_unbalanced_on_disc.value();
        }
        else {
            const auto disc_content = program_disc_content(current_program, tower_of_programs);
            const auto proper_weight = proper_weight_on_disc(disc_content, tower_of_programs);
            for(const auto& program: disc_content) {
                unsigned stack_weight = program_stack_weight(program, tower_of_programs);
                if(stack_weight != proper_weight) {
                    int weight_correction = static_cast<int>(proper_weight) - static_cast<int>(stack_weight);
                    return static_cast<int>(tower_of_programs.weights_by_programs.at(program)) + weight_correction;
                }
            }
            throw std::runtime_error{"There are no unbalanced programs in the tower."};
        }
    }
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << solve_part_one(puzzle_input) << std::endl;
    std::cout << "Part 2: " << solve_part_two(puzzle_input) << std::endl;
    return 0;
}