#pragma once

#include <unordered_map>
#include <string>
#include <queue>
#include <variant>
#include <cstdint>

struct MachineState {
    int64_t program_counter{};
    std::unordered_map<std::string, int64_t> registers{};
};

// TODO: refactor to concepts
template<typename State, typename Instruction>
void run_program_instruction(State& state, const std::vector<Instruction>& program) {
    auto current_instruction = program.at(state.program_counter);
    std::visit([&](auto&& instruction){
        instruction.execute(state);
    }, current_instruction);
}

template<typename State, typename Instruction>
bool program_counter_out_of_bounds(State& state, const std::vector<Instruction>& program) {
    return state.program_counter < 0 || state.program_counter >= static_cast<int64_t>(program.size());
}