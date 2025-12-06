#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <print>
// Just happened to have an interval tree lying around
#include "intervalTree.hpp"

void parse(std::filesystem::path& file, std::vector<std::pair<int64_t, int64_t>>& bounds, std::vector<long>& items){
    std::ifstream in(file);
    std::string line;
    while (std::getline(in, line))
    {
        if (line == ""){
            break;
        }
        size_t dash = line.find('-');

        int64_t  first = stol(std::string(line.substr(0, dash)));
        int64_t second = stol(std::string(line.substr(dash+1)));
        bounds.push_back(std::pair<int64_t, int64_t>{first, second}); 
        line.clear();
    }
    while (std::getline(in, line)){
        items.push_back(stol(line));
    }

    
}

// need an interval tree
int part1(std::filesystem::path file){

    std::vector<std::pair<int64_t, int64_t>> bounds; // {low, high}
    std::vector<long> items;

    parse(file, bounds, items);

    IntervalTree<int64_t> tree;
    for (auto [low, high] : bounds){
        tree.insert(low, high);
    }

    int totalGood = 0;
    for (long item : items){
        std::vector<std::pair<int64_t, int64_t>> overlaps = tree.findOverlaps(item);
        if (!overlaps.empty()){
            ++totalGood;
        }
    }
    return totalGood;
}

using IntervalID = std::pair<int64_t, int64_t>;
int64_t part2(std::filesystem::path file){
    std::vector<std::pair<int64_t, int64_t>> bounds; // {low, high}
    std::vector<long> items;

    parse(file, bounds, items);

    IntervalTree<int64_t> tree;
    for (auto [low, high] : bounds){
        tree.insert(low, high);
    }
    std::ranges::sort(bounds); // sort by the lowest value;
    int64_t total = 0;
    int64_t current = bounds.begin()->first;
    int64_t maxValue = bounds.back().second;
    while (current < maxValue){
        std::vector<IntervalID> overlaps = tree.findOverlaps(current);
        auto compareMaxes = [](IntervalID i1, IntervalID i2){
            return i1.second > i2.second;
        };
        // minimum max value of an interval that contains this value 
        if (overlaps.empty()){
            current = std::ranges::upper_bound(bounds, IntervalID{current, current})->first;
            ++total; // Count the tiny match
            continue;
        }
        int64_t minMax = std::min_element(overlaps.begin(), overlaps.end(), compareMaxes)->second;
        if (minMax == current){ // there is no other corresponding one. Need to increment to the NEXT interval
            current = std::ranges::upper_bound(bounds, IntervalID{current, current})->first;
            ++total; // The current match never gets counted
            continue;
        }
        total += minMax - current;
        current = minMax;
    }
    ++total;
    // the last value never gets counted
    return total;
}

int main(int argc, char** argv){
    std::cout << part1(argv[1]) << std::endl;
    std::cout << part2(argv[1]) << std::endl;
}