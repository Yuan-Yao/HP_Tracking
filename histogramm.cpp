#include "histogramm.h"

Histogramm::Histogramm(std::vector<Bunch> & bunch, Parameter & para):

  in(new double[para.histo_anz]),
  in_dis(new double[para.histo_anz]),
  out(new double[para.histo_anz]),
  out_dis(new double[para.histo_anz]),
  fft_plan(fftw_plan_r2r_1d(para.histo_anz,in,out,FFTW_R2HC,FFTW_ESTIMATE)),
  fft_plan_dis(fftw_plan_r2r_1d(para.histo_anz,in_dis,out_dis,FFTW_R2HC,FFTW_ESTIMATE)),
  distortion_power(bunch.size()*para.anz_Teilchen/para.histo_anz*para.noise_power/para.h), //Teilchen sind gleichverteilt und werden durch die Anzahl der Histogrammbalken geteilt
  histogramm_com(std::vector<double>(para.histo_anz*para.h)),
  histogramm_dis_com(std::vector<double>(para.histo_anz*para.h))
{
  qDebug() << "Anfang";
  histogramm.assign(para.h,std::vector<double>(para.histo_anz));
  histogramm_dis.assign(para.h,std::vector<double>(para.histo_anz));
  amplitude.assign(para.h,std::vector<double>(para.histo_anz));
  amplitude_dis.assign(para.h,std::vector<double>(para.histo_anz));
  phase.assign(para.h,std::vector<double>(para.histo_anz));
  phase_dis.assign(para.h,std::vector<double>(para.histo_anz));
  qDebug() << "Ende";
}

void Histogramm::fft(std::vector<double> & hist,double *in_local, double *out_local,fftw_plan fft_plan_local, std::vector<double> & amplitude_local, std::vector <double> & phase_local){
  for(unsigned int i = 0;i < hist.size(); i++){
    in_local[i] = hist[i];
  }
  fftw_execute(fft_plan_local);
  out_local[0] = out_local[0]/hist.size();  //normalise Fourier Trafos with histogramm size
  for(unsigned int i = 1; i< hist.size(); i++){
    out_local[i] = out_local[i]/(hist.size()/2);  //all coefficients beside 0 are to be halved because of symmetries of an real histogram
  }
  amplitude_local[0] = out_local[0]; phase_local[0] = 0;
  for(unsigned int i = 1; i<(hist.size()/2); i++){
    amplitude_local[i] = sqrt(pow(out_local[i],2)+pow(out_local[hist.size()-i],2)); // check with mathematica, if this is right
    phase_local[i] = atan(out_local[hist.size()-i]/out_local[i]);  //check with mathematica, if this is right
  }
}

void Histogramm::process_bunch(std::vector<Bunch> & bunch, Parameter & para){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<> ndis(0,1);

  //complete histgoram is divided into h single bucket histograms and fourier transformed
  for (int k = 0; k < para.h; k++){
    for (int i = 0; i < para.histo_anz; i++){
      histogramm[k][i] = 0;
      for (unsigned int j = 0; j < bunch.size(); j++){
        histogramm[k][i] += bunch[j].histogramm[k][i];
      }
      histogramm_dis[k][i] = histogramm[k][i] + distortion_power*(ndis(gen));
    }

    fft(histogramm[k],in,out,fft_plan,amplitude[k],phase[k]);
    fft(histogramm_dis[k],in_dis,out_dis,fft_plan_dis,amplitude_dis[k],phase_dis[k]);

  }
  //complete histograms for saving are created from single bucket histograms
  for(int k = 0; k < para.h; k++){
    for(int i = 0; i < para.histo_anz; i++){
      histogramm_com[i+k*para.histo_anz] = histogramm[k][i];
      histogramm_dis_com[i+k*para.histo_anz] = histogramm_dis[k][i];
    }
  }
}
