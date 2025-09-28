#include "prep_functions.hpp"
#include <numeric>
#include <iostream>
#include <chrono>


/**
 * Предварительная обработка данных для оптимизации солвером Gurobi.
 *
 * @param N                 Число станций (не учитывая депо)
 * @param H                 Длительность смены в минутах
 * @param K                 Число бензовозов
 * @param time_matrix       Матрица длительностей поездок между станциями
 * @param depot_times       Массив длительностей поездок от станций до депо (и обратно)
 * @param stations          Минимальные и максимальные потребности станций в топливе
 * @param trucks            Вместимости отсеков бензовозов
 * @param R1                Число станций в маршруте
 * @param R2                Число рассматриваемых ближайших станций
 * @param access            Матрица допустимых станций для бензовозов
 * @param double_piped      Булев массив, указывающий на возможности использования двух шлангов
 * @param daily_coefficient Коэффициент длительности поездок между станциями
 * @param docs_fill         Время на заполнение документов на станции
 * @param H_k               Массив длительностей смен для бензовозов
 * @param loading_prepared  Массив отметок, что бензовоз загружен под сменщика
 *
 * @return tuple Кортеж, содержащий
 *         - filling_on_route  Заполнение резервуаров отсеками в глобальной нумерации различными видами топлива
 *         - sigma             Времена выполнения маршрутов
 *         - reservoirs        Минимальные и максимальные потребности резервуаров в топливе
 *         - tank_count        Общее число резервуаров
 *         - H_k               Массив длительностей смен для бензовозов (с учётом времени, добавленного в смену вледствие загрузки под сменщика после обработки loading_prepared)
 */

tuple< 
    map<int, vector<vector<int>>>,
    map<pair<int, int>, int>,
    vector<map<string, int>>, 
    int,
    map<pair<int, int>, int>, 
    map<pair<int, int>, vector<string>>,
    vector<float>
    >
