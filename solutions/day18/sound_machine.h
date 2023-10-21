#pragma once

#include <machine.h>
#include <instructions.h>

struct SoundMachineState: MachineState {
    int64_t recovery_count{};
    int64_t last_sound{};
};

class PlaySound {
public:
    explicit PlaySound(Operand operand): m_operand{std::move(operand)} {}

    void execute(SoundMachineState& state) {
        state.last_sound = resolve(m_operand, state);
        ++state.program_counter;
    }

private:
    Operand m_operand;
};

class RecoverSound {
public:
    explicit RecoverSound(Operand operand): m_operand{std::move(operand)} {}

    void execute(SoundMachineState& state) {
        if(resolve(m_operand, state) != 0) {
            ++state.recovery_count;
        }
        ++state.program_counter;
    }

private:
    Operand m_operand;
};

using SoundMachineInstruction = std::variant<Set, Add, Multiply, Modulo, JumpIfGreaterThanZero, PlaySound, RecoverSound>;
