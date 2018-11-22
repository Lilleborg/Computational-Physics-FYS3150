#ifndef POPULATIONS_H
#define POPULATIONS_H
#include<vector>
#include<string>
#include "vec3.h"
using namespace std;

class Populations
{
public:
    //TRANSMISSION RATES
    double a;
    double b;
    double c;

    //POPULATION NAME
    string name;

    //CURRENT VALUES ODES
    vector<vec3> u_i;

    //VECTOR FOR STORING SOLUTIONS TO DIFFERENTIAL EQUATIONS
    //u[0] = S, u[1] = I, u[2] = R
    vector<vec3> u;

    //How to deal with intial conditions?
    Populations();
};

#endif // POPULATIONS_H
