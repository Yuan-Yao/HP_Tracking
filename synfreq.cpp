#include "synfreq.h"

SynFreq::SynFreq(Parameter & para, Referenz & ref, double max_energy_):
  h(para.h),
  pi(para.pi),
  q(para.q),
  is_init(false),

  DEo_LUT(std::vector<double> (512)),
  syn_freq(std::vector<double> (512)),
  syn_T(std::vector<double>(512))
{
  if (para.coasting == false) init(ref,max_energy_);
  qDebug() << "init done";
}

struct syn_freq_dh
{
    double energy,T_in,DEo1h_in,DEo2h_in,phia_in;

    double time;
    double phi;
    syn_freq_dh(double T_in_, double DEo1h_in_, double DEo2h_in_, double phia_in_):T_in(T_in_),DEo1h_in(DEo1h_in_),DEo2h_in(DEo2h_in_),phia_in(phia_in_){}
    double operator() (double energy_){
      energy = energy_;
      time = 0;
      phi = 0;
      while(energy >= 0){
        energy = energy + DEo1h_in*sin(phi)+DEo2h_in*sin(2*phi);
        phi = phi+phia_in*energy;
        time = time+T_in;
      }
      return time*4;
    }
};

void SynFreq::init(Referenz & ref,double max_energy_){
  omega_R = ref.omega_R;
  eta_R = ref.eta_R;
  beta_R = ref.beta_R;
  W_R = ref.w_R;
  T = ref.T_R;

  u_1R = ref.u_1R;
  u_2R = ref.u_2R;
  phi_R = ref.phi_R;
  phi_dh2 = ref.phi_dh2;
  max_energy = max_energy_;

  DEo1h = q/(omega_R*h)*u_1R;
  DEo2h = q/(omega_R*h)*u_2R;
  phia = 2*pi*pow(h,2)*eta_R*omega_R/pow(beta_R,2)/W_R;

  syn_freq_dh s1 = syn_freq_dh(T,DEo1h,DEo2h,phia);

  double DEo_LUT_step = max_energy/(DEo_LUT.size()+1);
  for(unsigned int i = 1; i< DEo_LUT.size()-1; i++){
    DEo_LUT[i] = i*DEo_LUT_step;
    syn_T[i] = s1(DEo_LUT[i]);
    syn_freq[i] = 1/syn_T[i];
  }
  DEo_LUT[0] = 0; DEo_LUT[DEo_LUT.size()-1] = max_energy;
  syn_T[0] = syn_T[1]*10; syn_T[DEo_LUT.size()-1] = syn_T[DEo_LUT.size()-2]*10;
  syn_freq[0] = 1/syn_T[0];syn_freq[DEo_LUT.size()-1] = 1/syn_T[DEo_LUT.size()-1];
}

std::tuple<double,double> SynFreq::init_pos (double energy,double frac){
  //search particle quadrant, seperatrix in the beginning has two symmetric axes
  int quadrant = 0;
  if (frac < 0.25) {quadrant = 0;}
  else if (frac <0.5) {quadrant = 1; frac = 0.5-frac;}
  else if (frac < 0.75) {quadrant = 2; frac = frac - 0.5;}
  else {quadrant = 3; frac = 1-frac;}

  //extract frequency
  int choice = (int)(energy*512);
  double frequency_inter = (energy*512)-choice;
  if (choice > 511) qDebug() << choice;
  double frequency = syn_freq[choice]*(1-frequency_inter)+ syn_freq[choice+1]*(frequency_inter);
  double frac_time = 1/frequency*frac;

  double energy_out = energy*max_energy;
  double time = 0;
  double phi = 0;
  while(time < frac_time){
    energy_out = energy_out + DEo1h*sin(phi)+DEo2h*sin(2*phi);
    phi = phi+phia*energy_out;
    time = time+T;
  }

  if (quadrant == 0) {return std::tuple<double,double> (phi,energy_out);}
  else if (quadrant == 1) {return std::tuple<double,double> (phi,-energy_out);}
  else if (quadrant == 2) {return std::tuple<double,double> (-phi,-energy_out);}
  else {return std::tuple<double,double> (-phi,energy_out);}
}
