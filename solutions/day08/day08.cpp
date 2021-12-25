#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <Utils.h>

struct CPUInstruction {
    std::string destination_register{};
    std::string modifying_operation_mnemonic{};
    int modifying_operation_argument{};
    std::string comparison_register{};
    std::string comparison_operation_mnemonic{};
    int comparison_operation_argument{};
};

class CPU {
public:
    using RegisterModifyingOperation = std::function<void(int&, int)>;
    using ComparisonOperation = std::function<bool(int, int)>;

    int run_instructions_and_get_max_final_value(const std::vector<CPUInstruction>& instructions) {
        const auto registers_values_comparator = [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        };
        registers.clear();
        for(const auto& instruction: instructions) {
            run_instruction(instruction);
        }
        return std::max_element(std::begin(registers), std::end(registers), registers_values_comparator)->second;
    }

    int run_instructions_and_get_max_total_value(const std::vector<CPUInstruction>& instructions) {
        registers.clear();
        int max_value = std::numeric_limits<int>::min();
        for(const auto& instruction: instructions) {
            run_instruction(instruction);
            max_value = std::max(max_value, registers[instruction.destination_register]);
        }
        return max_value;
    }

private:
    std::map<std::string, int> registers{};

    const std::map<std::string, RegisterModifyingOperation> register_modifying_operations_by_mnemonics {
        { "inc", [](int& lhs, int rhs) { lhs += rhs; } },
        { "dec", [](int& lhs, int rhs) { lhs -= rhs; } }
    };

    const std::map<std::string, ComparisonOperation> comparison_operations_by_mnemonic {
        { "<", [](int lhs, int rhs) { return lhs < rhs; } },
        { ">", [](int lhs, int rhs) { return lhs > rhs; } },
        { "<=", [](int lhs, int rhs) { return lhs <= rhs; } },
        { ">=", [](int lhs, int rhs) { return lhs >= rhs; } },
        { "==", [](int lhs, int rhs) { return lhs == rhs; } },
        { "!=", [](int lhs, int rhs) { return lhs != rhs; } }
    };

    void run_instruction(const CPUInstruction& instruction) {
        const auto modification = register_modifying_operations_by_mnemonics.at(instruction.modifying_operation_mnemonic);
        const auto comparison = comparison_operations_by_mnemonic.at(instruction.comparison_operation_mnemonic);
        if(comparison(registers[instruction.comparison_register], instruction.comparison_operation_argument)) {
            modification(registers[instruction.destination_register], instruction.modifying_operation_argument);
        }
    }
};

std::vector<CPUInstruction> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    std::vector<CPUInstruction> instructions{};
    for(std::string input_line{}; std::getline(file, input_line); ) {
        const auto instruction_parts = utils::split_by_delimiter(utils::trim(input_line), " ");
        CPUInstruction instruction {instruction_parts.at(0), instruction_parts.at(1), std::stoi(instruction_parts.at(2)),
                                    instruction_parts.at(4), instruction_parts.at(5), std::stoi(instruction_parts.at(6)) };
        instructions.emplace_back(instruction);
    }
    return instructions;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << CPU{}.run_instructions_and_get_max_final_value(puzzle_input) << std::endl;
    std::cout << "Part 2: " << CPU{}.run_instructions_and_get_max_total_value(puzzle_input) << std::endl;
    return 0;
}