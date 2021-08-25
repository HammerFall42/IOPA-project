#include "./headers/modeler.h"

int Modeler::ReadFile(string filename)
{
    ifstream rfile;

    rfile.open(filename, fstream::in);

    if (!rfile.is_open())
    {
        return -1;
    }

    request item;
    while (!rfile.eof())
    {
        rfile >> item._mass;
        rfile >> item._dist;
        _reqs.push_back(item);
    }
    rfile.close();

    return 0;
}

vector <int> Modeler::StartModeling(string filename)
{
    _reqs.clear();
    _drones.clear();
    vector <Drone> types{Drone("Дрон-1", 19440, 43200, 5500, 18, 432000, 0),
                        Drone("Дрон-2", 28800, 45600, 2340, 20, 400000, 1),
                        Drone("Дрон-3", 35000, 49000, 1000, 10, 165500, 2)};
    size_t summ = 0;
    vector <int> drone_count = {0, 0, 0};
    _prices = {types[0].Price(), types[1].Price(), types[2].Price()};

    if (ReadFile(filename))
    {
        return {};
    }
    SortReqs(types);
    int req_count = BuyDrones(types);

    if (!req_count)
    {
        for (size_t i = 0; i < _drones.size(); ++i)
        {
            ++drone_count[_drones[i].Type()];
            summ += _drones[i].Price();
        }

        //PrintResults(drone_count, summ);
        return drone_count;
    }

    for (size_t i = 0; i < _drones.size(); ++i)
    {
        req_count -= FillDrone(_drones[i]);
    }

    if (!req_count)
    {
        for (size_t i = 0; i < _drones.size(); ++i)
        {
            ++drone_count[_drones[i].Type()];
            summ += _drones[i].Price();
        }

        //PrintResults(drone_count, summ);
        return drone_count;
    }

    vector <Drone> temp = RecurSearch(_reqs, {}, types);
    _drones.insert(_drones.end(), temp.begin(), temp.end());

    for (size_t i = 0; i < _drones.size(); ++i)
    {
        ++drone_count[_drones[i].Type()];
        summ += _drones[i].Price();
    }

    //PrintResults(drone_count, summ);

    return drone_count;
}

void Modeler::PrintResults(vector <int>& res, size_t summ)
{
    cout << "Drone-1 count: " << res[0] << endl;
    cout << "Drone-2 count: " << res[1] << endl;
    cout << "Drone-3 count: " << res[2] << endl;
    cout << "Summary cost: " << summ << endl;
}

void Modeler::SortReqs(vector <Drone>& types)
{
    vector <request> temp;

    while (_reqs.size() > 0)
    {
        float max = _reqs[0]._dist / types[0].Velocity() / 60 * 2 + T_CONST;
        int maxid = 0;
        for (int i = 0; i < _reqs.size(); ++i)
        {
            float diff = _reqs[i]._dist / types[0].Velocity() / 60 * 2 + T_CONST;
            if (diff > max)
            {
                max = diff;
                maxid = i;
            }
        }
        temp.push_back(_reqs[maxid]);
        _reqs.erase(_reqs.begin() + maxid);
    }

    _reqs = temp;
}

vector <Drone> Modeler::RecurSearch(vector <request> reqs, vector <Drone> drones, vector<Drone> &types)
{
    if (!reqs.size()) return drones;

    vector <Drone> res;
    size_t minsumm;

    for (size_t k = 0; k < 3; ++k)
    {
        vector <request> tempr = reqs;
        vector <Drone> tempd = drones;

        bool flag = false;

        for (size_t i = 0; i < tempr.size(); ++i)
        {
            if (types[k].CheckRequest(tempr[i]._mass, tempr[i]._dist))
            {
                flag = true;
                break;
            }
        }

        if (!flag) continue;

        tempd.push_back(types[k]);
        int ind = tempd.size() - 1;
        for (size_t i = 0; i < tempr.size(); ++i)
        {
            if (types[k].CheckRequest(tempr[i]._mass, tempr[i]._dist))
            {
                float diff = tempr[i]._dist / tempd[ind].Velocity() / 60 * 2 + T_CONST;
                if ((tempd[ind].Res() - diff) > 0)
                {
                    tempd[ind].DecRes(diff);
                    tempr.erase(tempr.begin() + i);
                    --i;
                }
            }
        }

        vector <Drone> tempres = RecurSearch(tempr, tempd, types);
        size_t summ = 0;

        for (size_t j = 0; j < tempres.size(); ++j)
        {
            summ += tempres[j].Price();
        }

        if (res.empty())
        {
            res = tempres;
            minsumm = summ;
        }
        else if (summ < minsumm)
        {
            res = tempres;
            minsumm = summ;
        }
    }

    return res;
}

