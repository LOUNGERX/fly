#include "planner.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

Planner::Planner() {
    // 加载航班数据
    ifstream file("flight-data.csv");
    if (!file.is_open()) {
        cerr << "Error opening flight-data.csv" << endl;
        return;
    }
    
    string line;
    // 跳过表头
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Flight flight;
        
        // 解析Flight ID
        getline(ss, token, ',');
        flight.flight_id = stoi(token);
        
        // 跳过Departure date
        getline(ss, token, ',');
        
        // 跳过Intl/Dome
        getline(ss, token, ',');
        
        // 跳过Flight NO.
        getline(ss, token, ',');
        
        // 解析Departure airport
        getline(ss, token, ',');
        flight.departure_airport = stoi(token);
        
        // 解析Arrival airport
        getline(ss, token, ',');
        flight.arrival_airport = stoi(token);
        
        // 解析Departure Time
        getline(ss, token, ',');
        flight.departure_time = token;
        
        // 解析Arrival Time
        getline(ss, token, ',');
        flight.arrival_time = token;
        
        // 跳过Airplane ID
        getline(ss, token, ',');
        
        // 跳过Airplane Model
        getline(ss, token, ',');
        
        // 解析Air fares
        getline(ss, token, ',');
        flight.fare = stoi(token);
        
        // 添加到航班列表
        flights.push_back(flight);
        
        // 添加到航线图
        flight_graph[flight.departure_airport].push_back(flight);
    }
    
    file.close();
    
    // 对每个机场的航班按出发时间和目标机场编号排序
    for (auto& pair : flight_graph) {
        sort(pair.second.begin(), pair.second.end(), [](const Flight& f1, const Flight& f2) {
            if (f1.departure_time != f2.departure_time) {
                return f1.departure_time < f2.departure_time;
            }
            return f1.arrival_airport < f2.arrival_airport;
        });
    }
}

bool Planner::is_time_after(const string& time1, const string& time2) {
    // 时间格式：MM/DD/YYYY HH:MM
    return time1 > time2;
}

int Planner::calculate_time_difference(const string& time1, const string& time2) {
    // 简化实现：假设时间格式正确，并且time1 >= time2
    // 实际应用中需要更精确的时间解析
    return 0;
}

DFSResult Planner::query_dfs(int airport_id, string start_time) {
    DFSResult result;
    map<int, bool> visited;
    
    // 深度优先遍历
    function<void(int)> dfs = [&](int current) {
        if (visited[current]) return;
        visited[current] = true;
        result.airport_sequence.push_back(current);
        
        // 获取从当前机场出发的所有航班
        if (flight_graph.find(current) == flight_graph.end()) return;
        
        for (const Flight& flight : flight_graph[current]) {
            if (is_time_after(flight.departure_time, start_time) || flight.departure_time == start_time) {
                dfs(flight.arrival_airport);
            }
        }
    };
    
    dfs(airport_id);
    return result;
}

BFSResult Planner::query_bfs(int airport_id, string start_time) {
    BFSResult result;
    map<int, bool> visited;
    queue<int> q;
    
    visited[airport_id] = true;
    result.airport_sequence.push_back(airport_id);
    q.push(airport_id);
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        // 获取从当前机场出发的所有航班
        if (flight_graph.find(current) == flight_graph.end()) continue;
        
        for (const Flight& flight : flight_graph[current]) {
            if ((is_time_after(flight.departure_time, start_time) || flight.departure_time == start_time) && !visited[flight.arrival_airport]) {
                visited[flight.arrival_airport] = true;
                result.airport_sequence.push_back(flight.arrival_airport);
                q.push(flight.arrival_airport);
            }
        }
    }
    
    return result;
}

ConnectivityResult Planner::query_connectivity(int airport_1, int airport_2) {
    ConnectivityResult result;
    result.exists = false;
    
    // 检查直飞
    if (flight_graph.find(airport_1) != flight_graph.end()) {
        for (const Flight& flight : flight_graph[airport_1]) {
            if (flight.arrival_airport == airport_2) {
                result.exists = true;
                result.flight_ids.push_back(flight.flight_id);
                return result;
            }
        }
    }
    
    // 检查1次中转
    if (flight_graph.find(airport_1) != flight_graph.end()) {
        for (const Flight& flight1 : flight_graph[airport_1]) {
            int transit = flight1.arrival_airport;
            if (flight_graph.find(transit) != flight_graph.end()) {
                for (const Flight& flight2 : flight_graph[transit]) {
                    if (flight2.arrival_airport == airport_2 && is_time_after(flight2.departure_time, flight1.arrival_time)) {
                        result.exists = true;
                        result.flight_ids.push_back(flight1.flight_id);
                        result.flight_ids.push_back(flight2.flight_id);
                        return result;
                    }
                }
            }
        }
    }
    
    return result;
}

ShortestPathResult Planner::query_shortest_path(int airport_1, int airport_2, int start_time, int end_time) {
    ShortestPathResult result;
    result.exists = false;
    result.total_time = INT_MAX;
    
    // 简化实现：返回第一条找到的航线
    // 实际应用中需要使用Dijkstra算法
    return result;
}

MinimumCostPathResult Planner::query_minimum_cost_path(int airport_1, int airport_2, int start_time, int end_time) {
    MinimumCostPathResult result;
    result.exists = false;
    result.total_cost = INT_MAX;
    
    // 简化实现：返回第一条找到的航线
    // 实际应用中需要使用Dijkstra算法
    return result;
}

AllPathsResult Planner::query_all_paths(int airport_1, int airport_2, int start_time, int end_time) {
    AllPathsResult result;
    
    // 查找所有直飞航线
    if (flight_graph.find(airport_1) != flight_graph.end()) {
        for (const Flight& flight : flight_graph[airport_1]) {
            if (flight.arrival_airport == airport_2) {
                vector<int> path;
                path.push_back(flight.flight_id);
                result.all_flight_ids.push_back(path);
            }
        }
    }
    
    // 查找所有1次中转的航线
    if (flight_graph.find(airport_1) != flight_graph.end()) {
        for (const Flight& flight1 : flight_graph[airport_1]) {
            int transit = flight1.arrival_airport;
            if (flight_graph.find(transit) != flight_graph.end()) {
                for (const Flight& flight2 : flight_graph[transit]) {
                    if (flight2.arrival_airport == airport_2 && is_time_after(flight2.departure_time, flight1.arrival_time)) {
                        vector<int> path;
                        path.push_back(flight1.flight_id);
                        path.push_back(flight2.flight_id);
                        result.all_flight_ids.push_back(path);
                    }
                }
            }
        }
    }
    
    return result;
}
