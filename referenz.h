#ifndef REFERENZ_H
#define REFERENZ_H
#include "math.h"
#include "parameter.h"
#include "qdebug.h"
#include "qvector.h"
#include <vector>
#include <chrono>
#include <tuple>
#include <seperatrix.h>

class Log_Reference;
class Bunch;

class Referenz
{
  private:
    double w_Kin;
    double w_Kin_R;
    double w_R;     //Energy E_R
    double gamma_R;
    double beta_R;
    double u_R;
    double u_1R;
    double phi_dh2;
    double u_2R;
    double f_R;
    double f_RF;
    double omega_R;
    double eta_R;
    double omega_RF;
    double T_R;
    double t_R;
    double phi_R;
    double p_R;
    double B;
    double B2;
    double BDot;
    int particle_Lost;
    double f_syn;
    double omega_syn;
    double Q_syn;
    float bucket;
    float stat_bucket;
    double stat_beta_R;
    double stat_gamma_R;
    double stat_w_R;
    double stat_u_1R;
    double stat_eta_R;
    double stat_omega_R;
    bool stat_save;
    bool coasting;
    double UFP;

  public:
    void rampenevent(Parameter &);

    Referenz();
    void init_ref(Parameter &);

    void tracking(Parameter &, Seperatrix &);
    void ausgabe_debug();
    double get_ref_voltage(){return u_1R;}
    double get_ref_UFP(){return UFP;}
    double get_t(){return t_R;}
    double get_T(){return T_R;}
    double get_f_syn(){return f_syn;}
    double get_f_R(){return f_R;}
    //Tupel mit: 1:u_1R,2:UFP,3:u_R,4:beta_R,5:w_R,6:eta_R,7:omega_R,8:phi_R,9:phi_dh_2,10:u_2R;
    std::tuple<double,double,double,double,double,double,double,double,double,double> ref_to_seperatrix(){return std::tuple<double,double,double,double,double,double,double,double,double,double>(u_1R,UFP,u_R,beta_R,w_R,eta_R,omega_R,phi_R,phi_dh2,u_2R);}

    friend class Log_Reference;
    friend class Bunch;
    friend class SynFreq;
};

#endif // REFERENZ_H
