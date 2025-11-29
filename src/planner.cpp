#include "planner.hpp"

Planner::Planner() {
    // 读取航班数据
    std::ifstream file("flight-data.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening flight-data.csv" << std::endl;
        return;
    }

    std::string line;
    // 跳过表头
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Flight flight;

        // 读取Flight ID
        std::getline(ss, token, ',');
        flight.flight_id = std::stoi(token);

        // 读取Departure date
        std::getline(ss, flight.departure_date, ',');

        // 读取Intl/Dome
        std::getline(ss, flight.intl_dome, ',');

        // 读取Flight NO.
        std::getline(ss, flight.flight_no, ',');

        // 读取Departure airport
        std::getline(ss, token, ',');
        flight.departure_airport = std::stoi(token);

        // 读取Arrival airport
        std::getline(ss, token, ',');
        flight.arrival_airport = std::stoi(token);

        // 读取Departure Time
        std::getline(ss, flight.departure_time, ',');

        // 读取Arrival Time
        std::getline(ss, flight.arrival_time, ',');

        // 读取Airplane ID
        std::getline(ss, token, ',');
        flight.airplane_id = std::stoi(token);

        // 读取Airplane Model
        std::getline(ss, flight.airplane_model, ',');

        // 读取Air fares
        std::getline(ss, token, ',');
        flight.air_fares = std::stoi(token);

        flights.push_back(flight);
    }

    file.close();
    build_adjacency_list();
}

time_t Planner::parse_time(const std::string& time_str) {
    // 检查缓存
    if (time_cache.find(time_str) != time_cache.end()) {
        return time_cache[time_str];
    }

    // 解析时间字符串，格式为 "5/5/2017 12:20"
    struct tm tm = {0};
    int month, day, year, hour, minute;
    sscanf(time_str.c_str(), "%d/%d/%d %d:%d", &month, &day, &year, &hour, &minute);
    tm.tm_mon = month - 1; // 月份从0开始
    tm.tm_mday = day;
    tm.tm_year = year - 1900; // 年份从1900开始
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = 0;

    time_t t = mktime(&tm);
    time_cache[time_str] = t;
    return t;
}

void Planner::build_adjacency_list() {
    for (const Flight& flight : flights) {
        adjacency_list[flight.departure_airport].push_back(flight);
    }

    // 对每个机场的航班按出发时间和目标机场编号排序
    for (auto& entry : adjacency_list) {
        std::sort(entry.second.begin(), entry.second.end(), [this](const Flight& a, const Flight& b) {
            time_t time_a = parse_time(a.departure_time);
            time_t time_b = parse_time(b.departure_time);
            if (time_a == time_b) {
                return a.arrival_airport < b.arrival_airport;
            }
            return time_a < time_b;
        });
    }
}

std::vector<Flight> Planner::get_sorted_flights(int airport_id, std::string current_time) {
    std::vector<Flight> result;
    if (adjacency_list.find(airport_id) == adjacency_list.end()) {
        return result;
    }

    time_t current_t = parse_time(current_time);
    for (const Flight& flight : adjacency_list[airport_id]) {
        if (parse_time(flight.departure_time) >= current_t) {
            result.push_back(flight);
        }
    }

    return result;
}

DFSResult Planner::query_dfs(int airport_id, std::string start_time) {
    DFSResult result;
    std::set<int> visited;
    dfs_helper(airport_id, start_time, visited, result.airports);
    return result;
}

void Planner::dfs_helper(int current, std::string current_time, std::set<int>& visited, std::vector<int>& result) {
    if (visited.find(current) != visited.end()) {
        return;
    }

    visited.insert(current);
    result.push_back(current);

    std::vector<Flight> sorted_flights = get_sorted_flights(current, current_time);
    for (const Flight& flight : sorted_flights) {
        dfs_helper(flight.arrival_airport, flight.arrival_time, visited, result);
    }
}

