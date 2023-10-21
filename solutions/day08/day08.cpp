#include <iostream>
#include <fstream>
#include <map>
#include <limits>
#include <functional>
#include <common.h>

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

    void run(const std::vector<CPUInstruction>& instructions) {
        for(const auto& instruction: instructions) {
            run(instruction);
        }
    }

    bool run(const CPUInstruction& instruction) {
        const auto modification = register_modifying_operations_by_mnemonics.at(instruction.modifying_operation_mnemonic);
        const auto comparison = comparison_operations_by_mnemonic.at(instruction.comparison_operation_mnemonic);
        if(comparison(registers[instruction.comparison_register], instruction.comparison_operation_argument)) {
            modification(registers[instruction.destination_register], instruction.modifying_operation_argument);
            return true;
        }
        return false;
    }

    [[nodiscard]]
    int register_value(const std::string& register_name) const {
        return registers.at(register_name);
    }

    [[nodiscard]]
    std::map<std::string, int>::const_iterator begin() const {
        return registers.cbegin();
    }

    [[nodiscard]]
    std::map<std::string, int>::const_iterator end() const {
        return registers.cend();
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

int solve_part_one(const std::vector<CPUInstruction>& instructions) {
    const auto registers_values_comparator = [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    };
    CPU cpu{};
    cpu.run(instructions);
    return std::max_element(std::begin(cpu), std::end(cpu), registers_values_comparator)->second;
}

int solve_part_two(const std::vector<CPUInstruction>& instructions) {
    CPU cpu{};
    int max_value = std::numeric_limits<int>::min();
    for(const auto& instruction: instructions) {
        bool instruction_executed = cpu.run(instruction);
        if(instruction_executed) {
            max_value = std::max(max_value, cpu.register_value(instruction.destination_register));
        }
    }
    return max_value;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << solve_part_one(puzzle_input) << std::endl;
    std::cout << "Part 2: " << solve_part_two(puzzle_input) << std::endl;
    return 0;
}