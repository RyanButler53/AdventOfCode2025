cmake_minimum_required(VERSION 4.0)
project(AdventOfCode2025)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

macro(addDay day)
    add_executable("day${day}" "day${day}/day${day}.cpp")
endmacro()

addDay(1)
addDay(2)
addDay(3)
addDay(4)
addDay(5)
