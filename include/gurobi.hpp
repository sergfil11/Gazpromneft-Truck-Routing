#include "gurobi_c++.h"

void gurobi_results(
    GRBModel& model,
    const map<int, GRBVar>& y,
    const map<pair<int,int>, GRBVar>& g,
    const map<int, vector<vector<int>>>& filling_on_route,   // грузовику -> список маршрутов -> список заполнений
    const map<pair<int,int>, int>& gl_num,                   // (станция, резервуар) -> глобальный номер
    const map<pair<int,int>, vector<string>>& log,           // (k,r) -> лог действий
    const map<pair<int,int>, double>& sigma                  // (k,r) -> время маршрута
);

GRBModel gurobi_covering(
    const map<int, vector<vector<int>>>& filling_on_route,  // маршруты
    const map<pair<int,int>, int>& sigma,                   // время на маршрут
    const vector<map<string,int>>& reservoirs,              // {min,max}
    int tank_count,
    int H = 720,
    int K = 20,
    int timelimit = 900,
    const map<pair<int,int>, int>& gl_num = {},
    const vector<double>& H_k = {},
    vector<int> owning = {}
);