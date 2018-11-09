#include <iostream>
#include <armadillo>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace arma;

// DECLARE FUNCTIONS
void initialize_new_round(int Nspins, imat& Lattice, double &Energy, double &MagneticMom);
void metropolis(int Nspins, int MCs, double T,__1::mt19937_64 gen, imat &Lattice, vec &w, double &E, double &M);
void results_per_mc(double *quantity,int MC, string filename);

inline int PeriodicBoundary(int i, int limit, int add) { //Theft
    return (i+limit+add) % (limit);
}

int main()
{
    int Nspins = 2;
    double T,E,M;;    // temperatur, energy and magnetic moment
    T = 2.4;
    int MC = 1e5;      // # cycles

    vec ExpectationVals(5,fill::zeros); // Vector for holding expectation values
    // Initialize lattice matrix
    imat Lattice(Nspins, Nspins, fill::ones);   // imat giving typedef <sword> matrix - armadillos integer type
    double *Energies = new double[MC];  // FOR SAVING ENERGIES PER MC CYCLE, ONLY USED WHEN LOOKING AT PATH TO EQUILIBRIUM
    double *MagneticMom = new double[MC];   // SAME AS "Energies", but for magnetic moment


    // Initialize seed CHANGE TO DEPEND ON MY_RANK EVENTUALLY
    random_device rd;
    mt19937_64 gen(rd());

    // FROM HERE WE SHOULD LOOP OVER TEMPERATURES EVENTUALLY
    // Precalculating possible energy differences, fill vector of probability ratios
    vec w(17);

    for (int dE = -8; dE <= 8; dE+=4) {
        w(dE+8) = exp(-dE/T);
    }
    cout << w << endl;
    /*
    vec w(5,fill::zeros), dE = linspace(-8,8,5);
    for (int i = 0; i < dE.size(); ++i) {
        w(i) = exp(-dE(i)/T);
    }*/
    initialize_new_round(Nspins,Lattice,E,M);   // Sets up Lattice, calc M and E all spins up, WILL BE MORE USEFULL FOR MULTIPLE TEMPERATURES

    // STARTING MC CYCLES
    for (int cycle = 0; cycle < MC; cycle ++){

    metropolis(Nspins,MC,T,gen,Lattice,w,E,M);  // One cycle through the lattice
    Energies[cycle] = E;
    MagneticMom[cycle] = M;
    // Update expectation values:
    ExpectationVals(0) += E; ExpectationVals(1) += E*E;
    ExpectationVals(2) += M; ExpectationVals(3) += M*M; ExpectationVals(4) += fabs(M);

    }   // END MC CYCLES
    // WRITE TO FILES
    string fileending = "energies_" + to_string(T) + ".bin";
    results_per_mc(Energies,MC,"energies_"+fileending);
    results_per_mc(MagneticMom,MC,"magnetic_"+fileending);

    //write_double_vector(Energy,string ("energy_temp.txt"));
    return 0;
} // MAIN END

void results_per_mc(double *quantity,int MC, string filename){
    cout << "Writing " << filename << endl;
    ofstream file(filename, ofstream::binary);
    file.write(reinterpret_cast<const char*> (quantity), MC*sizeof(double));
    file.close();
    cout << filename << " closed" << endl;
}

void metropolis(int Nspins, int MCs, double T,__1::mt19937_64 gen, imat &Lattice, vec &w, double &E, double &M){

    // Set up the uniform distribution for x \in [[0, 1]
    uniform_real_distribution<double> RNG(0.0,1.0);
    // Looping over the hole lattice
    for (int x = 0; x < Nspins; x++){
        for (int y = 0; y < Nspins; y++){
            int xi = int(RNG(gen)*Nspins);
            int yi = int(RNG(gen)*Nspins);
            // Energy from spinn (xi,yi)
            //cout << xi << " " << yi << endl;
            int dE =  (2*Lattice(yi,xi)*
                          (Lattice(yi,PeriodicBoundary(xi,Nspins,-1))+
                           Lattice(PeriodicBoundary(yi,Nspins,-1),xi) +
                           Lattice(yi,PeriodicBoundary(xi,Nspins,1)) +
                           Lattice(PeriodicBoundary(yi,Nspins,1),xi)));
            //cout << dE << endl;

            //cout << RNG(gen) << " " << w(dE+8) << endl;
            if (rand()/(RAND_MAX*1.0) <= w(dE+8)){ // Accept or not
                cout << "hei" << endl;
                Lattice(xi,yi) *= -1;   // flip to new accepted config
                M += 2*Lattice(xi,yi);
                E += dE;
            }
            //cout << E << endl;
        }   // END Y-DIRECTION
    }   // END X-DIRECTION, CONCLUDING LOP OVER LATTICE
}   // METROPOLIS END

