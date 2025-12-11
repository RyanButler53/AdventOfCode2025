/**
 * @file lib.cpp
 * @author Ryan Butler
 * @brief My Aoc 2025 Utility Library. Using C++23 features for error handling and parsing. 
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "aoc.hpp"

#include <ranges>
#include <iostream>
#include <expected>
#include <vector>
#include <functional>
#include <concepts>

std::vector<std::string> aoc::splitString(std::string line, std::string delim){
    return std::views::split(line, delim) | std::ranges::to<std::vector<std::string>>();
}

std::vector<int64_t> aoc::splitAndCast(std::string line, std::string delim){
    std::vector<std::string> split = splitString(line, delim);
    return split | std::views::transform([](std::string s){return stol(s);})  | std::ranges::to<std::vector<int64_t>>();
}

std::vector<std::string> aoc::readFileIntoStrings(std::filesystem::path file){
    std::ifstream in(file);
    std::string line;
    std::vector<std::string> map;
    while(std::getline(in, line)){map.push_back(line);}
    return map;
}
