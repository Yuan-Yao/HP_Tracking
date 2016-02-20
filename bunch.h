#ifndef BUNCH_H
#define BUNCH_H
#include <vector>
#include "parameter.h"
#include <random>
#include "qdebug.h"
#include "referenz.h"
#include "math.h"
#include "fftw3.h"
#include "seperatrix.h"
#include "synfreq.h"
#include <chrono>
#include <log_reference.h>
#include <log_ref_i.h>

class Log_Bunch;
class Histogramm;

class Bunch
{
  private:
    std::vector<double> phi;                  //Phase-difference without feedthrough
    std::vector<double> DEo;                  //DEo is  Delta_E * q / omega_RF; conjugated energy deviation from phi
    std::vector<int> lost;                    //determines whether a particle is annihilated due to a collision with the beam pipe
    double lost_count;                        //how many particles lost already
    double lost_threshold;                    //threshold when particle should be count as lost
    double reduction;                         //reduction if smaller seperatrix is needed
    const int h;                              //harmonic number

    std::vector<double> mp1;                  //first moment phi
    std::vector<double> md1;                  //first moment DEo
    std::vector<QVector<double>> mp1_tracker;     //Error-Tracker-Vector
    std::vector<QVector<double>> md1_tracker;     //Error-Tracker-Vector
    std::vector<int> particle_count;          //particle count per bucket
    std::vector<double> histogramm_complete;  //histogramm with all buckets
    std::vector< std::vector<double> >histogramm; //histogram for each bucket seperatly
    std::vector<double> histogramm_complete_energy;  //energy_deviation_histogramm with all buckets
    std::vector<std::vector<double> > histogramm_energy; //histogramm_energy for each bucket seperatly
    std::vector<double> histogramm_hamilton_energy_complete; //histogramm_hamilton_energy with all buckets
    std::vector<std::vector<double> > histogramm_hamilton_energy; //histogramm_hamilton_energy for each bucket seperatly

    std::vector<double> mp2;                  //second moment phi
    std::vector<double> md2;                  //second moment DEo
    std::vector<double> mp1d1;                //correlation between first moment phi and first moment DEo
    std::vector<int> particle_in_bunch;       //determines in which a particle lies in
    void first_order_moment_corrector();      //Error-Tracker-Function


  public:
    Bunch(Parameter &);                       //constructor
    void moment_actualisation(Parameter &);   //first order moment and histogramm calculations
    void moment_actualisation_2(Parameter &); //second order moment calculations
    void init_bunch(Parameter &, Seperatrix &,Referenz & ref); //init bunch
    std::vector<double> get_bunch_phi(){return phi;}
    std::vector<double> get_bunch_DEo(){return DEo;}
    std::vector<std::vector<double>> get_histogramm(){return histogramm;}
    std::vector<double> get_bunch_mp1(){return mp1;}
    void tracking(Parameter &, log_ref_i &,std::vector<double> u); //particle tracking
    void phi_manip(std::vector<double> distortion);   //distortion moves bunch with respect to Ref-DDS

    friend class Log_Bunch;
    friend class Histogramm;
};

#endif // BUNCH_H