void initialize_new_round(int Nspins, imat &Lattice, double &Energy, double &MagneticMom){
    Lattice.ones();
    MagneticMom = Lattice.size();

    Energy = 0;
    for (int i = 0; i< Nspins; i++){
        for (int j = 0; j< Nspins; j++){
            Energy -= double (Lattice(i,j)*( Lattice(PeriodicBoundary(i, Nspins, 1), j)) + Lattice(i, PeriodicBoundary(j, Nspins, 1)));
        }
    }
}   // INITIALIZE NEW ROUND END

void write_exp_values(vec const input, string filename, string path){
    ofstream ofile(path + filename);
    cout << "Writing file " << filename << endl;

    for (const auto &e : input) ofile << e << "\n";
    cout << "Written" << endl;

}   // WRITE DOUBLE VECTOR END


void EnergyCalculation(const mat &Lattice, double &E, int Nspins)
{
    //Calculate energy of whole lattice
    for (int i = 0; i<Nspins; i++){
        for (int j = 0; j<Nspins; j++){
            E -= double (Lattice(i,j)*( Lattice(PeriodicBoundary(i, Nspins, 1), j))
                         + Lattice(i, PeriodicBoundary(j, Nspins, 1)));
        }
    }
} // ENERGY CALCULATIONS END

void MetropolisAlgorithm(int Nspins, mat &Lattice, int MC, vector<double> &Energy, double T, vector<double> &MagneticMom)
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double>RandomGenerator(0.0, 1.0);
    double E0 = 0;
    double E;
    double w;
    double z;
    double delta;
    double TotalEnergy;
    int rejected = 0;
    int accepted = 0;


    EnergyCalculation(Lattice, E0, Nspins);
    TotalEnergy = E0;

    for (int i=0; i<MC; i++){
        //E0 = 0.0;
        E = 0.0;
        w = 0.0;
        z = 0.0;

        //EnergyCalculation(Lattice, E0, Nspins); //Calculate initial energy
        //TotalEnergy += E0;
        //cout << E0 << endl;

        //Choose random position in lattice and flip spin
        int x = int ((RandomGenerator(gen)*Nspins));
        int y = int ((RandomGenerator(gen)*Nspins));
        Lattice(x, y) *= -1;

        //cout << Lattice << endl;
        //Calculate new energy

        EnergyCalculation(Lattice, E, Nspins);
        delta = E-E0;
        if (delta <= 0.0){ //Keep energy if new one is lower
            TotalEnergy += E;
            Energy.push_back(double (E));
            MagneticMom.push_back(double (2*Lattice(x,y)));
            accepted++;
        }
        else{ //If energy is larger
            w = exp(-delta/T);
            if (w >= double (RandomGenerator(gen)) ){ //If true, flip spin back again
                rejected++;
                Lattice(x, y) *= -1;
            }
            else{ //Keep energy if not true
                accepted++;
                TotalEnergy += E;
                Energy.push_back( double (E0) );
                MagneticMom.push_back( double (2*Lattice(x,y)) );
                //SpecificHeat.push_back( double 1./T * ())

            }
        }
    }
    cout << TotalEnergy/MC << endl;
}   // METROPOLIS END

//USED FOR PRINTING TYPE VECTOR IF NEEDED
void print(std::vector<double> const &input) //Theft
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
    cout << endl;
}
