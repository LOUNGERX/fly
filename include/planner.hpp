#ifndef PLANNER_HPP
#define PLANNER_HPP

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <climits>
#include <functional>

struct Flight {
    int flight_id;
    int departure_airport;
    int arrival_airport;
    std::string departure_time;
    std::string arrival_time;
    int fare;
};

struct DFSResult {
    std::vector<int> airport_sequence;
};

struct BFSResult {
    std::vector<int> airport_sequence;
};

struct ConnectivityResult {
    bool exists;
    std::vector<int> flight_ids;
};

struct ShortestPathResult {
    bool exists;
    std::vector<int> flight_ids;
    int total_time;
};

struct MinimumCostPathResult {
    bool exists;
    std::vector<int> flight_ids;
    int total_cost;
};

struct AllPathsResult {
    std::vector<std::vector<int>> all_flight_ids;
};

struct Planner {
    // 构造函数，加载航班数据
    Planner();
    
    // 深度优先遍历
    DFSResult query_dfs(int airport_id, std::string start_time);
    
    // 广度优先遍历
    BFSResult query_bfs(int airport_id, std::string start_time);
    
    // 直飞或1次中转的航线
    ConnectivityResult query_connectivity(int airport_1, int airport_2);
    
    // 最小时间开销航线
    ShortestPathResult query_shortest_path(
        int airport_1, int airport_2, int start_time, int end_time);
    
    // 最小价格开销航线
    MinimumCostPathResult query_minimum_cost_path(
        int airport_1, int airport_2, int start_time, int end_time);
    
    // 直飞或1次中转的所有航线
    AllPathsResult query_all_paths(
        int airport_1, int airport_2, int start_time, int end_time);
    
private:
    // 航班数据
    std::vector<Flight> flights;
    
    // 航线图：key是出发机场ID，value是从该机场出发的所有航班
    std::map<int, std::vector<Flight>> flight_graph;
    
    // 辅助函数：比较时间字符串的大小
    bool is_time_after(const std::string& time1, const std::string& time2);
    
    // 辅助函数：计算两个时间之间的分钟数差
    int calculate_time_difference(const std::string& time1, const std::string& time2);
};

#endif // PLANNER_HPP
