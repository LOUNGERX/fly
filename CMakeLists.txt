cmake_minimum_required(VERSION 3.10)
project(FlightPlanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(flight_planner src/main.cpp src/planner.cpp)

# 复制flight-data.csv到构建目录
configure_file(flight-data.csv flight-data.csv COPYONLY)