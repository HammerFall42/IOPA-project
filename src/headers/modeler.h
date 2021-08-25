#ifndef MODELER_H
#define MODELER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "drone.h"

using namespace std;

#define T_CONST 30

struct request
{
    float _dist;
    float _mass;
    int _type;
};

class Modeler
{
public:
    Modeler() {}
    ~Modeler() {}

    vector <int> StartModeling(string filename);
    vector <int> FindOptimized(vector <vector <int>>& res, float percent);

private:
    int ReadFile(string filename);
    void SortReqs(vector <Drone>& types);
    int BuyDrones(vector <Drone>& types);
    int FillDrone(Drone& drone);
    int CompareDrones(Drone& drone_1, Drone& drone_2);
    vector <Drone> RecurSearch(vector <request> reqs, vector <Drone> drones, vector <Drone>& types);
    void PrintResults(vector <int>& res, size_t summ);
    vector <int> FindMax(vector <vector <int>>& res);
    vector <int> FindFrequent(vector <vector <int>>& res, vector <int>& max);
    vector <int> FindForPercent(vector <vector <int>>& res, vector <int>& max, float percent);

    vector <request> _reqs;
    vector <Drone> _drones;
    vector <int> _prices;
};

#endif // MODELER_H
