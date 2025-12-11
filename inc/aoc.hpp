#pragma once
#include <vector>
#include <string>
#include <ranges>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace aoc {

    /**
     * @brief Split a string by a delimiter
     * 
     * @param line Line to split
     * @param delim Delimter to split on
     * @return std::vector<std::string> Vector containing the split parts
     */
    std::vector<std::string> splitString(std::string line, std::string delim);

    /**
     * @brief Split a string and cast it to int64_t, common AOC data type
     * 
     * @param line 
     * @param delim 
     * @return std::vector<int64_t> Vcctor of int64_t's with result
     */
    std::vector<int64_t> splitAndCast(std::string line, std::string delim);

    /**
     * @brief Reads the contents of the file into a 2d map. Indexable with map[r][c]
     * 
     * @param file File to read from
     * @return std::vector<std::string> 
     */
    std::vector<std::string> readFileIntoStrings(std::filesystem::path file);

    // TEMPLATE DEFINITIONS

    /**
     * @brief Splits a string intt parts by delimiter and casts it. 
     * 
     * @tparam T Type to cast to
     * @tparam F Function to convert string -> T
     * @param line line to split
     * @param delim delimiter
     * @param convert conversion function
     * @return std::vector<T> Return vector. 
     */
    template <typename T, typename F>
    std::vector<T> splitAndCast(std::string line, std::string delim, F convert){
        std::vector<std::string> split = splitString(line, delim);
        return split | std::views::transform(convert) | std::ranges::to<std::vector<T>>();
    }
    
    /**
     * @brief Utility for printing a vector
     */
    template <typename T>
    void printVec(const std::vector<T>& v){
        for (auto x : v){
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }
    
}
