#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <array>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <functional>
#include <queue>
#include <set>
#include "kdtree.hpp"
#include "aoc.hpp"

using Point = std::array<int64_t, 3>;

// DFS to get the network of points corresponding with this point p
std::set<Point> getNetwork(const std::map<Point, std::set<Point>>& adjlist, const std::vector<Point>& allPoints, const Point& p){
    std::map<Point, bool> visited;
    for (auto p : allPoints){
        visited.insert({p, false});
    }
    std::queue<Point> q;
    q.push(p);
    std::set<Point> network;
    while(!q.empty()){
        const Point& cur = q.front();
        q.pop();
        visited[cur] = true;
        network.insert(cur);
        for (auto next : adjlist.at(cur)){
            if (visited[next] == false){
                q.push(next);
                visited[next] = true;
            }
        }
    }
    return network;
}



int64_t part1(std::filesystem::path file, size_t nconn){
    std::vector<std::string> lines = aoc::readFileIntoStrings(file);
    std::vector<Point> points;
    KDTree<int64_t, 3> kdtree;

    for (auto line : lines){
        std::vector<int64_t> pts = aoc::splitAndCast(line, ",");
        points.push_back({pts[0], pts[1], pts[2]});
        kdtree.insert(points.back());
    }
    // Need to use a min heap to keep track if the nconn shortest things 
    using heap_t = std::tuple<double, Point, Point>;
    std::priority_queue<heap_t> closestDistances; // max heap
    for (const Point& p : points){
        // Find the n nearest 
        std::vector<Point> closestN = kdtree.nearestNeighbor(p, nconn - 1, DistanceFunction::Euclidean);

        for (const auto& p2 : closestN){
            double dist = KDTree<int64_t, 3>::dist(DistanceFunction::Euclidean, p, p2);

            if (closestDistances.size() < 2 * nconn){
                // need to populate the heap. Doesn't need to be good yet. 
                closestDistances.push({dist, p, p2});
            } else  if (dist < std::get<0>(closestDistances.top())){
                closestDistances.pop();
                closestDistances.push({dist, p, p2});
            }
        }
    }
    // Now we have the nconn * 2 closest connections. // Need to remove the duplicates. 
    std::vector<std::tuple<double, Point, Point>> connections;
    while(!closestDistances.empty()){
        connections.push_back(closestDistances.top());
        closestDistances.pop();
    }

    std::ranges::sort(connections);

    // Merge Networks Together
    std::map<Point, std::set<Point>> adjlist;
    for (const auto& p : points){
        adjlist.insert({p, {}});
    }
    for (auto [_, p1, p2]: connections){
        adjlist[p1].insert(p2);
    }
    

    // DFS to find the connected components
    std::vector<std::set<Point>> networks;
    for (auto& p : points){
        std::set<Point> network = getNetwork(adjlist, points, p);
        networks.push_back(network);
    }

    std::ranges::sort(networks, [](auto n1, auto n2){return n1.size() < n2.size();});
    auto lastNetwork = std::unique(networks.begin(), networks.end());
    networks.erase(lastNetwork, networks.end());
    std::ranges::reverse(networks);

    return networks[0].size() * networks[1].size() * networks[2].size();
}

int64_t part2(std::filesystem::path file){

    std::vector<std::string> lines = aoc::readFileIntoStrings(file);
    std::vector<Point> points;
    KDTree<int64_t, 3> kdtree;

    for (auto line : lines){
        std::vector<int64_t> pts = aoc::splitAndCast(line, ",");
        points.push_back({pts[0], pts[1], pts[2]});
        kdtree.insert(points.back());
    }

    // Need to use a min heap to keep track if the nconn shortest things 
    using heap_t = std::tuple<double, Point, Point>;
    std::priority_queue<heap_t, std::vector<heap_t>, std::greater<heap_t>> closestDistances; // min heap
    for (const Point& p : points){
        // Find the n nearest 
        std::vector<Point> closestN = kdtree.nearestNeighbor(p, points.size() - 1, DistanceFunction::Euclidean);

        // Throw all points into a min heap
        for (const auto& p2 : closestN){
            double dist = KDTree<int64_t, 3>::dist(DistanceFunction::Euclidean, p, p2);
            closestDistances.push({dist, p, p2});
        }
    }

    // Begin adding stuff to the network. 
    std::map<Point, std::set<Point>> adjlist;
    for (const auto& p : points){
        adjlist.insert({p, {}});
    }

    // WLOG, point 0 is the starting point. Ensure that the entire network is connected. 
    Point start = points[0];
    size_t numEdges = 0;
    while (!closestDistances.empty()){
        auto [_, p1, p2] = closestDistances.top();
        closestDistances.pop();
        adjlist[p1].insert(p2);
        adjlist[p2].insert(p1);
        std::set<Point> network = getNetwork(adjlist, points, start);
        ++numEdges;
        if (network.size() == points.size()){
            std::cout << "Num Edges: " << numEdges << std::endl;
            return p1[0] * p2[0];
        }
    }

        

    return -1;
}

int main(int argc, char** argv){
    size_t nconn = 10;
    std::string file =argv[1];
    if (file == "../day8/inputs/part1.txt"){
        nconn = 1000;
    }
    // Part 2 takes some time
    std::cout << part1(argv[1], nconn) << std::endl; // 171503
    std::cout << part2(argv[1]) << std::endl; // 9069509600
}