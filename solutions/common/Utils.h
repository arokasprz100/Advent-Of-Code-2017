#pragma once

#include <string>
#include <vector>

namespace utils {

// TODO: unit test, refactor
std::vector<std::string> split_by_delimiter(std::string to_split, const std::string& delimiter) {
    std::vector<std::string> tokens{};
    size_t delimiter_position{0};
    while((delimiter_position = to_split.find(delimiter)) != std::string::npos) {
        std::string token = to_split.substr(0, delimiter_position);
        to_split.erase(0, delimiter_position + delimiter.length());
        tokens.push_back(token);
    }
    if(!to_split.empty()) {
        tokens.push_back(to_split);
    }
    return tokens;
}

bool is_not_whitespace(const char character) {
    return !std::isspace<char>(character, std::locale::classic());
}

std::string left_trim(std::string to_trim) {
    auto it = std::find_if(std::begin(to_trim), std::end(to_trim), is_not_whitespace);
    to_trim.erase(std::begin(to_trim), it);
    return to_trim;
}

std::string right_trim(std::string to_trim) {
    auto it = std::find_if(std::rbegin(to_trim), std::rend(to_trim), is_not_whitespace);
    to_trim.erase(it.base(), std::end(to_trim));
    return to_trim;
}

std::string trim(const std::string& to_trim) {
    return left_trim(right_trim(to_trim));
}

}