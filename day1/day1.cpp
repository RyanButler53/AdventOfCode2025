#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>

// because C++ handles modulo weird
int mod100(int x){
    if (x > 0){
        return x % 100;
    } else {
        return ((x % 100) + 100)%100;
    }
}

std::vector<std::tuple<char, int>> getInput(std::string filename){
    std::ifstream in(filename);
    std::string line;
    std::vector<std::tuple<char, int>> turns;
    while (std::getline(in, line))
    {
        int turn = stoi(line.substr(1));
        turns.push_back({line[0], turn});
    }
    return turns;
}

int part1(){
    int zeroes = 0;
    int value = 50;

    auto input = getInput("../day1/inputs/part1.txt");
    for (auto [dir, turn] : input) {
        if (dir == 'R'){
            value += turn;
        } else {
            value -= turn;
        }

        if (value % 100 == 0){
            zeroes += 1;
        }

    }
    
    return zeroes;
}

int part2(){
    int zeroes = 0;
    int position = 50;

    auto input = getInput("../day1/inputs/part1.txt");
    for (auto [dir, turn] : input){
        if (dir == 'R'){
            for (size_t i = 0; i < turn; ++i){
                position = mod100(position + 1);
                zeroes += (position % 100 == 0);
            }
        } else {
            for (size_t i = 0; i < turn; ++i){
                position = mod100(position - 1);
                zeroes += (position % 100 == 0);
            }
        }
    }
    return zeroes;
}
int main(){

    std::cout << part1() << std::endl;
    std::cout << part2() << std::endl;
}