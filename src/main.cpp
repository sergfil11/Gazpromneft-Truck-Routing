#include "prep_functions.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

int main() {

    // vector<int> demand = {0, 5, 0, 3};
    // vector<int> spaces = {100, 100, 100, 100};

    // Station s(1, 10, 15, demand, spaces);

    // for (int i : s.demanded_res) {
    //     cout << i << " ";  // → 1 3
    // }

    // cout << bool_mask("245", 7) << " ";
    // cout << mask_to_digits(52, 7) << " ";

    // cout << "\n";

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


    // // Вывод
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

    map<pair<int, int>, int> gl_num = global_numeration({4,4});
    for (const auto& [key, value] : gl_num) {
        const auto& [station, res_idx] = key; // распаковка пары-ключа
        cout << "(" << station << "," << res_idx << ") -> " << value << '\n';
    }


    Truck truck = Truck(1, {1000, 1000, 1000, 1000}, 1);
    vector<Station> chosen_stations = {Station(2, 10, 15, {0, 5, 0, 3}, {100, 100, 100, 100}), Station(3, 10, 15, {1000, 5, 3000, 3}, {1001, 100, 3005, 100})};
    vector<vector<string>> fillings = get_fillings(truck, chosen_stations, gl_num);


    // for (size_t i = 0; i < chosen_stations.size(); ++i) {
    //     assert(chosen_stations[i].number == i && "Station numbers must be consecutive starting from 0");            // TODO: убрать assert и поправить нумерацию, чтобы она не падала
    // }

    for (auto &filling : fillings) {
        for (auto &elem : filling) {
            if (!elem.empty())
                cout << elem << " ";
            else
                cout << "x ";
        }
        cout << "\n";
    }




    // cerr << "fillings[0].size()=" << (fillings.empty() ? 0 : fillings[0].size())
    //  << " gl_num.size()=" << gl_num.size() << "\n";

    return 0;
};