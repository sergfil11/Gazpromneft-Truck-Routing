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
    vector<double>
    >
gurobi_preprocessing(
    int N,
    int H,
    int K,
    const vector<vector<int>>& time_matrix,
    const vector<map<string, int>>& depot_times,
    const vector<map<string, vector<int>>>& stations,
    const vector<vector<int>>& trucks,
    int R1,
    int R2,
    vector<vector<int>>& access,
    vector<bool>& double_piped,
    double daily_coefficient,
    vector<int>& docs_fill,
    vector<double>& H_k,
    vector<bool>& loading_prepared,
    const map<int, vector<string>>& reservoir_to_product,
    const map<int, set<tuple<string>>>& truck_to_variants
  );