#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <array>
#include <string_view>
#include <ranges>
#include <print>
#include <algorithm>
#include <concepts>
#include <map>
#include <utility>
#include <functional>
#include <optional> // monads!
#include <numeric>
#include <functional>

std::vector<int> getStarts(std::string line, std::vector<char>& ops){
    std::vector<int> starts;
    bool prevSpace = true;
    for (unsigned int i = 0; i < line.size(); ++i){
        if (line[i] != ' ' && prevSpace) { // found an op. 
            starts.push_back(i);
            ops.push_back(line[i]);
        }
        prevSpace = (line[i] == ' ');
    }
    return starts;
}

void parse(std::filesystem::path file, std::vector<std::vector<int64_t>>& columns, std::vector<char>& ops){
    std::ifstream in(file);
    std::string line;

    while(std::getline(in, line)){}
    std::vector<int> starts= getStarts(line, ops);

    line.clear();
    in.close();
    // using the starts, parse into numbers.  One vector per column of numbers
    
    columns.resize(starts.size());
    in.open(file);
    while(std::getline(in, line)){
        // Break on operators 
        if (line[0] == '+' or line[0] == '*'){
            break;
        }
        for (size_t i = 0; i < starts.size() - 1; ++i){
            columns[i].push_back(stol(line.substr(starts[i], starts[i+1] - starts[i])));
        }
        // special case for last one
        columns.back().push_back(stol(line.substr(starts.back())));
    }
}

void parse2(std::filesystem::path file, std::vector<std::vector<int64_t>>& columns, std::vector<char>& ops){
    std::ifstream in(file);
    std::string line;
    std::vector<std::string> grid;
    while(std::getline(in, line)){
        grid.push_back(line);
    }

    std::vector<int> starts = getStarts(line, ops);

    columns.resize(starts.size());
    for (size_t start = 1; start < starts.size(); ++start){
        int gridCol = starts[start] - 2;
        // Iterate right towards the previous operator
        while (gridCol >= starts[start - 1])
        {
            // build up digits
            std::string digits;
            for (size_t row = 0; row < grid.size() - 1; ++row) { // grid size - 1 to account for operators in last row. 
                digits.push_back(grid[row][gridCol]);
            }

            columns[start-1].push_back(stol(digits));
            --gridCol;
        }   
    } 
    // last start case:
    int gridCol = grid[0].size() - 1;
    // Iterate right towards the previous operator
    while (gridCol >= starts.back())
    {
        // build up digits
        std::string digits;
        for (size_t row = 0; row < grid.size() - 1; ++row) { // grid size - 1 to account for operators in last row. 
            digits.push_back(grid[row][gridCol]);
        }
        columns.back().push_back(stol(digits));
        --gridCol;
    }
    
}

int64_t fold(std::vector<std::vector<int64_t>>& columns, std::vector<char>& ops){
    const std::map<char, std::function<int64_t(int64_t, int64_t)>> operators{
        {'+', std::plus<int64_t>()},
        {'*', std::multiplies<int64_t>()}
    };

    int64_t sum = 0;
    for (auto [nums, op] : std::views::zip(columns, ops)){
        // C++23 Solution: int64_t result = std::ranges::fold_left_first(columns, ops[op]);
        sum += std::accumulate(nums.begin(), nums.end(), int64_t(op == '*'), operators.at(op));
    }
    return sum;
}

template <typename F>
int64_t solve(std::filesystem::path file, F parseFunction){
    std::vector<std::vector<int64_t>> columns;
    std::vector<char> ops;
    parseFunction(file, columns, ops);
    return fold(columns, ops);
}

int main(int argc, char** argv){
    std::cout << solve(argv[1], parse) << std::endl;
    std::cout << solve(argv[1], parse2) << std::endl;
}