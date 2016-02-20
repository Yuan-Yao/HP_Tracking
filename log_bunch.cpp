#include "log_bunch.h"

Log_Bunch::Log_Bunch(Parameter & para,Seperatrix & sep, int saveslots, int thread_count):
  pi(para.pi),
  h(para.h),
  lost_particles(std::vector<double>(para.anz_Umdrehungen+1)),
  emittance_com(std::vector<double> (para.anz_Umdrehungen+1))
{
  log_bunch_phi.assign(saveslots+1,std::vector<double>(para.anz_Teilchen*thread_count));
  log_bunch_DEo.assign(saveslots+1,std::vector<double>(para.anz_Teilchen*thread_count));
  log_bunch_sep_pos.assign(saveslots+1,std::vector<double>(sep.get_phi_ring_sequence().size()));
  log_bunch_sep_neg.assign(saveslots+1,std::vector<double>(sep.get_phi_ring_sequence().size()));
  log_bunch_voltage.assign(saveslots+1,std::vector<double>(sep.get_phi_ring_sequence().size()));

  log_bunch_moment_phi_1.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  log_bunch_moment_phi_2.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  log_bunch_moment_DEo_1.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  log_bunch_moment_DEo_2.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  log_bunch_moment_phi_1_DEo_1.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  emittance.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  log_histogramm.assign(saveslots+1,std::vector<double>(para.histo_anz*para.h));
  log_histogramm_dis.assign(saveslots+1,std::vector<double>(para.histo_anz*para.h));
  fft_phase_0.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  fft_amp_0.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  fft_phase_dis_0.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
  fft_amp_dis_0.assign(para.h,std::vector<double>(para.anz_Umdrehungen+1));
}

//save complete histograms, not every turn, because of limited amount of RAM
void Log_Bunch::save_histogramm(Histogramm & hist, int i){
  log_histogramm[i] = hist.histogramm_com;
  log_histogramm_dis[i] = hist.histogramm_dis_com;
}

//save complete bunch phasespacedata, seperatrix and histogramm
void Log_Bunch::save_bunch(std::vector<Bunch> & bunch,Histogramm & hist, Seperatrix & sep, int i){
  for (unsigned int j = 0 ; j < bunch.size(); j++){
    int size = bunch[0].phi.size();
      std::copy(bunch[j].phi.begin(),bunch[j].phi.end(),log_bunch_phi[i].begin()+j*size);
      std::copy(bunch[j].DEo.begin(),bunch[j].DEo.end(),log_bunch_DEo[i].begin()+j*size);
    }
    log_bunch_sep_pos[i] = sep.get_sep_pos();
    log_bunch_sep_neg[i] = sep.get_sep_neg();
    log_bunch_voltage[i] = sep.get_voltage();
    save_histogramm(hist,i);
}

//sum and normalise first order moments of each thread an save them
void Log_Bunch::save_moments_1(std::vector<Bunch> & bunch, int i){

  for (int k = 0; k < h;k++){
    log_bunch_moment_phi_1[k][i] = 0.0;
    log_bunch_moment_DEo_1[k][i] = 0.0;
  }
  double lost = 0.0;
  for (unsigned int j = 0; j< bunch.size(); j++){
    for (int k = 0; k< h;k++){
      log_bunch_moment_phi_1[k][i] += bunch[j].mp1[k];
      log_bunch_moment_DEo_1[k][i]  += bunch[j].md1[k];
      lost += bunch[j].particle_count[k];
    }
  }
  for(int k = 0; k<h;k++){
    log_bunch_moment_phi_1[k][i] = log_bunch_moment_phi_1[k][i] / bunch.size();
    log_bunch_moment_DEo_1[k][i] = log_bunch_moment_DEo_1[k][i] / bunch.size();
  }
  for (unsigned int j = 0; j< bunch.size();j++){
    for(int k = 0; k<h;k++){
      bunch[j].mp1[k] = log_bunch_moment_phi_1[k][i];
      bunch[j].md1[k] = log_bunch_moment_DEo_1[k][i];
    }
  }
}

//combine and normalise second order moments, emittance and lost particles of each thread and save them
void Log_Bunch::save_moments_2(std::vector<Bunch> & bunch,Histogramm & hist,bool coasting, int i){
  emittance_com[i] = 0;
  lost_particles[i] = 0;
  for(int k = 0; k < h; k++){
    log_bunch_moment_phi_2[k][i] = 0.0;
    log_bunch_moment_phi_1_DEo_1[k][i] = 0.0;
    log_bunch_moment_DEo_2[k][i] = 0.0;
  }
  for(int k = 0; k < h; k++){
    for (unsigned int j = 0; j< bunch.size(); j++){
      log_bunch_moment_phi_2[k][i] += bunch[j].mp2[k];
      log_bunch_moment_phi_1_DEo_1[k][i] += bunch[j].mp1d1[k];
      log_bunch_moment_DEo_2[k][i] += bunch[j].md2[k];
    }
  }
  for (int k = 0; k<h;k++){
  log_bunch_moment_DEo_2[k][i] = log_bunch_moment_DEo_2[k][i] / bunch.size();
  log_bunch_moment_phi_2[k][i] = log_bunch_moment_phi_2[k][i] / bunch.size();
  log_bunch_moment_phi_1_DEo_1[k][i] = log_bunch_moment_phi_1_DEo_1[k][i] / bunch.size();
  emittance[k][i] = sqrt(log_bunch_moment_phi_2[k][i]*log_bunch_moment_DEo_2[k][i]-pow(log_bunch_moment_phi_1_DEo_1[k][i],2));
  emittance_com[i] += emittance[k][i]/h;
  }
  for (unsigned int j = 0; j<bunch.size(); j++){
    lost_particles[i] += bunch[j].lost_count;
  }

  //if no coasting, then fourier trafo shift has to be added.
  for(int k = 0; k<h; k++){
    if (coasting == false){
      //exact convention has to be evaluatet !!! still to be done, Phase-Shift inverted
      int div = hist.histogramm[0].size();
      fft_phase_0[k][i]= -(hist.phase[k][1]-pi/div);
      fft_amp_0[k][i]= -(hist.amplitude[k][1]-pi/div);
      fft_phase_dis_0[k][i]= -(hist.phase_dis[k][1]-pi/div);
      fft_amp_dis_0[k][i]= -(hist.amplitude_dis[k][1]-pi/div);
    }
    else{
      fft_phase_0[k][i]=0.0;
      fft_amp_0[k][i]=0.0;
      fft_phase_dis_0[k][i]=0.0;
      fft_amp_dis_0[k][i]=0.0;
    }
  }
}
