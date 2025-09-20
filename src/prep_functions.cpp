#include "prep_functions.hpp"

using namespace std;

// Классы Truck и Station для дальнейшего использования в препроцессинге

Station::Station(int number, int time_to_depot, int time_from_depot,
            const std::vector<int>& demand, const std::vector<int>& remaining_spaces) :
  number(number),
  time_to_depot(time_to_depot),
  time_from_depot(time_from_depot),
  demand(demand),
  remaining_spaces(remaining_spaces),
  demanded_res(),
  filled_values(demand.size(), 0)               // инициализация нулями
{
for (int i = 0; i < demand.size(); i++) {       // проходим по запросам резервуаров станции и отмечаем номера тех, у которых ненулевой запрос
      if (demand[i] > 0) {
          demanded_res.push_back(i);
      }
  }
};


Truck::Truck(int number, const std::vector<int>& compartments, std::optional<int> ftid): 
  number(number), 
  compartments(compartments),
  remaining_time(12*60),
  total_trips(0),
  pour_time((accumulate(compartments.begin(), compartments.end(), 0) / 1000) * 3),
  FuelTankerID(ftid) 
{};


// Функции для работы с маской внутри алгоритма динамического программирования
//'245' → bool_mask = 52 (00110100) → mask_to_digits → '245'
int bool_mask(string s, int comp_num) {    // перевод строки из цифр - отсеков в булеву маску
    int mask = 0;
    for (char c : s) {                   // итерируемся по символам строки
        int d = c - '0';                 // получаем значение цифры за счёт вычитания ASCII кодов
        if (0 <= d && d <= comp_num) {
            int bit = 1 << d;            // сдвигаем бит на нужное место 
            mask |= bit;                 // совмещаем его с маской 
        }
    }
    return mask;
}

string mask_to_digits(int mask, int comp_num) {
    string s;
    for (int i = 0; i <= comp_num; ++i) {   // перебор всех битов
        if (mask & (1 << i)) {              // если есть бит
            s += char('0' + i);             // добавляем цифру (ASCII-код нуля + величина цифры)
        }
    }
    sort(s.begin(), s.end());   // сортируем по возрастанию
    return s;
}


// Алгоритм динамического программирования 
pair<vector<int>, vector<vector<vector<string>>>> dp_max_unique_digits_all_masks(
  const map<int, vector<string>>& possible_combinations, 
  int comp_num
) {
  map<int, vector<vector<string>>> dp;        // словарь вида маска цифр(отсеки) : вектор выбранных отсеков ("" если резервуар пропущен)
  dp[0] = {{}};                               // начальное состояние: ни одна цифра не использована, вектор векторов пуст
  
  for (const auto& [key, current_options] : possible_combinations) {
        map<int, vector<vector<string>>> next_dp;

        for (const auto& [mask, selections] : dp) {                   // перебираем все состояния (маски : выбор отсеков)
            for (const auto& selection : selections) {                // для каждой селекции выбранной маски 

                // пропускаем резервуар
                for (const auto& num_str : current_options) {         // выбираем один из вариантов заполнения текущего резервуара num_str (например, '254')
                    int num_mask = bool_mask(num_str, comp_num);      // берём маску от него, чтобы удобно пересекать

                    if ((mask & num_mask) == 0) {                     // если нет пересечений в масках (общих используемых отсеков)
                        int new_mask = mask | num_mask;               // совмещаем маски
                        vector<string> new_selection = selection;     
                        new_selection.push_back(num_str);             // добавляем в конец текущий вариант заполнения резервуара
                        next_dp[new_mask].push_back(new_selection);
                    }
                }

                // заполняем резервуар
                vector<string> new_selection = selection;
                new_selection.push_back("");                           // добавляем в конец "" - пропуск резервуара
                next_dp[mask].push_back(new_selection);
            }
        }

        dp = move(next_dp);
    }


    // находим максимальное число использованных битов
    int max_bits = 0;
    for (const auto& [mask, _] : dp) {
        int bits = __builtin_popcount(mask);      // считаем число битиков - это и есть число использованных отсеков
        if (bits > max_bits) max_bits = bits;     // и если что, обновляем максимум
    }
    
    // full_mask = 2**comp_num - 1
    //     if full_mask in dp:
    //         # возвращаем сразу варианты для полной маски
    //         return full_mask, dp[full_mask]

    // TODO: move'ать векторы здесь 

    // собираем все лучшие маски
    vector<int> best_masks;
    vector<vector<vector<string>>> best_selections;

    for (const auto& [mask, selections] : dp) {
        if (__builtin_popcount(mask) == max_bits) {
            best_masks.push_back(mask);
            best_selections.push_back(selections);
        }
    }

    return {best_masks, best_selections};
}


// аналог itertools.combinations
vector<vector<int>> combinations(int n, int k) {
    vector<vector<int>> result;

    // маска: k единиц, потом (n-k) нулей
    vector<bool> mask(n);
    fill(mask.begin(), mask.begin() + k, true);     // заполняем первые k True-шками

    do {                        // берём максимальную маску и двигаемся назад, перебирая все размещения 
        vector<int> comb;
        for (int i = 0; i < n; i++) {
            if (mask[i]) comb.push_back(i);             // по маске берём индексы
        }
        result.push_back(comb);
    } while (prev_permutation(mask.begin(), mask.end()));

    return result;
}



