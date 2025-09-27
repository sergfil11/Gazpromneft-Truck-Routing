#pragma once   // защита от двойного включения

#include <vector>
#include <optional>
#include <string>
#include <map>
#include <set>
#include <unordered_set>

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
    double pour_time;
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
map<pair<int, int>, int> global_numeration(const vector<int>& lengths);



// Main

// Динамический алгоритм поиска всех вариантов выгрузки отсеков бензовоза на множестве резервуаров
pair<vector<int>, vector<vector<vector<string>>>> dp_max_unique_digits_all_masks(const map<int, vector<string>>& possible_combinations, int comp_num);

// Находит все допустимые комбинации отсеков между min[i] и max[i]
vector<vector<string>> possible_filling(const vector<int>& compartments, const vector<int>& mins, const vector<int>& maxs);

// Возвращает заполнения для выбранного грузовика и станций
vector<vector<string>> get_fillings(const Truck& truck, const vector<Station>& chosen_stations, const map<pair<int, int>, int>& gl_num);

set<vector<string>> all_fillings(
    const vector<Station>& stations, 
    const Truck& truck, 
    const vector<vector<int>>& time_to_station,
    const map<pair<int,int>,int>& gl_num,
    int H,
    int st_in_trip = 3, 
    int top_nearest = 4,
    map<int, int> local_index = {}
    );

set<vector<string>> find_routes(
    vector<Station> current_route, 
    const vector<Station>& stations,
    set<set<int>>& seen_routes, 
    const Truck& truck,
    const map<pair<int, int>, int>& gl_num,
    const map<int,int>& local_index,
    const vector<vector<int>>& time_to_station, 
    int current_time, 
    int st_in_trip, 
    int top_nearest,
    int H
    );

int two_pipes_opt(const vector<int>& fill_times);

// void log_time(const vector<string>& messages, vector<string>& time_log);

pair<int, vector<string>> compute_time_for_route(
    const map<int, pair<int, int>>& reverse_index,
    const vector<int>& compartments, 
    const vector<string>& fill,
    bool double_piped,
    const vector<Station>& input_station_list,
    const vector<vector<int>>& demanded_matrix,
    const vector<int>& docs_fill
    );