#include "prep_functions.hpp"
#include <iostream>
#include <assert.h>
#include <locale>


using namespace std;

int main() {
    setlocale(LC_ALL, ".utf8");
    
    // MASKS TEST
    // vector<int> demand = {0, 5, 0, 3};
    // vector<int> spaces = {100, 100, 100, 100};
    // Station s(1, 10, 15, demand, spaces);
    // for (int i : s.demanded_res) {
    //     cout << i << " ";  // → 1 3
    // }
    // cout << bool_mask("245", 7) << " ";
    // cout << mask_to_digits(52, 7) << " ";
    // cout << "\n";

    // POSSIBLE COMBINATIONS TEST 
    // map<int, vector<string>> possible_combinations = {
    // {0, {string("0"), string("356"), string("107"), string("256"), string("123456"), string("56789")}},
    // {1, {}},
    // {2, {string("23"), string("6"), string("780")}},
    // {3, {string("5"), string("72"), string("8"), string("0123")}},
    // {4, {string("1"), string("34"), string("2"), string("9"), string("4")}},
    // {5, {string("01234")}},
    // };

    // int comp_num = 10;
    // auto [best_masks, best_selections] = dp_max_unique_digits_all_masks(possible_combinations, comp_num);
    // vector<string> digits_combination;
    // for (int mask : best_masks) {
    //     digits_combination.push_back(mask_to_digits(mask, comp_num));
    // }
    // cout << "Digits combinations:\n";
    // for (const auto &s : digits_combination) cout << s << " ";
    // cout << endl;

    // cout << "Best selections:\n";
    // for (auto &s : best_selections) {
    //     for (auto &selection : s) {
    //         for (auto &sel : selection) {
    //             cout << sel << " ";
    //         }
    //         cout << "\n";            
    //     }
    // }

    // vector<vector<string>> testic = possible_filling({1,2,3,4}, {4,4,4,12}, {5,7,9,16});
    // for (auto &combination : testic) {
    //     cout << "[";  
    //     for (auto &elem : combination) {
    //         if (elem != "")
    //             cout << elem + ',' << " ";
    //         else
    //             cout << "None, ";

    //     }
    //     cout << "]\n";            
    // }

    // GET FILLINGS TEST
    // map<pair<int, int>, int> gl_num = global_numeration({4,4});
    // for (const auto& [key, value] : gl_num) {
    //     const auto& [station, res_idx] = key; // распаковка пары-ключа
    //     cout << "(" << station << "," << res_idx << ") -> " << value << '\n';
    // }
    // Truck truck = Truck(1, {1000, 10, 1000, 1000}, 1);
    // vector<Station> chosen_stations = {Station(2, 10, 15, {0, 5, 0, 3}, {100, 100, 100, 100}), Station(3, 10, 15, {1000, 5, 3000, 3}, {1001, 100, 3005, 100})};
    // vector<vector<string>> fillings = get_fillings(truck, chosen_stations, gl_num);
    // for (auto &filling : fillings) {
    //     for (auto &elem : filling) {
    //         if (!elem.empty())
    //             cout << elem << " ";
    //         else
    //             cout << "x ";
    //     }
    //     cout << "\n";
    // }

    // ALL_FILLINGS_TEST
    // vector<Station> stations = {
    //     Station(0, 0, 2, {5, 0}, {10, 10}),
    //     Station(1, 1, 1, {3, 2}, {10, 10})
    // };
    // Truck truck(1, {5, 5}, 1);  // два отсека по 5 единиц
    // vector<vector<int>> time_to_station = {
    //     {0, 1},
    //     {1, 0}
    // };
    // map<pair<int,int>,int> gl_num = {
    //     {{0,0}, 0}, {{0,1}, 1},
    //     {{1,0}, 2}, {{1,1}, 3}
    // };
    // int H = 5;
    // int st_in_trip = 2;
    // int top_nearest = 2;
    // set<vector<string>> result = all_fillings(stations, truck, time_to_station, gl_num, H, st_in_trip, top_nearest);
    // int cnt = 0;
    // for (const auto& filling : result) {
    //     cout << "Filling " << cnt++ << ": ";
    //     for (const auto& val : filling) cout << (val.empty() ? "x" : val) << " ";
    //     cout << endl;
    // }

    // TWO_PIPES_OPT_TEST
    // vector<int> fill_times = {3, 1, 4, 2, 7, 9};
    // int best = two_pipes_opt(fill_times);
    // cout << "BEST TIME = " << best << endl;


    // COMPUTE_TIME_FOR_ROUTE_TEST
    map<int, pair<int,int>> reverse_index = {
        {0, {1,0}}, {1, {1,0}}, {2, {2,0}}, {3, {3,0}}
    };
    vector<int> compartments = {1000, 2000, 1500, 1200};
    vector<string> fill = {"0", "1", "0", "3"};  // строки как в твоей функции
    bool double_piped = true;
    vector<Station> input_station_list = {
        Station(0, 5, 5, {0, 1}, {10, 10}),
        Station(1, 3, 3, {1, 0}, {10, 10}),
        Station(2, 4, 4, {0, 1}, {10, 10}),
        Station(2, 4, 4, {0, 1}, {10, 10})
    };
    vector<vector<int>> demanded_matrix = {
        {0,5,10,15},
        {5,0,8,12},
        {10,8,0,6},
        {15,12,6,0}
    };
    vector<int> docs_fill = {3,4,5,2};
    pair<int, vector<string>> result = compute_time_for_route(
        reverse_index,
        compartments,
        fill,
        double_piped,
        input_station_list,
        demanded_matrix,
        docs_fill
    );
    cout << "Минимальное время: " << result.first << " минут\n";
    cout << "Лог времени:\n";
    for (const string& s : result.second) {
        cout << "  " << s << "\n";
    }

    return 0;
};