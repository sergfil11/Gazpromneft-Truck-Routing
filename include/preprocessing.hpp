#pragma once

#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <set>

using namespace std;

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
  );