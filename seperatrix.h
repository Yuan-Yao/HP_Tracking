#ifndef SEPERATRIX_H
#define SEPERATRIX_H

#include "parameter.h"
#include "QVector"
#include <tuple>
#include <qdebug.h>
#include <QString>
#include <log_ref_i.h>

class Log_Reference;

class Seperatrix
{
  private:
    //Reference_values
    double u_1R;
    double UFP;
    double u_R;
    double beta_R;
    double w_R;
    double eta_R;
    double omega_R;
    double phi_R;
    double phi_dh2;
    double u_2R;

    //Parameter_Values
    const double q;
    const double pi;
    const int h;
    const int h_pow;
    const QString spannungstyp;
    const double u_1R_base;
    const double u_2R_base;


    //diverse interne Variablen
    double var; //variable Bucketgröße
    std::vector<double> phi_ring_sequence;
    std::vector<double> seperatrix_pos;
    std::vector<double> seperatrix_neg;
    std::vector<double> seperatrix_turbo;
    std::vector<double> seperatrix_08;
    std::vector<double> seperatrix_06;
    std::vector<double> seperatrix_04;
    std::vector<double> seperatrix_02;
    std::vector<double> seperatrix_var;
    std::vector<double> stat_seperatrix_pos;
    std::vector<double> stat_seperatrix_neg;
    std::vector<double> voltage;
    QVector<double> sep_difference;

    //diverse Seperatrix Turbo Größen
    std::vector<double> phi_ring_sequence_turbo;
    double turbo_front;
    double turbo_back;

    double bucket;
    double stat_bucket;
    double stat_sepfactor;
    double stat_max_sep;

    void set_var_seperatrix(std::vector<double> & sep,double reduk);
    //sums up seperatrix values with triangle approximation
    double sum_seperatrix(std::vector<double> & sep);

  public:
    Seperatrix(Parameter & para, int standard_length, int turbo_length);
    void get_from_ref(std::tuple<double,double,double,double,double,double,double,double,double,double>);
    void get_from_lri(log_ref_i & log_ref);
    double get_u_1R(){return u_1R;}
    void set_all_small_seperatrix();
    void set_static_seperatrix();
    void set_full_length_dynamic_sep(double reduk);
    void set_turbo_length_dynamic_sep();
    void set_voltage(std::vector<double> u);
    double get_sepfactor_stat(){return stat_sepfactor;}
    double get_stat_max_sep(){return stat_max_sep;}
    std::vector<double> get_sep_pos(){return seperatrix_pos;}
    std::vector<double> get_sep_neg(){return seperatrix_neg;}
    std::vector<double> get_voltage(){return voltage;}
    QVector<double> get_sep_pos_static(){return QVector<double>::fromStdVector(stat_seperatrix_pos);}
    QVector<double> get_sep_neg_static(){return QVector<double>::fromStdVector(stat_seperatrix_neg);}
    QVector<double> get_phi_ring_sequence(){return QVector<double>::fromStdVector(phi_ring_sequence);}
    friend class Log_Reference;
    void integrator_debug();
    QVector<double> get_sep_difference(QVector<double> input_vector);
};

#endif // SEPERATRIX_H