gurobi_preprocessing(
  int N,
  int H,
  int K,
  vector<vector<int>> time_matrix,
  vector<map<string, int>> depot_times ,
  vector<map<string, vector<int>>> stations,
  vector<vector<int>> trucks,
  int R1,
  int R2,
  vector<vector<int>> access,
  vector<bool> double_piped,
  float daily_coefficient,
  vector<int> docs_fill,
  vector<float> H_k,
  vector<bool> loading_prepared,
  map<int, vector<string>> reservoir_to_product,
  map<int, set<tuple<string>>> truck_to_variants
  ){

  // значения по умолчанию
  if (H_k.empty())
    H_k = vector<float>(K, H);
  if (access.empty())                                         
    access = vector<vector<int>> (N, vector<int>(K, 1));
  if (double_piped.empty())
      double_piped = vector<bool> (K, false); 
  if (docs_fill.empty())
      docs_fill = vector<int> (N, 2400);
  if (loading_prepared.empty())
      loading_prepared = vector<bool> (K, false);

  // если загружен под сменщика, увеличиваем длину смены на время заполнения в депо
  for (int truck = 0; truck < K; ++truck) {
    if (loading_prepared[truck]) {
        int sum_truck = accumulate(trucks[truck].begin(), trucks[truck].end(), 0);
        H_k[truck] += (sum_truck / 1000.0) * 3;
    }
  }

  // применяем дневной коэффициент к матрице перемещений
  vector<vector<double>> coef_time_matrix(time_matrix.size(), vector<double>(time_matrix[0].size()));
  for (int i = 0; i < time_matrix.size(); ++i) {
    for (int j = 0; j < time_matrix[i].size(); ++j) {
        coef_time_matrix[i][j] = (time_matrix[i][j] * daily_coefficient > 900)      // если время больше 900, то оставляем 900 (итак больше длины смены)
                                  ? 900
                                  : time_matrix[i][j] * daily_coefficient;
    }
  }

  // ищем пустые станции
  vector <int> demanded_idx = {};
    for (int st_number = 0; st_number < N; st_number++) {
        if (stations[st_number].count("max") && !stations[st_number].at("max").empty())   // если есть ключ "max" и вектор по ключу "max" не пуст
            demanded_idx.push_back(st_number);
    }
    cout << "Станции с ненулевыми запросами " << demanded_idx.size() << endl;

  // "срезаем" пустые станции в матрице времён
  vector<vector<double>> demanded_matrix(demanded_idx.size(), vector<double>(demanded_idx.size()));
  for (int i = 0; i < demanded_idx.size(); ++i) {
      for (int j = 0; j < demanded_idx.size(); ++j) {
          demanded_matrix[i][j] = coef_time_matrix[demanded_idx[i]][demanded_idx[j]];
      }
  }
  
  vector<vector<int>> cut_access;
  vector<map<string, vector<int>>> demanded_st;
  vector<map<string, int>> demanded_depot_times;

  // "срезаем" пустые станции в векторах access и stations
  for (int idx : demanded_idx) {
      cut_access.push_back(access[idx]);
      demanded_st.push_back(stations[idx]);
      demanded_depot_times.push_back(depot_times[idx]);
  }

  // создаём вектор станций
  vector<Station> input_station_list;
  for (int i = 0; i < demanded_st.size(); ++i) {
    Station st(
        i,          // номер станции в срезе
        demanded_depot_times[i].at("to") * daily_coefficient,
        demanded_depot_times[i].at("from") * daily_coefficient,
        demanded_st[i].at("min"),
        demanded_st[i].at("max")
    );
    input_station_list.push_back(move(st));
  }

  vector<int> lengths;  // число резервуаров на станции
  for (int i = 0; i < demanded_st.size(); ++i)
    lengths.push_back(demanded_st[i].at("max").size());

  // глобальная нумерация выбранных резервуаров
  map<pair<int, int>, int> gl_num = global_numeration(lengths);

  auto t1 = chrono::system_clock::now();  
  
   
 set<pair<int, vector<string>>> result = {};
 for (int idx = 0; idx < trucks.size(); ++idx) {
      vector <int> truck = trucks[idx];
      // станцию и матрицу времени нужно обрезать в случае ограничений a_ik

      // допустимые станции для бензовоза
      vector<int> accessible;
      for (auto& row : cut_access) {
          accessible.push_back(row[idx]);
      }

      // индексы допустимых станций
      vector<int> accessible_idx;
      for (size_t i = 0; i < accessible.size(); ++i) {
          if (accessible[i] == 1) {
              accessible_idx.push_back(i);
          }
      }

      // допустимые станции
      vector<Station> accessible_st;
      for (int i : accessible_idx) {
          accessible_st.push_back(input_station_list[i]);
      }
      
      // часть матрицы времени, соответствующая допустимым станциям бензовоза
      vector<vector<double>> accessible_matrix;
      for (int i : accessible_idx) {
          vector<double> row;
          for (int j : accessible_idx) {
              row.push_back(demanded_matrix[i][j]);
          }
          accessible_matrix.push_back(move(row));
      }
      
      map<int, int> local_index = {};      // сопоставление номера станции и её индекса (нумерация поданных станций)
      for (int i = 0; i < accessible_st.size(); ++i) {
          local_index[accessible_st[i].number] = i;
      }
      
      // для параметров (1..R1)
      for (int r = 1; r < R1+1; r++) {
          set<vector<string>> val = all_fillings(accessible_st, Truck(idx, truck), accessible_matrix, gl_num, H_k[idx], r, R2, local_index);
          
          // print(f'Маршрутов для бензовоза {idx}: {len(val)}')
          for (vector<string> elem : val){
              result.insert({idx, elem});
              // print(elem)
          }
      }
  }

  auto t2 = chrono::system_clock::now();
  cout << "Время вычисления маршрутов:" << roundN(chrono::duration<double>(t2 - t1).count(), 3) << " сек." << endl;
  cout << "Всего маршрутов:"  << result.size() << endl;


  // заполнение резервуаров (в глобальной нумерации) бензовозами на маршрутах
  map<int, vector<vector<string>>> filling_on_route = {};                      // n_truck, truck_fillings
  for (pair<int, vector<string>> elem : result) {                 
    filling_on_route[elem.first].push_back(elem.second);
  }

  // перевод демандов станций в деманды резервуаров (проходимся по всем станциям и добавляем все их резервуары в список)
  vector<map<string, int>> reservoirs = {};        
  for (int i = 0; i < demanded_st.size(); ++i){
    map<string, vector<int>> st = demanded_st[i];
    for (int j = 0; j < st["min"].size(); ++j){
        reservoirs.push_back({{"min", st["min"][j]}, {"max", st["max"][j]}});
    }   
  }          
  int tank_count = reservoirs.size();
  
  t1 = chrono::system_clock::now(); 
  
  map<int, pair<int,int>> reverse_global;
  for (const pair<const pair<int, int>, int>& kv : gl_num) {
    reverse_global[kv.second] = kv.first;
  }
  
  map<pair<int,int>, int> sigma;
  map<pair<int,int>, vector<string>> timelogs;
  for (int truck = 0; truck < K; ++truck) {
    if (filling_on_route.count(truck) > 0) {
        for (int route = 0; route < filling_on_route.at(truck).size(); ++route) {
            const vector<string>& fill = filling_on_route.at(truck)[route];
            auto [computed_time, timelog] = compute_time_for_route(
                reverse_global,
                trucks[truck],
                fill,
                double_piped[truck],
                input_station_list,
                demanded_matrix,
                docs_fill
            );
            sigma[{truck, route}] = computed_time;
            timelogs[{truck, route}] = timelog;
        }
    }
  }

  // по паре (номер бензовоза, паттерн заполнения) возвращаем маршрут с минимальным временем, соответствующий этому паттерну (его время, заполнение и лог)
  map<pair<int, vector<int>>, tuple<int, vector<string>, vector<string>>> best_by_pattern = {};         
  for (int truck = 0; truck < K; ++truck) {                                                             
    if (filling_on_route.count(truck) > 0) {
        for (int route_idx = 0; route_idx < filling_on_route.at(truck).size(); ++route_idx) {
            vector<string> fill = filling_on_route.at(truck)[route_idx];
            
            vector<int> pattern = {};
            for (int i = 0; i < fill.size(); ++i) {
                pattern.push_back(fill[i].empty() ? 0 : 1);
            }

            int r_time = sigma[{truck, route_idx}];
            vector<string> r_log = timelogs[{truck, route_idx}];
            pair<int, vector<int>> key = {truck, pattern};

            if (best_by_pattern.count(key) == 0 or r_time < get<0>(best_by_pattern[key]))       // если еще нет такого паттерна, или время лучше, обновляем
                best_by_pattern[key] = {r_time, fill, r_log};
        }
    }
  }
  cout << "Лучших по паттерну маршрутов:"  << best_by_pattern.size() << endl;

  //  возможно надо сначала создать ключи а потом добавлять
  map<int, vector<vector<int>>> new_filling_on_route;  // для каждого бензовоза пустой список маршрутов
  map<pair<int,int>, int> new_sigma;
  map<pair<int,int>, vector<string>> new_log;

  // перезаписываем сигму
  for (const auto& [key, value] : best_by_pattern) {
        const auto& [truck, pattern] = key;
        const auto& [best_time, fill, log] = value;

        int new_route_idx = new_filling_on_route[truck].size();
        new_filling_on_route[truck].push_back(pattern);              // можно и сам fill записать, но запишу паттерн
        new_sigma[{truck, new_route_idx}] = best_time;
        new_log[{truck, new_route_idx}] = log;
  }

  int total_routes = 0;
  for (auto& [truck, routes] : new_filling_on_route)
      total_routes += routes.size();

  cout << "Маршрутов:" << total_routes << endl;
  cout << "Сигм:" << new_sigma.size() << endl;
  cout << "Лог:" << new_log.size() << endl;

  t2 = chrono::system_clock::now();
  cout << "Время вычисления длительностей:" << roundN(chrono::duration<double>(t2 - t1).count(), 3) << " сек." << endl;


  return make_tuple(new_filling_on_route, new_sigma, reservoirs, tank_count, gl_num, new_log, H_k);
}