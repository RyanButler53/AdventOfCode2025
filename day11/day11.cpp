#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <array>
#include <string_view>
#include <map>
#include <ranges>
#include <algorithm>
#include <format>
#include <optional> // monads!
#include <functional>
#include "aoc.hpp"

std::vector<std::vector<unsigned int>> parseInput(std::filesystem::path file, std::unordered_map<std::string, unsigned int>& nameToNode, std::vector<std::string>& nodeToName){
    
    std::vector<std::vector<std::string>> adjTempList; // node 0 -> index 0 -> adjacency list. Directed. 
    std::vector<std::string> rows = aoc::readFileIntoStrings(file);
    for (unsigned int i = 0; i < rows.size(); ++i){
        adjTempList.push_back({});
        std::vector<std::string> split = aoc::splitString(rows[i], " ");
        split[0] = split[0].substr(0, 3);
        nodeToName.push_back(split[0]);
        nameToNode.insert({split[0], i}); // map this name to index i. 
        for (size_t edge_i = 1; edge_i < split.size(); ++edge_i){
            adjTempList.back().push_back(split[edge_i]);
        }
    }
    nameToNode.insert({"out", nameToNode.size()});
    std::cout << adjTempList.size() << std::endl;
    std::vector<std::vector<unsigned int>> adjList;
    for (unsigned i = 0; i < adjTempList.size(); ++i){
        adjList.push_back({});
        for (auto endpoint : adjTempList[i]){
            adjList.back().push_back(nameToNode[endpoint]);
        }
    }
    adjList.push_back({});
    return adjList;
}

uint64_t findPath(const std::vector<std::vector<unsigned>>& adjList, unsigned current, unsigned target, std::unordered_map<unsigned, uint64_t>& memo){
    if (current == target){
        return 1;
    } else if (memo.contains(current)){
        return memo.at(current);
    } else {
        uint64_t total = 0;
        for (unsigned endpoint : adjList[current]){
            uint64_t newPaths = findPath(adjList, endpoint, target, memo);
            total += newPaths;
        }
        memo.insert({current, total});
        return total;
    }
}


uint64_t part1(std::filesystem::path file){
    std::unordered_map<std::string, unsigned int> nameToNode;
    std::vector<std::string> nodeToName;
    std::vector<std::vector<unsigned>> graph = parseInput(file, nameToNode, nodeToName);
    std::unordered_map<unsigned, uint64_t> memo;
    return findPath(graph, nameToNode["you"], nameToNode["out"], memo);
}

uint64_t part2(std::filesystem::path file){
    std::unordered_map<std::string, unsigned int> nameToNode;
    std::vector<std::string> nodeToName;
    std::vector<std::vector<unsigned>> graph = parseInput(file, nameToNode, nodeToName);

    std::unordered_map<unsigned, uint64_t> memo;
    int64_t fft = findPath(graph, nameToNode["svr"], nameToNode["fft"], memo);
    memo.clear();
    std::cout << "fft: " << fft << std::endl;
    uint64_t dac = findPath(graph, nameToNode["svr"], nameToNode["dac"], memo);
    std::cout << "dac: " << dac << std::endl;
    // for (auto [v, paths] : memo){
    //     std::cout << nodeToName[v] << ": " << paths << std::endl;
    // }
    memo.clear();
    int64_t fft2dac = findPath(graph, nameToNode["fft"], nameToNode["dac"], memo);

    memo.clear();
    int64_t dac2fft = findPath(graph, nameToNode["dac"], nameToNode["fft"], memo);

    memo.clear();
    int64_t dac2end = findPath(graph, nameToNode["dac"], nameToNode["out"], memo);

    memo.clear();
    int64_t fft2end = findPath(graph, nameToNode["fft"], nameToNode["out"], memo);

    uint64_t fftfirst = fft * fft2dac * dac2end;
    uint64_t dacfirst = dac * dac2fft * fft2end;
    return fftfirst + dacfirst;
}

int main(int argc, char** argv){

    if (argc < 2){
        std::cout << "Needs filename argument" << std::endl;
        return 0;
    }
    std::cout << part1(argv[1]) << std::endl;
    std::cout << part2(argv[1]) << std::endl;
}