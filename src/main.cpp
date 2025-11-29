#include "planner.hpp"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    Planner planner;
    
    while (true) {
        string query;
        getline(cin, query);
        
        if (query.empty()) continue;
        
        stringstream ss(query);
        string command;
        ss >> command;
        
        if (command == "dfs") {
            int airport_id;
            string start_time;
            ss >> airport_id >> start_time;
            
            // 处理带空格的时间字符串
            string time_part;
            while (ss >> time_part) {
                start_time += " " + time_part;
            }
            
            DFSResult result = planner.query_dfs(airport_id, start_time);
            
            cout << "DFS Result: ";
            for (int i = 0; i < result.airport_sequence.size(); ++i) {
                if (i > 0) cout << " ";
                cout << result.airport_sequence[i];
            }
            cout << endl;
        } else if (command == "bfs") {
            int airport_id;
            string start_time;
            ss >> airport_id >> start_time;
            
            // 处理带空格的时间字符串
            string time_part;
            while (ss >> time_part) {
                start_time += " " + time_part;
            }
            
            BFSResult result = planner.query_bfs(airport_id, start_time);
            
            cout << "BFS Result: ";
            for (int i = 0; i < result.airport_sequence.size(); ++i) {
                if (i > 0) cout << " ";
                cout << result.airport_sequence[i];
            }
            cout << endl;
        } else if (command == "connectivity") {
            int airport_1, airport_2;
            ss >> airport_1 >> airport_2;
            
            ConnectivityResult result = planner.query_connectivity(airport_1, airport_2);
            
            if (result.exists) {
                cout << "Connectivity Result: Exists, Flight IDs: ";
                for (int i = 0; i < result.flight_ids.size(); ++i) {
                    if (i > 0) cout << " ";
                    cout << result.flight_ids[i];
                }
                cout << endl;
            } else {
                cout << "Connectivity Result: No route found" << endl;
            }
        } else if (command == "shortest_path") {
            int airport_1, airport_2, start_time, end_time;
            ss >> airport_1 >> airport_2 >> start_time >> end_time;
            
            ShortestPathResult result = planner.query_shortest_path(airport_1, airport_2, start_time, end_time);
            
            if (result.exists) {
                cout << "Shortest Path Result: Exists, Flight IDs: ";
                for (int i = 0; i < result.flight_ids.size(); ++i) {
                    if (i > 0) cout << " ";
                    cout << result.flight_ids[i];
                }
                cout << ", Total Time: " << result.total_time << endl;
            } else {
                cout << "Shortest Path Result: No route found" << endl;
            }
        } else if (command == "minimum_cost_path") {
            int airport_1, airport_2, start_time, end_time;
            ss >> airport_1 >> airport_2 >> start_time >> end_time;
            
            MinimumCostPathResult result = planner.query_minimum_cost_path(airport_1, airport_2, start_time, end_time);
            
            if (result.exists) {
                cout << "Minimum Cost Path Result: Exists, Flight IDs: ";
                for (int i = 0; i < result.flight_ids.size(); ++i) {
                    if (i > 0) cout << " ";
                    cout << result.flight_ids[i];
                }
                cout << ", Total Cost: " << result.total_cost << endl;
            } else {
                cout << "Minimum Cost Path Result: No route found" << endl;
            }
        } else if (command == "all_paths") {
            int airport_1, airport_2, start_time, end_time;
            ss >> airport_1 >> airport_2 >> start_time >> end_time;
            
            AllPathsResult result = planner.query_all_paths(airport_1, airport_2, start_time, end_time);
            
            cout << "All Paths Result: " << result.all_flight_ids.size() << " paths found" << endl;
            for (int i = 0; i < result.all_flight_ids.size(); ++i) {
                cout << "Path " << i+1 << ": ";
                for (int j = 0; j < result.all_flight_ids[i].size(); ++j) {
                    if (j > 0) cout << " ";
                    cout << result.all_flight_ids[i][j];
                }
                cout << endl;
            }
        } else if (command == "quit") {
            break;
        } else {
            cout << "Unknown command. Available commands: dfs, bfs, connectivity, shortest_path, minimum_cost_path, all_paths, quit" << endl;
        }
    }
    
    return 0;
}
