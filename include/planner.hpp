#ifndef PLANNER_HPP
#define PLANNER_HPP

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

struct Flight {
    int flight_id;
    std::string departure_date;
    std::string intl_dome;
    std::string flight_no;
    int departure_airport;
    int arrival_airport;
    std::string departure_time;
    std::string arrival_time;
    int airplane_id;
    std::string airplane_model;
    int air_fares;
};

struct DFSResult {
    std::vector<int> airports;
};

struct BFSResult {
    std::vector<int> airports;
};

struct ConnectivityResult {
    bool has_route;
    std::vector<std::vector<int>> routes; // 每个路由包含航班ID
};

struct ShortestPathResult {
    bool has_route;
    std::vector<int> flight_ids;
    int total_time;
};

struct MinimumCostPathResult {
    bool has_route;
    std::vector<int> flight_ids;
    int total_cost;
};

struct AllPathsResult {
    bool has_route;
    std::vector<std::vector<int>> all_routes; // 每个路由包含航班ID
};

struct Planner {
    Planner();
    DFSResult query_dfs(int airport_id, std::string start_time);
    BFSResult query_bfs(int airport_id, std::string start_time);
    ConnectivityResult query_connectivity(int airport_1, int airport_2);
    ShortestPathResult query_shortest_path(
 int airport_1, int airport_2, const std::string& start_time, const std::string& end_time);
 MinimumCostPathResult query_minimum_cost_path(
 int airport_1, int airport_2, const std::string& start_time, const std::string& end_time);
 AllPathsResult query_all_paths(
 int airport_1, int airport_2, const std::string& start_time, const std::string& end_time);

private:
    std::vector<Flight> flights;
    std::map<int, std::vector<Flight>> adjacency_list;
    std::map<std::string, time_t> time_cache;

    time_t parse_time(const std::string& time_str);
    void build_adjacency_list();
    void dfs_helper(int current, std::string current_time, std::set<int>& visited, std::vector<int>& result);
    std::vector<Flight> get_sorted_flights(int airport_id, std::string current_time);
};

#endif // PLANNER_HPP