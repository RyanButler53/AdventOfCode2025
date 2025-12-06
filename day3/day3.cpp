#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <string_view>
#include <ranges>
#include <print>
#include <map>
#include <algorithm>
#include <iomanip>
#include <optional> // monads!
#include <functional>

std::vector<std::vector<int>> parse(std::filesystem::path file){
    std::ifstream in(file);
    std::string line;
    std::vector<std::vector<int>> result;
    while(std::getline(in, line)){
        // result.push_back(line);
        std::vector<int> digits;
        for(char c : line){
            digits.push_back(atoi(&c));
        }
        result.push_back(digits);
    }
    return result;
}

int getJoltage(const std::vector<int>& batteries){
    std::vector<int>::const_iterator first = std::max_element(batteries.begin(), batteries.end()- 1);
    std::vector<int>::const_iterator second = std::max_element(first + 1, batteries.end());
 
    return *first * 10 + *second; 
}


uint64_t part1(std::filesystem::path p){
    auto input = parse(p);
    uint64_t sum = 0;
    for (const auto& battery : input){
        sum += getJoltage(battery);
    }
    return sum;
}

// gets the indexes of all 
std::vector<size_t> findall(std::vector<int>& batteries, int n){
    std::vector<size_t> ret;
    for (size_t i = 0; i < batteries.size(); ++i){
        if (batteries[i] == n){
            ret.push_back(i);
        }
    } // put the later (less valuable ones) at the end
    return ret;
}
// Memoize:
int64_t joltageHelper(const std::vector<int>& batteries, size_t i, size_t n, std::map<std::pair<size_t, size_t>, int64_t>& memo){
    // Use memo if possible
    if (memo.contains({i, n})){
        return memo.at({i, n});
    } else if (n == 0){
        // Base Case: No more numbers left -> no more joltage to add
        memo.insert({{i, 0}, 0});
        return 0;
    } else if (i + n == batteries.size()){
        // Must use all the remaining batteries 
        int64_t result = (batteries[i] * pow(10, n-1)) + joltageHelper(batteries, i+1, n-1, memo);
        memo.insert({{i, n}, result});
        return result;
    } else if (batteries[i] < batteries[i+1]){
        // Branch and bound: Its always better to use the next battery 
        int64_t result = joltageHelper(batteries, i+1, n,memo);
        memo.insert({{i, n}, result});
        return result;
    } else {
        int64_t useit = (batteries[i] * pow(10, n-1)) + joltageHelper(batteries, i+1, n-1,memo);
        int64_t loseit = joltageHelper(batteries, i+1, n,memo);
        int64_t result = std::max(useit, loseit);
        memo.insert({{i, n}, result});
        return result;
    }
}

int64_t getJoltage2(const std::vector<int>& batteries){
    std::map<std::pair<size_t, size_t>, int64_t> memo;
    return joltageHelper(batteries, 0, 12, memo);    
}


uint64_t part2(std::filesystem::path p){
    auto input = parse(p);
    uint64_t sum = 0;
    for (const auto& battery : input){
        sum += getJoltage2(battery);
    }
    return sum;
}


int main(int argc, char** argv){

    std::cout << part1(argv[1]) << std::endl; 
    std::cout << part2(argv[1]) << std::endl; // getJoltage2(battery);

}