BFSResult Planner::query_bfs(int airport_id, std::string start_time) {
    BFSResult result;
    std::set<int> visited;
    std::queue<std::pair<int, std::string>> q;

    q.push({airport_id, start_time});
    visited.insert(airport_id);
    result.airports.push_back(airport_id);

    while (!q.empty()) {
        auto [current, current_time] = q.front();
        q.pop();

        std::vector<Flight> sorted_flights = get_sorted_flights(current, current_time);
        for (const Flight& flight : sorted_flights) {
            if (visited.find(flight.arrival_airport) == visited.end()) {
                visited.insert(flight.arrival_airport);
                result.airports.push_back(flight.arrival_airport);
                q.push({flight.arrival_airport, flight.arrival_time});
            }
        }
    }

    return result;
}

ConnectivityResult Planner::query_connectivity(int airport_1, int airport_2) {
    ConnectivityResult result;
    result.has_route = false;

    // 检查直飞
    if (adjacency_list.find(airport_1) != adjacency_list.end()) {
        for (const Flight& flight : adjacency_list[airport_1]) {
            if (flight.arrival_airport == airport_2) {
                result.has_route = true;
                result.routes.push_back({flight.flight_id});
            }
        }
    }

    // 检查一次中转
    if (adjacency_list.find(airport_1) != adjacency_list.end()) {
        for (const Flight& flight1 : adjacency_list[airport_1]) {
            int transfer = flight1.arrival_airport;
            if (adjacency_list.find(transfer) != adjacency_list.end()) {
                for (const Flight& flight2 : adjacency_list[transfer]) {
                    if (flight2.arrival_airport == airport_2 && parse_time(flight2.departure_time) >= parse_time(flight1.arrival_time)) {
                        result.has_route = true;
                        result.routes.push_back({flight1.flight_id, flight2.flight_id});
                    }
                }
            }
        }
    }

    return result;
}

ShortestPathResult Planner::query_shortest_path(int airport_1, int airport_2, const std::string& start_time_str, const std::string& end_time_str) {
    ShortestPathResult result;
    result.has_route = false;
    result.total_time = INT_MAX;

    // 解析时间
    time_t start_time = parse_time(start_time_str);
    time_t end_time = parse_time(end_time_str);

    // 如果起点和终点相同
    if (airport_1 == airport_2) {
        result.has_route = true;
        result.total_time = 0;
        result.flight_ids = {};
        return result;
    }

    // 使用Dijkstra算法寻找最短时间路径
    std::map<int, int> dist; // 到机场的最小时间
    std::map<int, std::vector<int>> path; // 到机场的路径
    std::map<int, std::string> arrival_time; // 到达机场的时间

    // 初始化所有机场的距离为无穷大
    for (const auto& entry : adjacency_list) {
        dist[entry.first] = INT_MAX;
    }

    // 起点的距离为0
    dist[airport_1] = 0;
    arrival_time[airport_1] = start_time_str;

    // 优先队列：(当前距离, 当前机场)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, airport_1});

    while (!pq.empty()) {
        auto [current_dist, current] = pq.top();
        pq.pop();

        // 如果已经到达终点，提前返回
        if (current == airport_2) {
            result.has_route = true;
            result.flight_ids = path[current];
            result.total_time = current_dist;
            break;
        }

        // 如果当前距离大于已知最短距离，跳过
        if (current_dist > dist[current]) {
            continue;
        }

        // 获取当前机场出发的所有航班（按时间排序）
        std::vector<Flight> sorted_flights = get_sorted_flights(current, arrival_time[current]);
        for (const Flight& flight : sorted_flights) {
            time_t dep_t = parse_time(flight.departure_time);
            time_t arr_t = parse_time(flight.arrival_time);
            int flight_time = arr_t - dep_t;

            // 如果到达时间超过截止时间，跳过
            if (arr_t > end_time) {
                continue;
            }

            int new_dist = current_dist + flight_time;
            int next_airport = flight.arrival_airport;

            // 如果找到更短的路径
            if (new_dist < dist[next_airport]) {
                dist[next_airport] = new_dist;
                path[next_airport] = path[current];
                path[next_airport].push_back(flight.flight_id);
                arrival_time[next_airport] = flight.arrival_time;
                pq.push({new_dist, next_airport});
            }
        }
    }

    return result;
}

