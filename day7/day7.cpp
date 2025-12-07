#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <array>
#include "matrix.hpp"
#include <ranges>
#include <algorithm>
#include <format>
#include <optional> // monads!
#include <functional>

using namespace std;
std::vector<std::string> getMap(std::filesystem::path path){
    vector<string> map;
    ifstream in(path);
    string line;
    while(getline(in, line)){
        map.push_back(line);
    }
    return map;
}

template <typename T>
void printVec(const std::vector<T>& v){
    for (auto x : v){
        std::cout << x << "\n";
    }
    std::cout << std::endl;
}



int64_t part1(std::filesystem::path file){
    std::vector<std::string> map = getMap(file);
    size_t start = map[0].find_first_of('S');
    map[0][start] = '|';
    int splits = 0; // result
    // printVec(map);
    for (size_t row_i = 0; row_i < map.size() - 1; ++row_i){
        // Row_i is current row, row_i + 1 is next row. row_i + 1 is an empty space row. 
        for (size_t char_i = 0; char_i < map[row_i].size(); ++char_i){
            // If the current row has a '.', do nothing. If it has a '|'
            if (map[row_i][char_i] == '|'){
                if (map[row_i + 1][char_i] == '^'){ // split
                    map[row_i + 1][char_i - 1] = '|';
                    map[row_i + 1][char_i + 1] = '|';
                    ++splits;
                } else {
                    map[row_i + 1][char_i] = '|';
                }
            }
        }
    }
    return splits;
}

int64_t part2(std::filesystem::path file){
    std::vector<std::string> map = getMap(file);
    size_t start = map[0].find_first_of('S');
    map[0][start] = '|';

    Matrix<int64_t> numPaths(map.size(), map[0].size());
    std::fill(numPaths.data(), numPaths.data() + map.size() * map[0].size(), 0);
    numPaths(0, start) = 1;
    for (size_t row_i = 0; row_i < map.size() - 1; ++row_i){
        // Row_i is current row, row_i + 1 is next row. row_i + 1 is an empty space row. 
        for (size_t char_i = 0; char_i < map[row_i].size(); ++char_i){
            // If the current row has a '.', do nothing. If it has a '|'
            if (map[row_i][char_i] == '|'){
                if (map[row_i + 1][char_i] == '^'){ // split
                    map[row_i + 1][char_i - 1] = '|';
                    map[row_i + 1][char_i + 1] = '|';
                    numPaths(row_i + 1, char_i - 1) += numPaths(row_i, char_i);
                    numPaths(row_i + 1, char_i + 1) += numPaths(row_i, char_i);
                } else {
                    map[row_i + 1][char_i] = '|';
                    numPaths(row_i + 1, char_i) += numPaths(row_i, char_i);
                }
            }
        }
    }
    int64_t total = 0;
    for (int i = 0; i < map[0].size(); ++i){
        total +=  numPaths(map.size() - 1, i);
    }
    return total;
}

int main(int argc, char** argv){
    std::cout << part1(argv[1]) << std::endl;
    std::cout << part2(argv[1]) << std::endl;
}