#ifndef HISTOGRAMM_H
#define HISTOGRAMM_H

#include <vector>
#include <random>
#include "bunch.h"
#include "parameter.h"
#include "fftw3.h"

class Log_Bunch;

class Histogramm
{
  private:
    double * in; //set of variables for fourier-trafo fftw
    double * in_dis;
    double * out;
    double * out_dis;
    fftw_plan fft_plan;
    fftw_plan fft_plan_dis; //end set of variables for fourier-trafo fftw

    double distortion_power; //additional noise to histogram; bias of histogram if all particles are equally distributed
    std::vector< std::vector<double> >histogramm; //histogram
    std::vector< std::vector<double> >histogramm_dis; //histogram with additional noise
    std::vector< std::vector<double> >amplitude; //absolute values of amplitude out of fourier trafo of histogram
    std::vector< std::vector<double> >amplitude_dis; //absolute values of amplitude of fourier trafo of distorted histogram
    std::vector< std::vector<double> >phase; //absolute values of phase out of fourier trafo of histogram
    std::vector< std::vector<double> >phase_dis; //absolute values of amplitude out of fourier trafo of distorted histogram
    std::vector<double> histogramm_com; //complete histogram of all buckets
    std::vector<double> histogramm_dis_com; //complete histogram of all buckets with additive distortions
  public:
    Histogramm(std::vector<Bunch> & bunch, Parameter & para);
    void fft(std::vector<double> & hist,double * in, double * out, fftw_plan,std::vector<double> & amplitude, std::vector<double> & phase); //transform histogram to phase and ampitude information to be calculated further on
    void process_bunch (std::vector<Bunch> & bunch, Parameter & para); //create single bucket histograms and extract phase and amplitude information

    friend class Log_Bunch;
};

#endif // HISTOGRAMM_H
