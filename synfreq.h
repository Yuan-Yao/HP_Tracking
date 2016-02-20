#ifndef SYNFREQ_H
#define SYNFREQ_H

#include "qdebug.h"
#include <vector>
#include <parameter.h>
#include "referenz.h"
#include <math.h>
#include <tuple>

class SynFreq
{
  private:
    const int h;
    const double pi;
    const double q;
    bool is_init;

    double omega_R;
    double eta_R;
    double beta_R;
    double W_R;
    double T;

    double u_1R;
    double u_2R;
    double phi_R;
    double phi_dh2;

    double DEo1h;
    double DEo2h;
    double phia;
    double max_energy;

    std::vector<double>DEo_LUT;   //lookup-table for energy deviation
    std::vector<double>syn_freq;  //lookup-table for energy deviation dependant synchrotron frequency
    std::vector<double>syn_T;     //lookup-table for energy deviation dependant synchrotron period time

    void init(Referenz & ref, double max_enery);  //initialize lookup-tables for further work
  public:
    SynFreq(Parameter & para, Referenz & ref, double max_energy); //constructor
    std::tuple<double,double> init_pos (double energy,double frac); //particle on trajectory; time-dependant
};

#endif // SYNFREQ_H
