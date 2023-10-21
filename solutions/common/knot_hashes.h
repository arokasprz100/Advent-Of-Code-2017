#pragma once

#include <algorithm>
#include <array>
#include <numeric>
#include <string>
#include <vector>

constexpr auto Sparse_Hash_Size{256};
constexpr auto Dense_Hash_Block_Size{16};
constexpr auto Number_Of_Sparse_Hash_Iterations{64};
constexpr auto Number_Of_Blocks_In_Dense_Hash = Sparse_Hash_Size / Dense_Hash_Block_Size;

struct KnotHashState {
    unsigned skip_size{0};
    unsigned current_position{0};
    std::array<unsigned, Sparse_Hash_Size> numbers{};
};

KnotHashState create_knot_hash_state() {
    KnotHashState state{};
    std::iota(std::begin(state.numbers), std::end(state.numbers), 0); // TODO: ranges::iota C++23
    return state;
}

void knot_hash_single_round(const std::vector<unsigned>& lengths, KnotHashState& state) {
    for(const auto& length: lengths) {
        std::vector<unsigned> to_reverse(length);
        for(unsigned i = 0; i < length; ++i) {
            const auto element_index = (state.current_position + i) % Sparse_Hash_Size;
            to_reverse[i] = state.numbers[element_index];
        }
        std::reverse(std::begin(to_reverse), std::end(to_reverse));
        for(unsigned i = 0; i < length; ++i) {
            const auto element_index = (state.current_position + i) % Sparse_Hash_Size;
            state.numbers[element_index] = to_reverse[i];
        }
        state.current_position = (state.current_position + length + state.skip_size) % Sparse_Hash_Size;
        ++state.skip_size;
    }
}

std::vector<unsigned> compute_dense_hash(const std::array<unsigned, Sparse_Hash_Size>& sparse_hash) {
    static const auto xor_numbers = [](unsigned lhs, unsigned rhs){ return lhs ^ rhs; };
    std::vector<unsigned> dense_hash(Number_Of_Blocks_In_Dense_Hash);
    for(unsigned i = 0; i < Number_Of_Blocks_In_Dense_Hash; ++i) {
        unsigned starting_index = i * Dense_Hash_Block_Size;
        unsigned end_index = starting_index + Dense_Hash_Block_Size;
        dense_hash[i] = std::accumulate(&sparse_hash[starting_index + 1], &sparse_hash[end_index], sparse_hash[starting_index],xor_numbers);
    }
    return dense_hash;
}

std::array<unsigned, Sparse_Hash_Size> compute_sparse_hash(const std::vector<unsigned>& lengths) {
    auto state = create_knot_hash_state();
    for(unsigned round = 0; round < Number_Of_Sparse_Hash_Iterations; ++round) {
        knot_hash_single_round(lengths, state);
    }
    return state.numbers;
}

std::vector<unsigned> prepare_input_lengths_as_ascii(const std::string& puzzle_input) {
    constexpr std::array<unsigned, 5> numbers_to_append = {17, 31, 73, 47, 23};
    std::vector<unsigned> ascii_codes(std::begin(puzzle_input), std::end(puzzle_input));
    std::copy(std::begin(numbers_to_append), std::end(numbers_to_append), std::back_inserter(ascii_codes));
    return ascii_codes;
}
