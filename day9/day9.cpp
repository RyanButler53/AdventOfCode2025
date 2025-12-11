#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <queue>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <format>
#include "aoc.hpp"
#include <map>
#include <optional> // monads!
#include <functional>


int64_t part1(std::filesystem::path file){
    std::ifstream in(file);
    std::string line;
    std::vector<std::pair<int64_t, int64_t>> points;
    while(std::getline(in,line)){
        auto pts = aoc::splitString(line, ",");
        points.push_back({stol(pts[0]), stol(pts[1])});
    }
    int64_t maxArea = -1;
    for (size_t p1 = 0; p1 < points.size(); ++p1){
        for (size_t p2 = p1 + 1; p2 < points.size(); ++p2){
            int64_t area = (std::abs(points[p1].first - points[p2].first)+1) * (std::abs(points[p1].second - points[p2].second)+1);
            maxArea = std::max(maxArea, area);
        }
    }
    return maxArea;
}

struct Point {
    int64_t x;
    int64_t y;
};

struct Bound {
    int64_t lower;
    int64_t upper;

};


// Returns true if it is valid
bool checkEdge(const std::map<int64_t, Bound>& v, const std::map<int64_t, Bound>& h, Point p1, Point p2){
    if (p1.x == p2.x && p1.y == p2.y){
        // Same Point -> always valid
        return true;
    }
    // If the edge between p1 and p2 is a vertical edge, need to check if there are any 
    auto sortFunc = [](const std::pair<int64_t, Bound>& pt1, const std::pair<int64_t, Bound>& pt2)->bool {return pt1.first < pt2.first;};
    if (p1.x == p2.x){ // vertical
        int64_t min = std::min(p1.y, p2.y) + 1; // +- 1 to account for the corner.  There will always be an acceptable corner as an intersection
        int64_t max = std::max(p1.y, p2.y) - 1;
        auto miniter = h.lower_bound(min);
        auto maxiter = h.upper_bound(max);
        for (auto boundIter = miniter; boundIter != maxiter; ++boundIter){
            int64_t edgeMinX = boundIter->second.lower;
            int64_t edgeMaxX = boundIter->second.upper;
            if (std::clamp(p1.x, edgeMinX, edgeMaxX) == p1.x){
                return false; // The edge between these 2 points intersects an edge
            }
        }
    } else {
        int64_t min = std::min(p1.x, p2.x) + 1; // +- 1 to account for the corner.  There will always be an acceptable corner as an intersection
        int64_t max = std::max(p1.x, p2.x) - 1;

        auto miniter = v.lower_bound(min);
        auto maxiter = v.upper_bound(max);
        for (auto boundIter = miniter; boundIter != maxiter; ++boundIter){
            int64_t edgeMinY = boundIter->second.lower;
            int64_t edgeMaxY = boundIter->second.upper;
            if (std::clamp(p1.y, edgeMinY, edgeMaxY) == p1.y){
                return false; // The edge between these 2 points intersects an edge
            }
        }
    }
    return true;
}

int64_t part2_2(std::filesystem::path file){
    std::ifstream in(file);
    std::string line;
    std::vector<Point> points;

    std::map<int64_t, Bound> verticalEdges; // {x: {ymin, ymax}}
    std::map<int64_t, Bound> horizontalEdges; // {y: {xmin, xmax}}

    while(std::getline(in,line)){
        auto pts = aoc::splitString(line, ",");
        points.push_back({stol(pts[0]), stol(pts[1])});
    }
    size_t n = points.size();
    for (size_t ip = 0; ip < n; ++ip){
        Point& p1 = points[ip];
        Point& p2 = points[(ip +1) % n];
        if (p1.x == p2.x) { // vertical edge
            verticalEdges.insert({p1.x, {std::min(p1.y, p2.y), std::max(p1.y, p2.y) }});
        } else {
            horizontalEdges.insert({p1.y, {std::min(p1.x, p2.x), std::max(p1.x, p2.x)}});
        }
    }

    int64_t maxArea = -1;
    int numChecked = 0;

    for (size_t p1 = 0; p1 < points.size(); ++p1){
        for (size_t p2 = p1 + 1; p2 < points.size(); ++p2){

            Point pt1 = points[p1];
            Point pt2 = points[p2];
            std::array<Point, 4> corners{pt1, {pt1.x,pt2.y}, pt2,{pt2.x,pt1.y}};
            bool validRectangle = true;
            for (size_t icorner = 0; icorner < 4; ++icorner){
                if (!checkEdge(verticalEdges, horizontalEdges, corners[icorner], corners[(icorner+1)%4])){
                    validRectangle = false;
                    break;
                }
            }
            if (validRectangle){
                int64_t area = (std::abs(pt1.x - pt2.x) + 1) * (std::abs(pt1.y - pt2.y) + 1);
                maxArea = std::max(maxArea, area);
            }
        }
    }
    return maxArea;
}

int main(int argc, char** argv){
    std::cout << part1(argv[1]) << std::endl;
    std::cout << part2_2(argv[1]) << std::endl;
}