#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qvector.h>
#include <vector>
#include <parameter.h>
#include <referenz.h>
#include <log_bunch.h>
#include <qdebug.h>
#include <log_ref_i.h>
#include <chrono>
#include <ctime>
#include <own_math.h>
//#include <Eigen>
#include <iostream>

class Controller
{
  private:
    const int a;    //constant of DSP_Software_Implementation41
    const int s_F;  //constant of DSP_Software_Implementation41
    const int h;    //harmonic number

    const double increment_f_FIR;       //pass-through-frequency modifier
    const double fd_delay_Td;           //delay time of plant
    const double internal_time_count;   //sampling time

    double fd_fir_fpass;                //pass-through-frequency
    int fd_delay_kd;                    //delay zero tap shift for 1.st coefficient
    int fd_delay_fir1;                  //      zero tap shift for 2.sec coefficient
    int fd_delay_fir2;                  //      zero tap shift for 3.rd coefficient
    double gain_FIR;                    //gain-factor
    double gain_FIR_Liwei;              //gain-factor for 3-Tap Liwei Controller
    double offset;                      //time until enough values for filters are available; also suppress every turn calculation with experimental_type_filters
    std::vector<int> energy_sink_flag;  //energy_sink_controller_flag;

    std::vector<double> u;                            //control variable
    std::vector< std::vector<double> > fd_xfir;       //sum of product of coefficients and values
    std::vector< std::vector<double> > fd_xI;         //discrete integrator
    std::vector< std::vector<double> > Delta_phi_gap; //phi_shift control varialbe
    std::vector< std::vector<double> > fir_phi_det;   //Delta_phi_det = Delta_phi - Delta_phi_Gap (with feedthroug Samuel Braun Masterarbeit 2014)

    //Hamilton-DFT-phi_gap
    // continuous mean building
    std::vector< std::vector<double> > DH_phi_gap;          //Hamilton_DFT_Delta_phi_gap
    std::vector< std::vector<double> > DH_phi_gap_10;       //Hamilton_DFT_Delta_phi_gap_10
    std::vector< std::vector<double> > DH_energy;           // Hamilton_DFT_energy tracking vector
    std::vector< std::vector<double> > DH_energy_10;        // Hamilton_DFT_energy tracking compress;
    std::vector< std::vector<double> > DH_integrator;       // Integration over all DH_phi_gaps;
    std::vector< std::vector<double> > DH_integrator_energy;//Energy estimation via Integration;
    std::vector< std::vector<double> > DH_pos;              // Hamilton_pos
    std::vector< std::vector<double> > DH_vel;              // Hamilton_vel
    double DH_SD; //standard-deviation of measurements, when 100 measurements occured. Hopefully without further disturbances
    std::vector< int > DH_LDC; //counter for large deviations, minimum 11 times, because the deviation is 10 steps long;
    std::vector< double > DH_MD; //container for Maximum deviation during, when large deviations occur
    bool lock;
    int DH_lock_counter;

    // signal analyzing for signal drifts and small oscillation detection
    std::vector < std::vector< double > > DH_sin_part_16; //sin_partitions, circulation time dependant
    std::vector< double > DH_base; //actual base_value
    std::vector< double > DH_rad; //actual DH_rad
    std::vector< double > DH_phase; //actual DH_phase
    double DH_small_dev_start; //small deviation damping
    double DH_small_dev_end; //small deviation damping end

    // DH_timer
    int DH_mode; //DH_mode
    double DHt_start;
    double DHt_end;
    double DHt_ptime;
    double DHt_pnum;
    double DHt_count;
    //Hamilton-DFT-phi_gap_end


    //Hamilton-Integrator_phi_gap_start
      std::vector< std::vector <double> >DH2_phi_gap;
      std::vector< std::vector <double> >DH2_base;
      std::vector< std::vector <double> >DH2_u;
      std::vector< std::vector <double> >DH2_integrator;
      std::vector< std::vector <double> >DH2_energy;
      std::vector< std::vector <double> >DH2_phi_gap_10;

      std::vector<double> DH2_local_max;
      std::vector<double> DH2_local_min;
      std::vector<bool> DH2_damp;
      std::vector< std::vector<double> >DH2_local_div;
      std::vector<int> DH2_msl; //DH"_max_search_flag: 0 -> init; 1-> new maximum
      //, but search further new maxima; 2-> new minimum, but search further new minima;
      //
      double DH2_std_dev;
    //Hamilton-Integrator_phi_gap_end


    //declaration of private functions
    void no_controller (int i, Log_Bunch & log);  //dummy controller function
    void three_Tap_Filter(int i,Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //three-tap-filter with every step actualisation
    void three_Tap_Filter_Experiment(int i, Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //three-tap-filter with every 3.22µs actualisation
    void three_Tap_Filter_Liwei(int i, Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //three-tap-filter with every 3.22µs actualisation on basis of Masterthesis Liwei Shen
    void Zustandsregler(int i, Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //Zustandsregler, state-feedback-controller
    void Energy_Sink(int i, Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //Energy_Sink_Controller, with respect to absolute phi_value
    void Hamilton_Damp(int i, Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //Hamilton_Damp_Controller with respect to Delta_phi_Gap
    void Hamilton_Damp_2(int i, Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //Hamilton_Damp_Controller with respect to Delta_phi_Gap, only take integrated values;

    std::chrono::time_point<std::chrono::system_clock>start,end;

  public:
    Controller(Parameter & para, log_ref_i & log_ref); //constructor

    void operate(int i, Log_Bunch & log, log_ref_i & log_ref, Parameter & para); //function calling correct controller
    QVector<double> get_Delta_phi_gap(int j){return QVector<double>::fromStdVector(Delta_phi_gap[j]);}
    QVector<double> get_DH_phi_gap(int j){return QVector<double>::fromStdVector(DH_phi_gap[j]);}
    QVector<double> get_DH_phi_gap_10(int j){return QVector<double>::fromStdVector(DH_phi_gap_10[j]);}
    QVector<double> get_DH_energy(int j){return QVector<double>::fromStdVector(DH_energy[j]);}
    QVector<double> get_DH_energy_10(int j){return QVector<double>::fromStdVector(DH_energy_10[j]);}
    QVector<double> get_DH_integrator(int j){return QVector<double>::fromStdVector(DH_integrator[j]);}
    QVector<double> get_DH_integrator_energy(int j){return QVector<double>::fromStdVector(DH_integrator_energy[j]);}
    QVector<double> get_DH_pos(int j){return QVector<double>::fromStdVector(DH_pos[j]);}
    QVector<double> get_DH_vel(int j){return QVector<double>::fromStdVector(DH_vel[j]);}
    QVector<double> get_fd_xI(int j){return QVector<double>::fromStdVector(fd_xI[j]);}
    std::vector<double> get_u(){return u;}
    QVector<double> get_u_print(){return QVector<double>::fromStdVector(u);}

    QVector<double> get_DH2_phi_gap(int j){return QVector<double>::fromStdVector(DH2_phi_gap[j]);}
    QVector<double> get_DH2_integrator(int j){return QVector<double>::fromStdVector(DH2_integrator[j]);}
    QVector<double> get_DH2_phi_gap_10(int j){return QVector<double>::fromStdVector(DH2_phi_gap_10[j]);}
    QVector<double> get_DH2_local_div(int j){return QVector<double>::fromStdVector(DH2_local_div[j]);}
    QVector<double> get_DH2_base(int j){return QVector<double>::fromStdVector(DH2_base[j]);}
    QVector<double> get_DH2_u(int j){return QVector<double>::fromStdVector(DH2_u[j]);}


};
#endif // CONTROLLER_H
