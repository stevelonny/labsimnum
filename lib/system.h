/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __System__
#define __System__

#include <version_config.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <armadillo>
#include <stdlib.h> //exit
#include "particle.h"
#include "random.h"

using namespace std;
using namespace arma;

struct system_input
{
  /* data */
  int sim_type;
  double _J;
  double _H;
  int restart;
  double temp;
  int npart;
  double rho;
  double r_cut;
  double delta;
  int nblocks;
  int nsteps;
  system_input() : sim_type{0}, restart{0}, temp{1.1}, npart{108}, rho{0.8}, r_cut{2.5}, delta{0.0005}, nblocks{100}, nsteps{200} {}
};

struct system_properties
{
  /* data */
  bool measure_penergy;
  bool measure_kenergy;
  bool measure_tenergy;
  bool measure_temp;
  bool measure_pressure;
  bool measure_gofr;
  bool measure_magnet;
  bool measure_cv;
  bool measure_chi;
  int n_bins;
  bool writeout;
  //ouch
  system_properties() : measure_penergy(false), measure_kenergy(false), measure_tenergy(false), measure_temp(false), measure_pressure(false), measure_gofr(false), measure_magnet(false), measure_cv(false), measure_chi(false), n_bins{100}, writeout{false} {}
};

class System {

private:
  const int _ndim = 3;  // Dimensionality of the system
  bool _restart;        // Flag indicating if the simulation is restarted
  int _sim_type;        // Type of simulation (e.g., Lennard-Jones, Ising)
  int _npart;           // Number of particles
  int _nblocks;         // Number of blocks for block averaging
  int _nsteps;          // Number of simulation steps in each block
  int _nattempts;       // Number of attempted moves
  int _naccepted;       // Number of accepted moves
  double _temp, _beta;  // Temperature and inverse temperature
  double _rho, _volume; // Density and volume of the system
  double _r_cut;        // Cutoff radius for pair interactions
  double _delta;        // Displacement step for particle moves
  double _J, _H;        // Parameters for the Ising Hamiltonian
  vec _side;            // Box dimensions
  vec _halfside;        // Half of box dimensions
  Random _rnd;          // Random number generator
  field <Particle> _particle; // Field of particle objects representing the system
  vec _fx, _fy, _fz;    // Forces on particles along x, y, and z directions

  // Properties
  int _nprop; // Number of properties being measured
  bool _measure_penergy, _measure_kenergy, _measure_tenergy;// Flags for measuring different energies
  bool _measure_temp, _measure_pressure, _measure_gofr;     // Flags for measuring temperature, pressure, and radial dist. function
  bool _measure_magnet, _measure_cv, _measure_chi;          // Flags for measuring magnetization, heat capacity, and susceptibility
  int _index_penergy, _index_kenergy, _index_tenergy;       // Indices for accessing energy-related properties in vec _measurement
  int _index_temp, _index_pressure, _index_gofr;            // Indices for accessing temperature, pressure, and radial dist. function
  int _index_magnet, _index_cv, _index_chi;                 // Indices for accessing magnetization, heat capacity, and susceptibility
  int _n_bins;           // Number of bins for radial distribution function
  double _bin_size;      // Size of bins for radial distribution function
  double _vtail, _ptail; // Tail corrections for energy and pressure
  vec _block_av;         // Block averages of properties
  vec _global_av;        // Global averages of properties
  vec _global_av2;       // Squared global averages of properties
  vec _average;          // Average values of properties
  vec _measurement;      // Measured values of properties

  // Paths
  std::filesystem::path _path_input;    // Path to input file
  std::filesystem::path _path_output;   // Path to output file

public: // Function declarations

  // Constructors
  System();                // Default constructor
  System(std::filesystem::path input_path, std::filesystem::path output_path);// Constructor with input/output paths
  ~System();               // Destructor

  int get_nbl();              // Get the number of blocks
  int get_nsteps();           // Get the number of steps in each block
  void initialize();          // Initialize system properties
  void initialize(system_input input); // Initialize system properties from struct systemt_input
  void initialize_properties();// Initialize properties for measurement
  void initialize_properties(system_properties properties); // Initialize properties for measurement from struct system_properties
  void finalize();            // Finalize system and clean up
  void write_configuration(); // Write final system configuration to XYZ file
  void write_XYZ(int nconf);  // Write system configuration in XYZ format on the fly
  void write_velocities();    // Write final particle velocities to file
  void read_configuration();  // Read system configuration from file
  void initialize_velocities();// Initialize particle velocities
  void step();                // Perform a simulation step
  void block_reset(int blk);  // Reset block averages
  void measure();             // Measure properties of the system
  void averages(int blk);     // Compute averages of properties
  void averages(bool nofile); // Compute averages of properties without writing to file
  double error(double acc, double acc2, int blk); // Compute error
  void move(int part);        // Move a particle
  bool metro(int part);       // Perform Metropolis acceptance-rejection step
  double pbc(double position, int i); // Apply periodic boundary conditions for coordinates
  int pbc(int i);             // Apply periodic boundary conditions for spins
  void Verlet();              // Perform Verlet integration step
  double Force(int i, int dim); // Calculate force on a particle along a dimension
  double Boltzmann(int i, bool xnew); // Calculate Boltzmann factor for Metropolis acceptance


  const int get_index_temp() {return _index_temp;}                      // Get index for temperature
  const int get_index_tenergy() {return _index_tenergy;}                // Get index for total energy
  const int get_index_cv() {return _index_cv;}                          // Get index for heat capacity
  const int get_index_chi() {return _index_chi;}                        // Get index for susceptibility
  const int get_index_magnet() {return _index_magnet;}                  // Get index for magnetization
  const double get_average(int index) {return _average(index);}         // Get average value of a property
  const double get_sumaverage(int index) {return _global_av(index);}    // Get sum of averages of a property
  const double get_sum2average(int index) {return _global_av2(index);}  // Get sum of squared averages of a property

};

#endif // __System__

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
