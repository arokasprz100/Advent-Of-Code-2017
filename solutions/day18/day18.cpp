#include <functional>
#include <string>
#include <variant>
#include <memory>
#include <fstream>
#include <iostream>
#include <common.h>
#include <format>

#include <instructions.h>
#include <sound_machine.h>
#include <parallel_machine.h>

std::vector<std::string> puzzle_input_from_file(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }

    std::vector<std::string> puzzle_input;
    for(std::string input_line{}; std::getline(file, input_line); ) {
        puzzle_input.push_back(input_line);
    }
    return puzzle_input;
}

Operand parse_operand(const std::string& operand) {
    if(utils::is_number(operand)) {
        return std::stoi(operand);
    }
    return operand;
}

template<typename Instruction, typename Snd, typename Rcv>
Instruction parse_single_value_instruction(const std::string& mnemonic, const Operand& value) {
    if (mnemonic == "snd") {
        return Snd{value};
    }
    else if (mnemonic == "rcv") {
        return Rcv{value};
    }
    throw std::runtime_error{"Invalid single-value instruction mnemonic"};
}

template<typename Instruction, typename Snd, typename Rcv>
Instruction parse_instruction(const std::string& instruction) {
    const auto parts = utils::split_by_delimiter(instruction, " ");
    if (parts.size() == 2) {
        const auto& mnemonic = parts.at(0);
        const auto operand = parse_operand(parts.at(1));
        return parse_single_value_instruction<Instruction, Snd, Rcv>(mnemonic, operand);
    }
    else if (parts.size() == 3) {
        const auto lhs = parse_operand(parts.at(1));
        const auto rhs = parse_operand(parts.at(2));
        if (const auto& mnemonic = parts.at(0); mnemonic == "set") {
            return Set{lhs, rhs};
        }
        else if (mnemonic == "add") {
            return Add{lhs, rhs};
        }
        else if (mnemonic == "mul") {
            return Multiply{lhs, rhs};
        }
        else if (mnemonic == "mod") {
            return Modulo{lhs, rhs};
        }
        else if (mnemonic == "jgz") {
            return JumpIfGreaterThanZero{lhs, rhs};
        }
    }
    throw std::runtime_error{"Invalid instruction syntax"};
}

template<typename Instruction, typename Snd, typename Rcv>
std::vector<Instruction> parse_program(const std::vector<std::string>& input) {
    std::vector<Instruction> program;
    for(const auto& line: input) {
        program.push_back(parse_instruction<Instruction, Snd, Rcv>(line));
    }
    return program;
}

ParallelMachineState create_parallel_machine_state(std::shared_ptr<SharedMachineState>& shared_state, int machine_id) {
    ParallelMachineState state{};
    state.shared_state = shared_state;
    state.machine_number = machine_id;
    state.registers["p"] = machine_id;
    return state;
}

int64_t solve_part_one(const std::vector<std::string>& input) {
    const auto program = parse_program<SoundMachineInstruction, PlaySound, RecoverSound>(input);
    SoundMachineState state{};
    const auto continue_execution = [&](){
        return state.recovery_count == 0 && !program_counter_out_of_bounds(state, program);
    };
    while(continue_execution()) {
        run_program_instruction(state, program);
    }
    return state.last_sound;
}

int64_t solve_part_two(const std::vector<std::string>& input) {
    const auto program = parse_program<ParallelMachineInstruction, Send, Receive>(input);
    auto shared_state = std::make_shared<SharedMachineState>();
    auto first_state = create_parallel_machine_state(shared_state, 0);
    auto second_state = create_parallel_machine_state(shared_state, 1);
    const auto continue_execution = [&]() {
        if (program_counter_out_of_bounds(first_state, program) && program_counter_out_of_bounds(second_state, program)) {
            return false;
        }
        return !shared_state->deadlock_detected();
    };
    while(continue_execution()) {
        run_program_instruction(first_state, program);
        run_program_instruction(second_state, program);
    }
    return second_state.number_of_sends;
}

int main() {
    const auto puzzle_input = puzzle_input_from_file("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(puzzle_input));
    return 0;
}
