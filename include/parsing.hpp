#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

struct RealData {
    int N;
    int H;
    int K;
    vector<vector<double>> time_matrix;
    vector<map<string, double>> depot_times;
    vector<map<string, vector<double>>> stations;
    vector<vector<double>> trucks;
    vector<vector<int>> access;
    vector<bool> dual_piped;
    double daily_coefficient;
    vector<double> docs_fill;
    vector<double> H_k;
    vector<int> loading_prepared;
    vector<int> owning;
};


RealData load_real_data(const string& name);

void print_input(const RealData& d);