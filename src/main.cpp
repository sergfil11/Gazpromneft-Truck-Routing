#include "prep_functions.hpp"
#include <iostream>

using namespace std;

int main() {
    vector<int> demand = {0, 5, 0, 3};
    vector<int> spaces = {100, 100, 100, 100};

    Station s(1, 10, 15, demand, spaces);

    for (int i : s.demanded_res) {
        cout << i << " ";  // → 1 3
    }

    cout << bool_mask("245", 7) << " ";
    cout << mask_to_digits(52, 7) << " ";

    cout << "\n";

    map<int, vector<string>> possible_combinations = {
    {0, {string("0"), string("356"), string("107"), string("256"), string("123456"), string("56789")}},
    {1, {}},
    {2, {string("23"), string("6"), string("780")}},
    {3, {string("5"), string("72"), string("8"), string("0123")}},
    {4, {string("1"), string("34"), string("2"), string("9"), string("4")}},
    {5, {string("01234")}},
    };

    int comp_num = 10;

    auto [best_masks, best_selections] = dp_max_unique_digits_all_masks(possible_combinations, comp_num);
    vector<string> digits_combination;
    for (int mask : best_masks) {
        digits_combination.push_back(mask_to_digits(mask, comp_num));
    }


    // Вывод
    cout << "Digits combinations:\n";
    for (const auto &s : digits_combination) cout << s << " ";
    cout << endl;

    cout << "Best selections:\n";
    for (auto &s : best_selections) {
        for (auto &selection : s) {
            for (auto &sel : selection) {
                cout << sel << " ";
            }
            cout << "\n";            
        }
    }

    // cout << endl;
    // vector<vector<int>> a;
    // a = combinations(5, 3);
    // for (auto &combination : a) {
    //     for (auto &elem : combination) {
    //         cout << elem << " ";
    //     }
    //     cout << "\n";            
    // }


    vector<vector<string>> testic = possible_filling({1,2,3,4}, {4,4,4,12}, {5,7,9,16});
    for (auto &combination : testic) {
        cout << "[";  
        for (auto &elem : combination) {
            if (elem != "")
                cout << elem + ',' << " ";
            else
                cout << "None, ";

        }
        cout << "]\n";            
    }
    return 0;
};