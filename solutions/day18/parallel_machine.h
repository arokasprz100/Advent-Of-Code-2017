#pragma once

class SharedMachineState {
public:

    bool queue_empty(int subscriber_number) {
        if (subscriber_number == 0) {
            return second_program_queue.empty();
        }
        return first_program_queue.empty();
    }

    void push_to_queue(int publisher_number, int64_t value) {
        if (publisher_number == 0) {
            first_program_queue.push(value);
        } else {
            second_program_queue.push(value);
        }
    }

    int64_t pop_from_queue(int subscriber_number) {
        if (subscriber_number == 0) {
            const auto value = second_program_queue.front();
            second_program_queue.pop();
            return value;
        } else {
            const auto value = first_program_queue.front();
            first_program_queue.pop();
            return value;
        }
    }

    void set_waiting_status(int machine_number, bool value) {
        if (machine_number == 0) {
            first_program_waiting = value;
        } else {
            second_program_waiting = value;
        }
    }

    [[nodiscard]]
    bool deadlock_detected() const {
        return first_program_waiting && second_program_waiting;
    }

private:
    bool first_program_waiting{};
    bool second_program_waiting{};
    std::queue<int64_t> first_program_queue{};
    std::queue<int64_t> second_program_queue{};
};

struct ParallelMachineState: MachineState {
    int64_t number_of_sends{};
    int machine_number{};
    std::shared_ptr<SharedMachineState> shared_state{};
};

class Send {
public:
    explicit Send(Operand operand): m_operand{std::move(operand)} {}

    void execute(ParallelMachineState& state) {
        state.shared_state->push_to_queue(state.machine_number, resolve(m_operand, state));
        ++state.number_of_sends;
        ++state.program_counter;
    }

private:
    Operand m_operand;
};

class Receive {
public:
    explicit Receive(Operand operand): m_operand{std::move(operand)} {}

    void execute(ParallelMachineState& state) {
        state.shared_state->set_waiting_status(state.machine_number, true);
        if (!state.shared_state->queue_empty(state.machine_number)) {
            resolve(m_operand, state) = state.shared_state->pop_from_queue(state.machine_number);
            ++state.program_counter;
            state.shared_state->set_waiting_status(state.machine_number, false);
        }
    }

private:
    Operand m_operand;
};

using ParallelMachineInstruction = std::variant<Set, Add, Multiply, Modulo, JumpIfGreaterThanZero, Send, Receive>;