MinimumCostPathResult Planner::query_minimum_cost_path(int airport_1, int airport_2, const std::string& start_time_str, const std::string& end_time_str) {
    MinimumCostPathResult result;
    result.has_route = false;
    result.total_cost = INT_MAX;

    // 解析时间
    time_t start_time = parse_time(start_time_str);
    time_t end_time = parse_time(end_time_str);

    // 如果起点和终点相同
    if (airport_1 == airport_2) {
        result.has_route = true;
        result.total_cost = 0;
        result.flight_ids = {};
        return result;
    }

    // 使用Dijkstra算法寻找最小价格路径
    std::map<int, int> cost; // 到机场的最小价格
    std::map<int, std::vector<int>> path; // 到机场的路径
    std::map<int, std::string> arrival_time; // 到达机场的时间

    // 初始化所有机场的价格为无穷大
    for (const auto& entry : adjacency_list) {
        cost[entry.first] = INT_MAX;
    }

    // 起点的价格为0
    cost[airport_1] = 0;
    arrival_time[airport_1] = start_time_str;

    // 优先队列：(当前价格, 当前机场)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, airport_1});

    while (!pq.empty()) {
        auto [current_cost, current] = pq.top();
        pq.pop();

        // 如果已经到达终点，提前返回
        if (current == airport_2) {
            result.has_route = true;
            result.flight_ids = path[current];
            result.total_cost = current_cost;
            break;
        }

        // 如果当前价格大于已知最低价格，跳过
        if (current_cost > cost[current]) {
            continue;
        }

        // 获取当前机场出发的所有航班（按时间排序）
        std::vector<Flight> sorted_flights = get_sorted_flights(current, arrival_time[current]);
        for (const Flight& flight : sorted_flights) {
            time_t arr_t = parse_time(flight.arrival_time);
            // 如果到达时间超过截止时间，跳过
            if (arr_t > end_time) {
                continue;
            }

            int new_cost = current_cost + flight.air_fares;
            int next_airport = flight.arrival_airport;

            // 如果找到更便宜的路径
            if (new_cost < cost[next_airport]) {
                cost[next_airport] = new_cost;
                path[next_airport] = path[current];
                path[next_airport].push_back(flight.flight_id);
                arrival_time[next_airport] = flight.arrival_time;
                pq.push({new_cost, next_airport});
            }
        }
    }

    return result;
}

AllPathsResult Planner::query_all_paths(int airport_1, int airport_2, const std::string& start_time_str, const std::string& end_time_str) {
    AllPathsResult result;
    result.has_route = false;

    // 解析时间
    time_t start_time = parse_time(start_time_str);
    time_t end_time = parse_time(end_time_str);

    // 直飞
    if (adjacency_list.find(airport_1) != adjacency_list.end()) {
        for (const Flight& flight : adjacency_list[airport_1]) {
            time_t dep_t = parse_time(flight.departure_time);
            time_t arr_t = parse_time(flight.arrival_time);
            if (flight.arrival_airport == airport_2 && dep_t >= start_time && arr_t <= end_time) {
                result.has_route = true;
                result.all_routes.push_back({flight.flight_id});
            }
        }
    }

    // 一次中转
    if (adjacency_list.find(airport_1) != adjacency_list.end()) {
        for (const Flight& flight1 : adjacency_list[airport_1]) {
            time_t dep1_t = parse_time(flight1.departure_time);
            time_t arr1_t = parse_time(flight1.arrival_time);
            if (dep1_t < start_time || arr1_t > end_time) {
                continue;
            }

            int transfer = flight1.arrival_airport;
            if (adjacency_list.find(transfer) != adjacency_list.end()) {
                for (const Flight& flight2 : adjacency_list[transfer]) {
                    time_t dep2_t = parse_time(flight2.departure_time);
                    time_t arr2_t = parse_time(flight2.arrival_time);
                    if (flight2.arrival_airport == airport_2 && dep2_t >= arr1_t && arr2_t <= end_time) {
                        result.has_route = true;
                        result.all_routes.push_back({flight1.flight_id, flight2.flight_id});
                    }
                }
            }
        }
    }

    return result;
}