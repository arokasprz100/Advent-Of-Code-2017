#pragma once

#include <variant>
#include <functional>

#include "machine.h"


using Operand = std::variant<std::string, int64_t>;

int64_t& resolve(Operand& operand, MachineState& state) {
    if(std::holds_alternative<std::string>(operand)) {
        return state.registers[std::get<std::string>(operand)];
    }
    return std::get<int64_t>(operand);
}


class Set {
public:
    Set(Operand lhs, Operand rhs): m_lhs{std::move(lhs)}, m_rhs{std::move(rhs)} {}

    void execute(MachineState& state) {
        resolve(m_lhs, state) = resolve(m_rhs, state);
        ++state.program_counter;
    }

private:
    Operand m_lhs;
    Operand m_rhs;
};

template<typename Operation>
class ArithmeticInstruction {
public:
    ArithmeticInstruction(Operand lhs, Operand rhs): m_lhs{std::move(lhs)}, m_rhs{std::move(rhs)} {}

    void execute(MachineState& state) {
        auto& resolved_lhs = resolve(m_lhs, state);
        resolved_lhs = Operation{}(resolved_lhs, resolve(m_rhs, state));
        ++state.program_counter;
    }

private:
    Operand m_lhs;
    Operand m_rhs;
};

template<typename Comparator>
class JumpInstruction {
public:
    JumpInstruction(Operand to_check, Operand offset):
            m_to_check{std::move(to_check)},
            m_offset{std::move(offset)} {}

    void execute(MachineState& state) {
        if (Comparator{}(resolve(m_to_check, state), 0)) {
            state.program_counter += resolve(m_offset, state);
        } else {
            ++state.program_counter;
        }
    }

private:
    Operand m_to_check;
    Operand m_offset;
};

template<typename Operation>
class CountedInstruction {
public:
    CountedInstruction(Operand lhs, Operand rhs): m_underlying{std::move(lhs), std::move(rhs)} {}

    void execute(MachineState& state) {
        m_underlying.execute(state);
        ++number_of_invokes;
    }

    static unsigned get_number_of_invokes() {
        return number_of_invokes;
    }
private:
    Operation m_underlying{};
    static inline unsigned number_of_invokes = 0;
};

using Add = ArithmeticInstruction<std::plus<>>;
using Multiply = ArithmeticInstruction<std::multiplies<>>;
using Modulo = ArithmeticInstruction<std::modulus<>>;
using Subtract = ArithmeticInstruction<std::minus<>>;
using JumpIfGreaterThanZero = JumpInstruction<std::greater<>>;
using JumpIfNotZero = JumpInstruction<std::not_equal_to<>>;
using CountedMultiply = CountedInstruction<Multiply>;
