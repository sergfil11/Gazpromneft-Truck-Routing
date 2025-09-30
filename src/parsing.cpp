#pragma once
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "parsing.hpp"


using json = nlohmann::json;
using namespace std;


RealData load_real_data(const string& name) {
    string full_path = name + ".json";
    cout << "Trying to open file: " << full_path << endl;

    ifstream f(full_path);
    if (!f.is_open()) {
        cerr << "Cannot open file!" << endl;
        return {};
    }
    else {
        cout << "File opened successfully!" << endl;
    }
    
    json data;
    f >> data;

    cout << "Data read successfully!" << endl;

    RealData result;
    result.N = data["N"];
    result.H = data["H"];
    result.K = data["K"];
    result.time_matrix = data["time_matrix"].get<vector<vector<double>>>();
    result.depot_times = data["depot_times"].get<vector<map<string, double>>>();
    result.stations = data["stations"].get<vector<map<string, vector<double>>>>();
    result.trucks = data["trucks"].get<vector<vector<int>>>();
    result.access = data["access"].get<vector<vector<int>>>();
    result.dual_piped = data["dual_piped"].get<vector<bool>>();
    result.daily_coefficient = data["daily_coefficient"];
    result.docs_fill = data["docs_fill"].get<vector<double>>();
    result.H_k = data["H_k"].get<vector<double>>();
    result.loading_prepared = data["loading_prepared"].get<vector<int>>();
    result.owning = data["owning"].get<vector<int>>();

    cout << "Data loaded successfully!" << endl;
    return result;
}



void print_input(const RealData& d) {
    cout << "N = " << d.N << "\n";
    cout << "H = " << d.H << "\n";
    cout << "K = " << d.K << "\n";

    cout << "\nTime matrix:\n";
    for (auto& row : d.time_matrix) {
        for (auto v : row) cout << v << " ";
        cout << "\n";
    }

    cout << "\nDepot times:\n";
    for (auto v : d.depot_times) {
        for (auto p : v) cout << p.first << ":" << p.second << " ";   
    }
    cout << "\n";

    cout << "\nStations:\n";
    for (auto& st : d.stations) {
        for (auto v : st){ cout << v.first << ":";
            for (auto v2 : v.second) cout << v2 << " ";
        }
        cout << "\n";
    }

    cout << "\nTrucks:\n";
    for (auto& tr : d.trucks) {
        for (auto v : tr) cout << v << " ";
        cout << "\n";
    }

    cout << "\nAccess:\n";
    for (auto& row : d.access) {
        for (auto v : row) cout << v << " ";
        cout << "\n";
    }

    cout << "\nDual piped:\n";
    for (auto v : d.dual_piped) cout << v << " ";
    cout << "\n";

    cout << "\nDaily coefficient = " << d.daily_coefficient << "\n";

    cout << "\nDocs fill:\n";
    for (auto v : d.docs_fill) cout << v << " ";
    cout << "\n";

    cout << "\nH_k:\n";
    for (auto v : d.H_k) cout << v << " ";
    cout << "\n";

    cout << "\nLoading prepared:\n";
    for (auto v : d.loading_prepared) cout << v << " ";
    cout << "\n";

    cout << "\nOwning:\n";
    for (auto v : d.owning) cout << v << " ";
    cout << "\n";
}