// функция, перебирающая все комбинации отсеков, чтобы узнать, какими комбинациями заполняются резервуары
// по завершении вызывает алгоритм ДП
vector<vector<string>> possible_filling(const vector<int>& compartments, const vector<int>& mins, const vector<int>& maxs) {
    map<int, vector<string>> possible_combinations;
    vector<vector<int>> result;

    for (int el_num = 1; el_num <= compartments.size(); el_num++) {      // число отсеков в комбинации
         for (auto comb : combinations(compartments.size(), el_num)) {     // перебираем комбинации с el_num числом отсеков
            int val = 0;
            for (int i : comb) {            // суммарная вместимость отсеков комбинации
                val += compartments[i];
            }
            for (int res_num = 0; res_num < mins.size(); res_num++) {   // проходим по резервуарам и проверяем, вмещается ли
                possible_combinations[res_num];                         // создаём set, если его еще нет, а иначе не перезапсиываем
                if (mins[res_num] <= val && val <= maxs[res_num]){
                    stringstream ss;
                    for (int i : comb) ss << i;
                    string s = ss.str();
                    possible_combinations[res_num].push_back(s);
                }
            }
        }
    }
    auto [best_masks, best_selections] = dp_max_unique_digits_all_masks(possible_combinations, compartments.size());

    if (best_masks[0] == (1 << compartments.size()) - 1) {  // есть вариант, в котором все отсеки использованы (он же и будет первым)
       return best_selections[0];
    }

}


// [[None, "z", "x"], ["a", None, "b"], [None, "c", "d"]] -> {[1, 0, 1], [0, 1, 1]}
set<vector<int>> boolify_reservoirs(const vector<vector<string>>& fillings) {
    set<vector<int>> masks;
    for (const auto& filling : fillings) {
        vector<int> mask;
        for (const auto& elem : filling) {
            mask.push_back(!elem.empty());      // если строка не пустая → 1, иначе 0
        }
        masks.insert(mask);
    }
    return masks;
}


// хочу вернуть словарь который по номеру станции и номеру резервуара даст глобальный номер
map<pair<int, int>, int> global_numeration(vector<int> lengths){
    int global_number = 0;
    map<pair<int, int>, int> num;

    for (int st_num = 0; st_num < lengths.size(); st_num++){
        int total_res = lengths[st_num];        // число резервуаров на станции
        int res_num = 0;                        // номер текущего резервуара на станции                
        while (res_num < total_res){
            num[{st_num, res_num}] = global_number;
            res_num += 1;
            global_number += 1;
        }
    }
    return num;
}

// Возвращает заполнения для выбранного грузовика и станций (в глобальной нумерации) 
vector<vector<int>> get_fillings(Truck truck, vector<Station> chosen_stations, map<pair<int, int>, int> gl_num){
    vector<int> mins, maxs;
    for (Station st : chosen_stations){
        mins.insert(mins.end(), st.demand.begin(), st.demand.end());
        maxs.insert(maxs.end(), st.remaining_spaces.begin(), st.remaining_spaces.end());
    }
    vector<vector<string>> fillings;
    fillings = possible_filling(truck.compartments, mins, maxs);
    return fillings;
}

    


// def get_fillings(truck: Truck, chosen_stations: list[Station], gl_num: dict[tuple[int, int], int]) -> list[list[bool]]:
//     "Возвращает заполнения для выбранного грузовика и станций (в глобальной нумерации)"
//     mins, maxs = [], []
//     for station in chosen_stations:         # объединяем массивы демандов и максимумов для нескольких станций
//         mins.extend(station.demand)
//         maxs.extend(station.remaining_space)
//     # print(mins,maxs)
    
//     local_to_global = []    # в соответствие со словарём достаём глобальные индексы 
//     for st in chosen_stations:
//         num_res = len(st.demand)        # число резервуаров на станции
//         for res_idx in range(num_res):
//             local_to_global.append(gl_num[(st.number, res_idx)])  # добавляем индекс


//     f = possible_filling(truck.compartments, mins, maxs)
//     # print(f'Total number before boolification {len(f)}')    
    
//     if f is None: 
//         return
    
//     # print(f)
//     # f = bool_res(f)
//     # print(f'Total number after boolification {len(f)}')
    
//     total_res_num = len(gl_num.keys())      # всего резервуаров             
    
//     global_fillings = []                    # TODO: в идеале мне бы сразу индексы единичек хранить в словаре а не пробегаться по всему массиву (тогда в следующей функции делать sorted по ключам)
//     for filling in f:
//         filling_arr = np.array(filling)
//         indices = np.nonzero(filling_arr)[0]                # индексы резервуаров которые мы решили заполнять
//         curr_fill = [0 for _ in range(total_res_num)]       # глобальный filling
//         for local_idx in indices:
//             global_idx = local_to_global[local_idx]         # достаём глобальный индекс
//             curr_fill[global_idx] = filling[local_idx]      # ставим нужные отсеки
//         global_fillings.append(curr_fill)

//     return global_fillings