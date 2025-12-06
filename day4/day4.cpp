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
#include <format>
#include <optional> // monads!
#include <functional>

std::vector<std::string> parse(std::filesystem::path file){
    std::ifstream in(file);
    std::string line;
    std::vector<std::string> map;
    while(std::getline(in, line)){
        map.push_back(line);
    }
    return map;
}

std::vector<std::pair<int, int>> neighbors(int row, int col, int nrows, int ncols){
    // Assume that we can spill over the edges. Rows are 0-nrows-1 and cols are 0 -> ncols-1
    std::vector<std::pair<int, int>> edges {{row-1, col-1}, {row-1, col}, {row-1, col+1}, 
                                            {row, col - 1},               {row, col + 1},
                                            {row+1, col-1}, {row+1, col}, {row+1, col+1}};
    // Prune edges. 
    std::vector<std::pair<int, int>> realEdges;
    for (auto [r,c] : edges){
        if (r >= nrows || r < 0 || c >= ncols || col < 0){
            continue;
        } else{
            realEdges.push_back({r,c});
        }
    }
    return realEdges;
}

int part1(std::filesystem::path file){
    std::vector<std::string> map =parse(file);
    int total = 0;
    for (int r = 0; r < map.size(); ++r){
        for (int c = 0; c < map[0].size(); ++c){
            if (map[r][c] != '@') {continue;}
            std::vector<std::pair<int, int>> adj = neighbors(r, c, map.size(), map[0].size());
            size_t rolls = 0;
            for (auto [nr, nc] : adj){
                if (map[nr][nc] == '@'){
                    ++rolls;
                }
            }

            if (rolls < 4){
                ++total;
            }
        }
    }

    return total;
}

std::vector<std::pair<int, int>> getRollsToRemove(const std::vector<std::string>& map){
    std::vector<std::pair<int, int>> toRemove;
    for (int r = 0; r < map.size(); ++r){
        for (int c = 0; c < map[0].size(); ++c){
            if (map[r][c] != '@') {continue;}
            std::vector<std::pair<int, int>> adj = neighbors(r, c, map.size(), map[0].size());
            size_t rolls = 0;
            for (auto [nr, nc] : adj){
                if (map[nr][nc] == '@'){
                    ++rolls;
                }
            }

            if (rolls < 4){
                toRemove.push_back({r, c});
            }
        }
    }
    return toRemove;
}

void removeRolls(std::vector<std::string>& map, std::vector<std::pair<int, int>> toRemove){
    for (auto [r,c] : toRemove){
        map[r][c] = '.';
    }
}

int part2(std::filesystem::path file){
    std::vector<std::string> map = parse(file);
    int total = 0;
    while(true){
        std::vector<std::pair<int, int>> toRem = getRollsToRemove(map);
        total += toRem.size();
        if (toRem.empty()){
            return total;
        }
        removeRolls(map, toRem);
    }
}

int main(int argc, char** argv){
    std::cout << part1(argv[1]) << std::endl;
    std::cout << part2(argv[1]) << std::endl;
}