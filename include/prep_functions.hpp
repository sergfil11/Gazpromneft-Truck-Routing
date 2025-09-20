#pragma once   // защита от двойного включения

#include <vector>
#include <numeric>
#include <optional>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <set>

using namespace std;

class Station {
public:
    int number;
    int time_to_depot;
    int time_from_depot;
    vector<int> demand;
    vector<int> remaining_spaces;
    vector<int> demanded_res;
    vector<int> filled_values;

    // конструктор
    Station(int number, int time_to_depot, int time_from_depot,
            const vector<int>& demand, const vector<int>& remaining_spaces);

};

class Truck {
public:
    int number;
    vector<int> compartments;
    int remaining_time;
    int total_trips;
    int pour_time;
    optional<int> FuelTankerID;

    Truck(int number, const vector<int>& compartments, optional<int> ftid);

};

// Utility


// перевод числа в битовую маску
int bool_mask(string n, int comp_num);

// обратный перевод маски в строку цифр
string mask_to_digits(int mask, int comp_num);

// аналог itertools.combinations
vector<vector<int>> combinations(int n, int k);

// преобразует вектор векторов строк в множество битовых масок
set<vector<int>> boolify_reservoirs(const vector<vector<string>>& fillings);

// переводит номер станции и резервуара в глобальный номер резервуара
map<pair<int, int>, int> global_numeration(vector<int> lengths);



// Main

// динамический алгоритм поиска всех вариантов выгрузки отсеков бензовоза на множестве резервуаров
pair<vector<int>, vector<vector<vector<string>>>> dp_max_unique_digits_all_masks(const map<int, vector<string>>& possible_combinations, int comp_num);

// Находит все допустимые комбинации отсеков между min[i] и max[i]
vector<vector<string>> possible_filling(const vector<int>& compartments, const vector<int>& mins, const vector<int>& maxs);