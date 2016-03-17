#ifndef LOG_BUNCH_H
#define LOG_BUNCH_H
#include "bunch.h"
#include <vector>
#include "qvector.h"
#include "parameter.h"
#include "qdebug.h"
#include "referenz.h"
#include "seperatrix.h"
#include "histogramm.h"

class Log_Bunch
{
  private:
    const double pi; //Pi
    const int h; //harmonic number
    std::vector< std::vector<double> > log_bunch_phi; //phase deviation of all particles
    std::vector< std::vector<double> > log_bunch_DEo; //energy deviation of all particles
    std::vector< std::vector<double> > log_bunch_sep_pos; //actual seperatrix in positive regime
    std::vector< std::vector<double> > log_bunch_sep_neg; //actual seperatrix in negative regime
    std::vector< std::vector<double> > log_bunch_voltage; //control-deviated voltage
    std::vector<double> lost_particles; //actually lost particles

    std::vector< std::vector<double> > log_bunch_moment_phi_1; //first order moment phi seperate for every bunch
    std::vector< std::vector<double> > log_bunch_moment_phi_2; //second order moment phi seperate for every bunch
    std::vector< std::vector<double> > log_bunch_moment_DEo_1; //first order moment energy deviation seperate for every bunch
    std::vector< std::vector<double> > log_bunch_moment_DEo_2; //second order moment energy deviation seperate for every bunch
    std::vector< std::vector<double> > log_bunch_moment_phi_1_DEo_1; //cross-correlation between phi and energy deviation
    std::vector< std::vector<double> > emittance; //emittance; occupation in phase space seperated by bunches
    std::vector<double> emittance_sum; //sum of all single bucket emittance
    std::vector< std::vector<double> > log_histogramm; //complete histogram phi synchrotron
    std::vector< std::vector<double> > log_histogramm_dis; //complete distorted histogram phi of synchrotron
    std::vector< std::vector<double> >fft_phase_0; //Fourier-Transformation phase deviation by bucket
    std::vector< std::vector<double> >fft_amp_0; //Forier-Transformation energy deviation by bucket
    std::vector< std::vector<double> >fft_phase_dis_0; //Fourier-Transformation distorted phase deviation by bucket
    std::vector< std::vector<double> >fft_amp_dis_0; //Fourier-Transformation distorted energy deviation by bucket

  public:
    Log_Bunch(Parameter &,Seperatrix &,int save_slots, int thread_count); //constructor
    int return_size(){return log_bunch_DEo.size();} //number of particles
    void save_bunch(std::vector<Bunch> &,Histogramm &, Seperatrix &, int); //save current status of bunch position in phase space, seperatrix and histogram
    void save_moments_1(std::vector<Bunch> &,int); //combine first order moments of each bunch thread bucketwise
    void save_histogramm(Histogramm & hist, int); //save complete disturbed and undisturbed histograms
    void save_moments_2(std::vector<Bunch> &,Histogramm &, bool coasting,int); //combine second order moments of each bunch thread bucketwise and emittance
    QVector<double> get_log_bunch_phi(int i){return QVector<double>::fromStdVector (log_bunch_phi[i]);}
    QVector<double> get_log_bunch_DEo(int i){return QVector<double>::fromStdVector (log_bunch_DEo[i]);}
    QVector<double> get_log_sep_pos(int i){return QVector<double>::fromStdVector (log_bunch_sep_pos[i]);}
    QVector<double> get_log_sep_neg(int i){return QVector<double>::fromStdVector (log_bunch_sep_neg[i]);}
    QVector<double> get_log_voltage(int i){return QVector<double>::fromStdVector(log_bunch_voltage[i]);}
    QVector<double> get_lost_particles(){return QVector<double>::fromStdVector(lost_particles);}
    QVector<double> get_log_histogramm(int i){return QVector<double>::fromStdVector(log_histogramm[i]);}
    QVector<double> get_log_histogramm_dis(int i){return QVector<double>::fromStdVector(log_histogramm_dis[i]);}
    QVector<double> get_log_fft_phase_0(int j){return QVector<double>::fromStdVector(fft_phase_0[j]);}
    QVector<double> get_log_fft_amp_0(int j){return QVector<double>::fromStdVector(fft_amp_0[j]);}
    QVector<double> get_log_fft_phase_dis_0(int j){return QVector<double>::fromStdVector(fft_phase_dis_0[j]);}
    QVector<double> get_log_fft_amp_dis_0(int j){return QVector<double>::fromStdVector(fft_amp_dis_0[j]);}
    QVector<double> get_emittance(int j){return QVector<double>::fromStdVector(emittance[j]);}
    QVector<double> get_emittance_sum(){return QVector<double>::fromStdVector(emittance_sum);}
    QVector<double> get_moment_DEo_1(int j){return QVector<double>::fromStdVector(log_bunch_moment_DEo_1[j]);}
    QVector<double> get_moment_phi_1(int j){return QVector<double>::fromStdVector(log_bunch_moment_phi_1[j]);}
    QVector<double> get_moment_DEo_2(int j){return QVector<double>::fromStdVector(log_bunch_moment_DEo_2[j]);}
    QVector<double> get_moment_phi_2(int j){return QVector<double>::fromStdVector(log_bunch_moment_phi_2[j]);}
    QVector<double> get_moment_phi_1_DEo_1(int j){return QVector<double>::fromStdVector(log_bunch_moment_phi_1_DEo_1[j]);}

    //fast data_read for controller
    double get_log_fft_phase_0_control(int k, int i){return fft_phase_0[k][i];}
    double get_Deo_control(int k, int i){return log_bunch_moment_DEo_1[k][i];}
    double get_log_fft_phase_dis_0_control(int k, int i){return fft_phase_dis_0[k][i];}

};

#endif // LOG_BUNCH_H
