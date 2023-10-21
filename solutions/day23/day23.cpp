#include <iostream>
#include <fstream>
#include <utility>
#include <variant>
#include <format>

#include <common.h>
#include "../day18/instructions.h"


using Instruction = std::variant<Set, Subtract, CountedMultiply, JumpIfNotZero>;

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

// TODO: this is reused from day18, move to common place
Operand parse_operand(const std::string& operand) {
    if(utils::is_number(operand)) {
        return std::stoi(operand);
    }
    return operand;
}

Instruction parse_instruction(const std::string& instruction) {
    const auto parts = utils::split_by_delimiter(instruction, " ");
    if (parts.size() == 3) {
        const auto lhs = parse_operand(parts.at(1));
        const auto rhs = parse_operand(parts.at(2));
        if (const auto& mnemonic = parts.at(0); mnemonic == "set") {
            return Set{lhs, rhs};
        }
        else if (mnemonic == "sub") {
            return Subtract{lhs, rhs};
        }
        else if (mnemonic == "mul") {
            return CountedMultiply{lhs, rhs};
        }
        else if (mnemonic == "jnz") {
            return JumpIfNotZero{lhs, rhs};
        }
    }
    throw std::runtime_error{"Invalid instruction syntax"};
}

std::vector<Instruction> parse_program(const std::vector<std::string>& input) {
    std::vector<Instruction> program;
    for(const auto& line: input) {
        program.push_back(parse_instruction(line));
    }
    return program;
}


// TODO: describe how to solve without running program
unsigned solve_part_one(const std::vector<std::string>& program) {
    const auto instructions = parse_program(program);
    MachineState state{};
    const auto continue_execution = [&](){
        return !program_counter_out_of_bounds(state, program);
    };
    while(continue_execution()) {
        run_program_instruction(state, instructions);
    }
    return CountedMultiply::get_number_of_invokes();
}

unsigned solve_part_two() {
    // we ignore following instructions, since for part 2, a == 1:
    // jnz a 2
    // jnz 1 5
    // the way this works is it reduces number of iterations (skips next lines of
    // code that change b and c) if a == 0

    // set b 84
    // set c b
    // ... ignored instructions
    // mul b 100        <- ignore, is just b = b * 100
    // sub b -100000    <- b = (b * 100) + 100000
    // set c b          <- ignore, is just c = b
    // sub c -17000     <- c = b + 17000
    // this basically sets b = (84 * 100) + 100000 = 125400 and c = b + 17000 = 125400
    // all other registers were used to store temporary values, so are not needed
    int64_t register_b = (84 * 100) + 100000; // 125400
    int64_t register_c = register_b + 17000; // 125400
    int64_t register_h = 0; // starts at 0

    do {
        // so we have following code here
        // set f 1
        // set d 2
        // set e 2
        // set g d   <- ignore, is just g = d
        // mul g e   <- ignore, is just g = d * e
        // sub g b   <- ignore, is just g = (d * e) - b
        // jnz g 2   <- if (d * e != b), skip next line
        // set f 0   <- sets f to 0, if b = (d * e)
        // sub e -1  <- ++e
        // set g e   <- ignore, is just g = e
        // sub g b   <- ignore, is just g = e - b
        // jnz g -8  <- if (e != b), return to (set g d) - loop on e
        // sub d -1  <- ++d
        // set g d   <- ignore, is just g = d
        // sub g b   <- ignore, is just g = d - b
        // jnz g -13 <- if (d != b), return to (set e 2) - loop on d
        // what is does is it takes d = 2 and e = 2, and does nested loops on them (both until equal b)
        // if we find pair of d and e, so that e * d == b, we set f to 0
        if (!utils::is_prime(register_b)) {
            // jnz f 2   <- skips incrementing h if j != 0 (if pair e * d == b not found)
            // sub h -1  <- increments h by 1
            ++register_h;
        }

        // loop condition logic:
        // set g b
        // sub g c     <- g = b - c
        // jnz g 2     <- if b != c, skip next instruction
        // jnz 1 3     <- terminates the program (by jumping out if it)
        // sub b -17   <- b += 17
        // jnz 1 -23
        if (register_b == register_c) {
            break;          // jnz 1 3
        }
        register_b += 17;   // sub b -17
    } while (true);         // jnz 1 -23
    return register_h;
}

int main() {
    const auto puzzle_input = puzzle_input_from_file("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two());
    return 0;
}