#include <iostream>
#include <math.h>
//#include "populations.h"

#include "problem.h"
#include "population/a_seasons.h"
#include "population/a_constant.h"

using namespace std;

int main()
{
    // Oppgave A
    problem A(400,300);
    a_seasons *a_s = new a_seasons(4,1,M_PI);
    A.set_afunc(a_s);
    A.set_population();
    a_constant a_c(3);
    cout << a_c.a(2) << " " << a_s->a(1.0/2) << " " << A.m_S->m_a_param->a(1.0/2) << endl;





    // Initialize populations
//    vector<double> as = {4,4,4,4};
//    vector<double> bs = {1,2,3,4};
//    vector<double> cs = {0.5,0.5,0.5,0.5};
//    vector<Populations> m_populations;
//    for (int i = 0; i < 4; ++i) {
//        m_populations
//    }
//    Popa = Populations()






    cout << "Hello World!" << endl;
    return 0;
}
