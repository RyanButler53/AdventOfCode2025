cmake_minimum_required(VERSION 4.0)
project(AdventOfCode2025)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(aoc lib/lib.cpp)
target_include_directories(aoc PUBLIC inc)

macro(addDay day)
    add_executable("day${day}" "day${day}/day${day}.cpp")
endmacro()

macro(addDayLib day)
    add_executable("day${day}" "day${day}/day${day}.cpp")
    target_link_libraries("day${day}" PUBLIC aoc)
endmacro(addDayLib)


addDay(1)
addDay(2)
addDay(3)
addDay(4)
addDay(5)
addDay(6)
addDayLib(7)
addDayLib(8)
addDayLib(9)
addDay(10)
addDayLib(11)
