#include "planner.hpp"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    Planner planner;
    string query;

    cout << "欢迎使用航线规划系统！请输入查询命令：" << endl;
    cout << "命令格式：" << endl;
    cout << "1. DFS <机场ID> <出发时间>" << endl;
    cout << "2. BFS <机场ID> <出发时间>" << endl;
    cout << "3. CONNECT <机场A> <机场B>" << endl;
    cout << "4. SHORTEST <机场A> <机场B> <开始时间> <结束时间>" << endl;
    cout << "5. MINCOST <机场A> <机场B> <开始时间> <结束时间>" << endl;
    cout << "6. ALL <机场A> <机场B> <开始时间> <结束时间>" << endl;
    cout << "7. QUIT" << endl;
    cout << "时间格式示例：5/5/2017 12:20" << endl;

    while (true) {
        cout << "\n请输入命令：";
        getline(cin, query);
        if (query.empty()) continue;

        stringstream ss(query);
        string command;
        ss >> command;

        if (command == "DFS") {
            int airport_id;
            string start_time;
            ss >> airport_id >> start_time;
            // 处理时间格式，可能需要补全空格
            string rest;
            while (ss >> rest) {
                start_time += " " + rest;
            }
            DFSResult result = planner.query_dfs(airport_id, start_time);
            cout << "DFS遍历结果：";
            for (int i = 0; i < result.airports.size(); ++i) {
                if (i > 0) cout << " ";
                cout << result.airports[i];
            }
            cout << endl;
        } else if (command == "BFS") {
            int airport_id;
            string start_time;
            ss >> airport_id >> start_time;
            string rest;
            while (ss >> rest) {
                start_time += " " + rest;
            }
            BFSResult result = planner.query_bfs(airport_id, start_time);
            cout << "BFS遍历结果：";
            for (int i = 0; i < result.airports.size(); ++i) {
                if (i > 0) cout << " ";
                cout << result.airports[i];
            }
            cout << endl;
        } else if (command == "CONNECT") {
            int airport1, airport2;
            ss >> airport1 >> airport2;
            ConnectivityResult result = planner.query_connectivity(airport1, airport2);
            if (result.has_route) {
                cout << "找到航线：" << endl;
                for (int i = 0; i < result.routes.size(); ++i) {
                    cout << "航线" << i+1 << ": ";
                    for (int j = 0; j < result.routes[i].size(); ++j) {
                        if (j > 0) cout << " -> ";
                        cout << "航班" << result.routes[i][j];
                    }
                    cout << endl;
                }
            } else {
                cout << "没有找到航线" << endl;
            }
        } else if (command == "SHORTEST") {
            int airport1, airport2;
            string start_time, end_time;
            ss >> airport1 >> airport2 >> start_time >> end_time;
            // 处理时间格式，可能需要补全空格
            string rest;
            while (ss >> rest) {
                if (end_time.find(":") == string::npos) {
                    end_time += " " + rest;
                } else {
                    // 时间已经完整，剩下的可能是错误输入，忽略
                    break;
                }
            }
            ShortestPathResult result = planner.query_shortest_path(airport1, airport2, start_time, end_time);
            if (result.has_route) {
                cout << "最短时间航线：";
                for (int i = 0; i < result.flight_ids.size(); ++i) {
                    if (i > 0) cout << " -> ";
                    cout << "航班" << result.flight_ids[i];
                }
                cout << "，总时间：" << result.total_time << "秒" << endl;
            } else {
                cout << "没有找到航线" << endl;
            }
        } else if (command == "MINCOST") {
            int airport1, airport2;
            string start_time, end_time;
            ss >> airport1 >> airport2 >> start_time >> end_time;
            // 处理时间格式，可能需要补全空格
            string rest;
            while (ss >> rest) {
                if (end_time.find(":") == string::npos) {
                    end_time += " " + rest;
                } else {
                    // 时间已经完整，剩下的可能是错误输入，忽略
                    break;
                }
            }
            MinimumCostPathResult result = planner.query_minimum_cost_path(airport1, airport2, start_time, end_time);
            if (result.has_route) {
                cout << "最低价格航线：";
                for (int i = 0; i < result.flight_ids.size(); ++i) {
                    if (i > 0) cout << " -> ";
                    cout << "航班" << result.flight_ids[i];
                }
                cout << "，总价格：" << result.total_cost << "元" << endl;
            } else {
                cout << "没有找到航线" << endl;
            }
        } else if (command == "ALL") {
            int airport1, airport2;
            string start_time, end_time;
            ss >> airport1 >> airport2 >> start_time >> end_time;
            // 处理时间格式，可能需要补全空格
            string rest;
            while (ss >> rest) {
                if (end_time.find(":") == string::npos) {
                    end_time += " " + rest;
                } else {
                    // 时间已经完整，剩下的可能是错误输入，忽略
                    break;
                }
            }
            AllPathsResult result = planner.query_all_paths(airport1, airport2, start_time, end_time);
            if (result.has_route) {
                cout << "所有航线：" << endl;
                for (int i = 0; i < result.all_routes.size(); ++i) {
                    cout << "航线" << i+1 << ": ";
                    for (int j = 0; j < result.all_routes[i].size(); ++j) {
                        if (j > 0) cout << " -> ";
                        cout << "航班" << result.all_routes[i][j];
                    }
                    cout << endl;
                }
            } else {
                cout << "没有找到航线" << endl;
            }
        } else if (command == "QUIT") {
            cout << "感谢使用航线规划系统！" << endl;
            break;
        } else {
            cout << "无效命令，请重新输入！" << endl;
        }
    }

    return 0;
}