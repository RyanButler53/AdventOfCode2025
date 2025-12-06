#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <string_view>
#include <ranges>
#include <print>
#include <algorithm>
#include <iomanip>
#include <optional> // monads!
#include <functional>

using std::operator""sv;

std::vector<std::pair<long, long>> parseInput(std::string filename){
    std::ifstream in(filename);
    std::string line;
    getline(in, line);
    std::string_view v(line);
    constexpr auto delim{","sv};
    std::vector<std::pair<long, long>> input;

    for (const auto s : std::views::split(v, delim)){
        std::string_view sview(s);
        size_t dash = sview.find('-');
        try
        {
            long  first = stol(std::string(sview.substr(0, dash)));
            long second = stol(std::string(sview.substr(dash+1)));
            input.push_back({first, second}); 

        }
        catch(const std::exception& e)
        {
            std::cout << std::string(sview.substr(0, dash)) << " " << std::string(sview.substr(dash+1)) << std::endl;
            std::cerr << e.what() << '\n';
        }
        

    }  
    return input;
}

// Pattern repeats n times
bool checkInvalid(uint64_t x){
    std::string str = std::to_string(x);
    size_t size = str.size();
    if (size % 2 != 0){
        return false;
    }
    
    std::string firsthalf = str.substr(0, size/2);
    std::string secondhalf = str.substr(size/2);
    return firsthalf == secondhalf;
}
/**
 * @brief Splits S in k equal sized pieces. Returns a null optional if n cannot be split in k ways evenly. 
 * 
 * @param s string of length n
 * @param k 
 * @return std::optional<std::vector<std::string>> 
 */
std::optional<std::vector<std::string>> splitStringKWays(const std::string& s, int k){
    if (s.size() % k != 0){
        return std::nullopt;
    }
    std::vector<std::string> splits;
    size_t n = s.size();
    // split size is n / k
    int splitSize = n/k;
    for (size_t i = 0; i < k; ++i){
        splits.push_back(s.substr(i*splitSize, splitSize));
    }
    return splits;
}

bool checkInvalid2(uint64_t x){
    std::string str = std::to_string(x);
    size_t n = str.size();
    // K is the number of splits
    for (int k = 2; k <= n; ++k){
        auto splits = splitStringKWays(str, k);
        if (splits){
            auto it = std::search_n(splits->begin(), splits->end(), splits->size(), splits->at(0));
            if (it == splits->begin()){
                return true;
            }
        }
       
    }
    return false;
}

uint64_t solve(std::filesystem::path filename,std::function<bool(uint64_t)> func){
    auto input = parseInput(filename);
    uint64_t sum = 0;
    for (auto [start, end] : input){
        for (uint64_t i = start; i < end+1; ++i){
            if (func(i)){
                sum += i;
            }
        }
    }
    return sum;
}

int main(){
    std::cout << solve("../day2/inputs/part1.txt", checkInvalid) << std::endl;
    std::cout << solve("../day2/inputs/part1.txt", checkInvalid2) << std::endl;
}