#ifndef EULER_H
#define EULER_H


class Euler
{
public:
    double m_dt;
    Euler(double dt);
    void IntegrateOneStep(class SolarSystem &system);

};

#endif // EULER_H
