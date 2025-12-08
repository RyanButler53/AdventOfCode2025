#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <print>
#include <ranges>
#include <print>
#include <map>
#include <algorithm>
#include <iomanip>
#include <mdspan>
#include "matrix.hpp"
#include <functional>

std::vector<std::vector<int>> parse(std::filesystem::path file){
    std::ifstream in(file);
    std::string line;
    std::vector<std::vector<int>> result;
    while(std::getline(in, line)){
        std::vector<int> digits;
        for(char c : line){
            digits.push_back(atoi(&c));
        }
        result.push_back(digits);
    }
    return result;
}

int64_t joltage(const std::vector<int>& batteries, size_t k){
    size_t n = (batteries.size() + 1);
    Matrix<int64_t> table(k+1, n);
    // Base Case:  No more numbers to pick. Always 0
    for (size_t basecase = 0; basecase < n; ++basecase){
        table(0, basecase) = 0;
    }

    for (size_t icol = 0; icol < batteries.size(); ++icol){
        size_t col = batteries.size() - icol - 1;
        for (size_t ik = 1; ik < k+1; ++ik){
            // Recursive Logic: 
            if (ik + col == batteries.size()){
                // Must use all batteries. 
                table(ik, col) = (batteries[col] * pow(10, ik-1)) + table(ik - 1, col + 1);
                break;
            } else if (batteries[col] < batteries[col+1]){
                // Skip the battry and use the next one
                table(ik, col) = table(ik, col + 1);
            } else {
                int64_t useit = (batteries[col] * pow(10, ik-1)) + table(ik - 1, col + 1);
                int64_t loseit = table(ik, col + 1);
                table(ik, col) = std::max(useit, loseit);
            }
        }
    }

    return table(k,0);
}

int64_t solve(std::filesystem::path file, size_t n){
    auto input = parse(file);
    uint64_t sum = 0;
    for (const auto& battery : input){
        sum += joltage(battery, n);
    }
    return sum;
}

int main(int argc, char** argv){

    std::cout << solve(argv[1], 2) << std::endl; 
    std::cout << solve(argv[1], 12) << std::endl;

}