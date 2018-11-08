#include <iostream>
#include <armadillo>
#include <vector>

using namespace std;
using namespace arma;


void print(std::vector<double> const &input);
void EnergyCalculation(const mat &Lattice, double &E, uword Nspins);
void MetropolisAlgorithm(uword Nspins, mat &Lattice, int MC, vector<double> &Energy, double T, vector<double> &MagneticMom);

inline uword PeriodicBoundary(uword i, uword limit, uword add) { //Theft
  return (i+limit+add) % (limit);
}

int main()
{
    uword Nspins = 2;
    double T = 1.0 ;
    int MC = 1000;
    vector<double> Energy;
    vector<double> MagneticMom;
    mat Lattice(Nspins, Nspins, fill::ones);

    MetropolisAlgorithm(Nspins, Lattice, MC, Energy, T, MagneticMom);

    uword sum_of_elems = 0;
    std::for_each(Energy.begin(), Energy.end(), [&] (uword n) {
        sum_of_elems += n;
    });
    cout << double (sum_of_elems) / double (MC) << endl;
    return 0;
} // MAIN END

void EnergyCalculation(const mat &Lattice, double &E, uword Nspins)
{
    //Calculate energy of whole lattice
    for (uword i = 0; i<Nspins; i++){
        for (uword j = 0; j<Nspins; j++){
            E -= double (Lattice(i,j)*( Lattice(PeriodicBoundary(i, Nspins, 1), j))
                    + Lattice(i, PeriodicBoundary(j, Nspins, 1)));
        }
    }
}

void MetropolisAlgorithm(uword Nspins, mat &Lattice, int MC, vector<double> &Energy, double T, vector<double> &MagneticMom)
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double>RandomGenerator(0.0, 1.0);
    double E0;
    double E;
    double w;

    for (int i=0; i<MC; i++){
        E0 = 0.0;
        E = 0.0;
        w = 0.0;

        EnergyCalculation(Lattice, E0, Nspins); //Calculate initial energy
        cout << E0 << endl;
        //Choose random position in lattice and flip spin
        uword x = uword ((RandomGenerator(gen)*Nspins));
        uword y = uword ((RandomGenerator(gen)*Nspins));
        Lattice(x, y) *= -1;
        cout << Lattice << endl;
        //Calculate new energy
        E = E0;
        EnergyCalculation(Lattice, E, Nspins);
        if (E-E0 < 0.0){ //Keep energy if new one is lower
            Energy.push_back(double (E0));
            MagneticMom.push_back(double (2*Lattice(x,y)));
        }
        else{ //If energy is larger
            w = exp(-1*(E-E0)/T);
            if (w >= double (RandomGenerator(gen))){ //If true, flip spin back again
                Lattice(x, y) *= -1;
            }
            else{ //Keep energy if not true
                Energy.push_back(double (E0));
                MagneticMom.push_back(double (2*Lattice(x,y)));
            }
        }

    }
}


//USED FOR PRINTING TYPE VECTOR IF NEEDED
void print(std::vector<double> const &input) //Theft
{
    for (uword i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}