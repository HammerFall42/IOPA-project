#include "./headers/drone.h"

Drone::Drone(string name, float min_dist, float max_dist, float mass, float velocity, float price, int type)
{
    _name = name;
    _min_dist = min_dist;
    _max_dist = max_dist;
    _mass = mass;
    _velocity = velocity;
    _price = price;
    _Q = (_max_dist - _min_dist) / _mass;
    _res = 1440;
    _type = type;
}

bool Drone::CheckRequest(float mass, float dist)
{
    return ((_max_dist - mass * _Q - dist) > dist && mass <= _mass);
}
