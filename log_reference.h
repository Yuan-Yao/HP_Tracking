#ifndef LOG_REFERENCE_H
#define LOG_REFERENCE_H
#include "referenz.h"
#include "qdebug.h"
#include "qvector.h"
#include <vector>
#include "math.h"
#include "log_ref_i.h"

class Log_Reference
{
  private:
    std::vector<double> t_R; //complete laboratory time
    std::vector<double> T_R; //time per turn[s]
    std::vector<double> u_R; //reference voltage
    std::vector<double> u_1R; //single harmonic cavityvoltage
    std::vector<double> u_2R; //double harmonic cavity voltage
    std::vector<double> phi_1R; //reference phase deviation compared to gap-voltage
    std::vector<double> phi_2R; //phase between single harmonic and double harmonic cavity
    std::vector<double> f_R; //frequency of reference particle
    std::vector<double> omega_R; //angular frequency
    std::vector<double> f_syn; //synchrotron frequency
    std::vector<double> omega_syn; //synchrotron angular frequency
    std::vector<double> w_R; // E / omega_RF
    std::vector<double> BDot; //change of magnetic field
    std::vector<double> B; //magnetic field
    std::vector<double> beta_R; //beta of reference particle
    std::vector<double> eta_R; //eta of reference particle
    std::vector<double> bucket; //bucket size
    std::vector<double> UFP; //unstable phase space turning point
    std::vector<double> phi_ring_sequence; //ring divided in equidistant space fragments

  public:
    Log_Reference(int); //constructor
    void save_stat(int, Referenz &, Seperatrix & sep); //save all reference variables
    void show_stat(int); //show all values of int state
    QVector<double> get_t_R() {return QVector<double>::fromStdVector(t_R);}
    QVector<double> get_T_R() {return QVector<double>::fromStdVector(T_R);}
    QVector<double> get_u_R() {return QVector<double>::fromStdVector(u_R);}
    QVector<double> get_u_1R() {return QVector<double>::fromStdVector(u_1R);}
    QVector<double> get_u_2R() {return QVector<double>::fromStdVector(u_2R);}
    QVector<double> get_phi_1R() {return QVector<double>::fromStdVector(phi_1R);}
    QVector<double> get_phi_2R() {return QVector<double>::fromStdVector(phi_2R);}
    QVector<double> get_f_R() {return QVector<double>::fromStdVector(f_R);}
    QVector<double> get_omega_R() {return QVector<double>::fromStdVector(omega_R);}
    QVector<double> get_f_syn() {return QVector<double>::fromStdVector(f_syn);}
    QVector<double> get_omega_syn() {return QVector<double>::fromStdVector(omega_syn);}
    QVector<double> get_w_R() {return QVector<double>::fromStdVector(w_R);}
    QVector<double> get_BDot() {return QVector<double>::fromStdVector(BDot);}
    QVector<double> get_B() {return QVector<double>::fromStdVector(B);}
    QVector<double> get_beta_R() {return QVector<double>::fromStdVector(beta_R);}
    QVector<double> get_eta_R() {return QVector<double>::fromStdVector(eta_R);}
    QVector<double> get_phi_ring_sequence() {return QVector<double>::fromStdVector(phi_ring_sequence);}
    QVector<double> get_bucket(){return QVector<double>::fromStdVector(bucket);}
    QVector<double> get_UFP(){return QVector<double>::fromStdVector(UFP);}
    void set_log_ref_i(log_ref_i &, int i);
};



#endif // LOG_REFERENCE_H
