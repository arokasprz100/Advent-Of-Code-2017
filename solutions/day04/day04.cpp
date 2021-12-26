#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <Utils.h>

using Passphrase = std::vector<std::string>;
using SecurityPolicy = std::function<bool(const Passphrase&)>;

std::vector<Passphrase> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if(!file.is_open()) {
        throw std::runtime_error{"Could not open file " + file_name};
    }
    std::vector<Passphrase> puzzle_input{};
    for(std::string input_line{}; std::getline(file, input_line); ) {
        const auto words = utils::split_by_delimiter(utils::trim(input_line), " ");
        puzzle_input.emplace_back(words);
    }
    return puzzle_input;
}

bool contains_no_duplicate_words(const Passphrase& passphrase) {
    std::set<std::string> without_duplicates(std::begin(passphrase), std::end(passphrase));
    return passphrase.size() == without_duplicates.size();
}

bool contains_no_anagrams(const Passphrase& passphrase) {
    Passphrase passphrase_with_rearranged_words{};
    for(std::string word: passphrase) {
        std::sort(std::begin(word), std::end(word));
        passphrase_with_rearranged_words.push_back(word);
    }
    return contains_no_duplicate_words(passphrase_with_rearranged_words);
}

unsigned count_number_of_valid_passphrases(const std::vector<Passphrase>& passphrases, const SecurityPolicy& security_policy) {
    return std::count_if(std::begin(passphrases), std::end(passphrases), security_policy);
}


int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << "Part 1: " << count_number_of_valid_passphrases(puzzle_input, contains_no_duplicate_words) << std::endl;
    std::cout << "Part 2: " << count_number_of_valid_passphrases(puzzle_input, contains_no_anagrams) << std::endl;
    return 0;
}