int Modeler::BuyDrones(vector <Drone>& types)
{
    int req_count[4] = {0, 0, 0, 0};
    int cur_drone = -1;

    for (size_t i = 0; i < _reqs.size(); ++i)
    {
        if (types[0].CheckRequest(_reqs[i]._mass, _reqs[i]._dist) &&
                !types[1].CheckRequest(_reqs[i]._mass, _reqs[i]._dist) &&
                !types[2].CheckRequest(_reqs[i]._mass, _reqs[i]._dist))
        {
            req_count[0]++;
            _reqs[i]._type = 0;
        }
        else if (types[1].CheckRequest(_reqs[i]._mass, _reqs[i]._dist) &&
                 !types[0].CheckRequest(_reqs[i]._mass, _reqs[i]._dist) &&
                 !types[2].CheckRequest(_reqs[i]._mass, _reqs[i]._dist))
        {
            req_count[1]++;
            _reqs[i]._type = 1;
        }
        else if (types[2].CheckRequest(_reqs[i]._mass, _reqs[i]._dist) &&
                 !types[0].CheckRequest(_reqs[i]._mass, _reqs[i]._dist) &&
                 !types[1].CheckRequest(_reqs[i]._mass, _reqs[i]._dist))
        {
            req_count[2]++;
            _reqs[i]._type = 2;
        }
        else
        {
            req_count[3]++;
            _reqs[i]._type = 3;
        }
    }

    for (size_t k = 0; k < 3; ++k)
    {
        while (req_count[k] > 0)
        {
            _drones.push_back(types[k]);
            ++cur_drone;
            for (int i = 0; i < _reqs.size(); ++i)
            {
                if (_reqs[i]._type == _drones[cur_drone].Type())
                {
                    float diff = _reqs[i]._dist / _drones[cur_drone].Velocity() / 60 * 2 + T_CONST;
                    if ((_drones[cur_drone].Res() - diff) > 0)
                    {
                        _drones[cur_drone].DecRes(diff);
                        --req_count[k];
                        _reqs.erase(_reqs.begin() + i);
                        --i;
                    }
                }
            }
        }
    }

    return req_count[3];
}

int Modeler::FillDrone(Drone & drone)
{
    bool flag = true;
    int req_count = 0;

    while (flag && _reqs.size() > 0)
    {
        flag = false;
        float min = _reqs[0]._dist / drone.Velocity() / 60 * 2 + T_CONST;
        size_t minid = 0;

        for (size_t i = 1; i < _reqs.size(); ++i)
        {
            float diff = _reqs[i]._dist / drone.Velocity() / 60 * 2 + T_CONST;
            if (drone.CheckRequest(_reqs[minid]._mass, _reqs[minid]._dist) && diff < min)
            {
                min = diff;
                minid = i;
            }
        }

        if (drone.CheckRequest(_reqs[minid]._mass, _reqs[minid]._dist) && drone.Res() - min > 0)
        {
            flag = true;
            drone.DecRes(min);
            _reqs.erase(_reqs.begin() + minid);
            ++req_count;
        }
    }

    return req_count;
}

vector <int> Modeler::FindOptimized(vector <vector <int>> &res, float percent)
{
    vector <int> max = FindMax(res);
    //vector <int> frequent = FindFrequent(res, max);
    return FindForPercent(res, max, percent);
}

vector <int> Modeler::FindMax(vector <vector <int>> &res)
{
    vector <int> max = {res[0][0], res[0][1], res[0][2]};
    for (size_t i = 0; i < res.size(); ++i)
    {
        for (size_t j = 0;j < 3; ++j)
        {
            if (res[i][j] > max[j])
            {
                max[j] = res[i][j];
            }
        }
    }

    return max;
}

vector <int> Modeler::FindFrequent(vector <vector <int>> &res, vector <int>& max)
{
    vector <vector <int>> frequent = {vector<int>(max[0]), vector<int>(max[1]), vector<int>(max[2])};
    vector <int> res_frequent = {0, 0, 0};

    for (size_t i = 0; i < res.size(); ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            frequent[j][res[i][j]]++;
        }
    }

    for (size_t i = 0; i < frequent.size(); ++i)
    {
        int m = frequent[i][0];
        for (int j = 0; j < frequent[i].size(); ++j)
        {
            if (frequent[i][j] > m)
            {
                m = frequent[i][j];
                res_frequent[i] = j;
            }
        }
    }

    return res_frequent;
}

vector <int> Modeler::FindForPercent(vector <vector <int>>& res, vector <int>& max, float percent)
{
    vector <int> cur = {0, 0, 0};
    vector <int> res_frequent;
    int summ = 0;

    for (cur[0] = 0; cur[0] <= max[0]; ++cur[0])
    {
        for (cur[1] = 0; cur[1] <= max[1]; ++cur[1])
        {
            for (cur[2] = 0; cur[2] <= max[2]; ++cur[2])
            {
                int count = 0;
                for (size_t i = 0; i < res.size(); ++i)
                {
                    if ((cur[0] >= res[i][0]) && (cur[1] >= res[i][1]) && (cur[2] >= res[i][2]))
                    {
                        ++count;
                    }
                }

                float temp_percent = ((float)(count)) / ((float)(res.size()));
                if (temp_percent >= percent)
                {
                    if (res_frequent.empty())
                    {
                        res_frequent = cur;
                        summ = res_frequent[0] * _prices[0] + res_frequent[1] * _prices[1] + res_frequent[2] * _prices[2];
                    }
                    else
                    {
                        int temp_summ = cur[0] * _prices[0] + cur[1] * _prices[1] + cur[2] * _prices[2];
                        if (temp_summ < summ)
                        {
                            res_frequent = cur;
                            summ = temp_summ;
                        }
                    }
                }
            }
        }
    }

    return res_frequent;
}
