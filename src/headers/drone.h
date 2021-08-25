#ifndef DRONE_H
#define DRONE_H

#include <string>

using namespace std;

class Drone
{
public:
    Drone(string name, float min_dist, float max_dist, float mass, float velocity, float price, int type);
    ~Drone() {}

    string Name() {return _name;}
    int Price() {return _price;}
    float Q() {return _Q;}
    float Res() {return _res;}
    float MaxDist() {return _max_dist;}
    float Velocity() {return _velocity;}
    int Type() {return _type;}
    void DecRes(float diff) {_res -= diff;}
    bool CheckRequest(float mass, float dist);

private:
    string _name;
    float _min_dist;
    float _max_dist;
    float _mass;
    float _velocity;
    int _price;
    float _res;
    float _Q;
    int _type;
};

#endif // DRONE_H
