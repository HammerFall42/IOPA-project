#include "./headers/modeler.h"

int main()
{
    Modeler mod;
    vector <vector <int>> res;
    vector <int> optimized;
    fstream wfile;
    float percent;

    cout << "Input percent: ";
    cin >> percent;

    wfile.open("result.txt", fstream::out);
    for (int i = 1; i < 31; ++i)
    {
        vector <int> temp = mod.StartModeling("day" + to_string(i) + ".txt");
        if (temp.empty())
        {
            cout << "No such file!";
            return -1;
        }

        wfile << temp[0] << temp[1] << temp[2] << endl;
        res.push_back({temp[0], temp[1], temp[2]});
    }

    wfile.close();
    optimized = mod.FindOptimized(res, percent);

    for (size_t i = 0; i < optimized.size(); ++i)
    {
        cout << "Drones of type \"Drone-" + to_string(i + 1) + "\": " << optimized[i] << endl;
    }

    return 0;
}
