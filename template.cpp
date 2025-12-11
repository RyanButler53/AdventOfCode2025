#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <array>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <format>
#include <optional> // monads!
#include <functional>


int64_t part1(std::filesystem::path file){
    return 0;
}

int64_t part2(std::filesystem::path file){
    return 0;
}

int main(int argc, char** argv){

    if (argc < 2){
        std::cout << "Needs filename argument" << std::endl;
        return 0;
    }
    std::cout << part1(argv[1]) << std::endl;
    std::cout << part2(argv[1]) << std::endl;
}