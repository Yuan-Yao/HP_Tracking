#include "bunch.h"

//constructor
Bunch::Bunch(Parameter & para):
  phi(std::vector<double>(para.anz_Teilchen)),//0,0,0,0...... in phi
  DEo(std::vector<double>(para.anz_Teilchen)),
  lost(std::vector<int>(para.anz_Teilchen)),
  lost_count(0.0),
  h(para.h),
  mp1(std::vector<double>(h)),//h dimentional vec
  md1(std::vector<double>(h)),//...
  particle_count(std::vector<int>(h)),
  histogramm_complete(std::vector<double>(h*para.histo_anz)),
  histogramm_complete_energy(std::vector<double>(h*para.histo_anz)),
  histogramm_hamilton_energy_complete(std::vector<double>(h*para.histo_anz)),
  mp2(std::vector<double>(h)),
  md2(std::vector<double>(h)),
  mp1d1(std::vector<double>(h)),
  particle_in_bunch(std::vector<int>(para.anz_Teilchen))
{
  histogramm.assign(h,std::vector<double>(para.histo_anz));//h mal vec full of para.histo_anz
  histogramm_energy.assign(h,std::vector<double>(para.histo_anz));//...
  histogramm_hamilton_energy.assign(h,std::vector<double>(para.histo_anz));//...
  mp1_tracker.assign(h,QVector<double>(0));//h mal 0
  md1_tracker.assign(h,QVector<double>(0));// h mal 0
}

//
//first moment and histogram calculation
//


//md1 and ma1 initial with 0.0
void Bunch::moment_actualisation(Parameter & para){
  for (unsigned int i = 0; i< mp1.size(); i++){
    mp1[i] = 0.0;
    md1[i] = 0.0;
    particle_count[i] = 0;
  }

  //first order moments
  for(int i = 0; i < para.anz_Teilchen; i++){
    if (lost[i] == 0){
      int temp = (int) ((phi[i]+para.pi)/2/para.pi);//
      particle_in_bunch[i] = temp;
      mp1[temp] += phi[i];//a=a+1
      md1[temp] += DEo[i];
      particle_count[temp] += 1;
    }
  }
  for (unsigned int i = 0; i< mp1.size(); i++){
    mp1[i] = mp1[i]/particle_count[i];
    md1[i] = md1[i]/particle_count[i];
  }
  first_order_moment_corrector();

  //particles to complete histogram
  histogramm_complete.assign(para.histo_anz*h,0.0);
  double help = para.histo_anz/(2*para.pi);
  double help_position = 0;
  for (int i=0; i<para.anz_Teilchen; i++){
    if(lost[i] == 0){
      help_position = (phi[i]+para.pi)*help;
      histogramm_complete[help_position] += 1.0;
    }
  }

  //particles to complete histogramm_energy
  histogramm_complete_energy.assign(para.histo_anz*h,0.0);
  help = 0.0;
  //particles to complete histogramm_hamilton_energy

  //particles to seperate bucket histograms
  int iterator = histogramm[0].size();
  for (int j = 0; j<h;j++){
    for (int k = 0; k < iterator; k++){
      histogramm[j][k] = histogramm_complete[j*iterator+k];
    }
  }

  //particles to seperate bucket histogram_energy

  //particles to seperate bucket histogramm_hamilton_energy

}
//
//
//

//
//second order moment calculations
//
void Bunch::moment_actualisation_2(Parameter & para){
  for (int i = 0; i < h; i++){
    mp2[i] = 0.0;
    md2[i] = 0.0;
    mp1d1[i] = 0.0;
  }

  for(int i = 0; i < para.anz_Teilchen; i++){
    if (lost[i] == 0){
      int temp = (int)((phi[i]+para.pi)/(2*para.pi));
      mp2[temp] += pow(phi[i] - mp1[temp],2);
      mp1d1[temp] += (phi[i] - mp1[temp]) * (DEo[i]-md1[temp]);
      md2[temp] += pow(DEo[i] - md1[temp],2);
    }
  }

  for (int i = 0; i < h; i++){
    mp2[i] = mp2[i]/particle_count[i];
    mp1d1[i] = mp1d1[i]/particle_count[i];
    md2[i] = md2[i]/particle_count[i];
  }
}
//
//
//


//
//help function for double harmonic particle placement
//
std::vector<double> dh_vertfunk(){
  std::vector<double> area_count = std::vector<double>(100);
  double area = 0.0;
  for(unsigned int i=0; i < area_count.size();i++){
    double temp = double(i+1);
    area_count[i] = 1.19242*(temp/(temp+8.76129))*(236.553)/(exp((temp-52.2506)/5.65456)+1); //fitted data from coasting to bunch in double harmonic potential
    area = area+(area_count[i]);
  }
  for(unsigned int i=0; i < area_count.size();i++){
    area_count[i] = area_count[i]/area;
    if (i>0) area_count[i] = area_count[i] + area_count[i-1];
    if(area_count[i] > 1) area_count[i] = 1;
  }
  return area_count;
}
//
//
//

//
//init_bunch
//
void Bunch::init_bunch (Parameter & para, Seperatrix & sep,Referenz & ref){

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-para.pi,para.pi+2*(para.h-1)*para.pi);
  std::uniform_int_distribution<> dis_int(1,h);
  std::tuple<double,double> temp;

  //coasting beam - bunch with gauss_spread energy values and uniform distributed phi
  if (para.single_particle == true){
    phi[0] = 0;
    DEo[0] = 0;
    lost.assign(1,0);
    lost_threshold = 4*sep.get_stat_max_sep();
  }

  else if (para.coasting == true){
    for (int i = 0; i< para.anz_Teilchen;i++){
      phi[i] = double(dis(gen));
    }
    std::normal_distribution<> ndis(0,0.5*0.27* sep.get_stat_max_sep());
    for (int i = 0; i< para.anz_Teilchen;i++){
      DEo[i] = double(ndis(gen));
    }
    lost.assign(para.anz_Teilchen,0);
    lost_threshold = 4*sep.get_stat_max_sep();
  }

  //bunched beam- single harmonic potential
  else if (para.spannungs_Typ == "eh"){
    double temp = -(pow(para.fuellgrad*sep.get_stat_max_sep(),2)/sep.get_sepfactor_stat()-1); //fuellgrad determines the outer most energy within the seperatrix
    double maxphi = acos(temp);
    for (int i = 0; i< para.anz_Teilchen;i++){
      std::normal_distribution<> ndis(0,0.33);
      phi[i] = (double)(ndis(gen))*maxphi;
      if (phi[i] < -para.pi ) {phi[i] = phi[i]+2*para.pi*h;} else if (phi[i] > 2*para.pi*h-para.pi) {phi[i] = phi[i]-2*para.pi*h;}
      DEo[i] = sqrt(-sep.get_sepfactor_stat()*(-1-cos(phi[i])))*(ndis(gen))*para.fuellgrad;
      phi[i] = phi[i]+ 2*para.pi*(dis_int(gen)-1);
    }
    lost.assign(para.anz_Teilchen,0);
    lost_threshold = 4*sep.get_stat_max_sep();
  }

  //bunched beam - double harmonic potential
  else if (para.spannungs_Typ == "dh"){
    //experimental energy distribution ist a connection of a saturation and a fermi function
    std::vector<double> dh_vert_funk = dh_vertfunk();
    for(unsigned int i = 0; i< dh_vert_funk.size(); i++){
      dh_vert_funk[i] = (int)(dh_vert_funk[i]*para.anz_Teilchen);
    }
    dh_vert_funk[dh_vert_funk.size()-1] = para.anz_Teilchen;

    int j = 0;
    SynFreq syn = SynFreq(para,ref,sep.get_stat_max_sep());
    std::uniform_real_distribution<> udis (0,1.0);
    for (int i = 0; i < para.anz_Teilchen; i++){
      while(i > dh_vert_funk[j]){j++;}
      double input = fabs( (udis(gen)+j)/dh_vert_funk.size() );
      temp = syn.init_pos(input, udis(gen));
      phi[i] = std::get<0>(temp)+2*para.pi*(dis_int(gen)-1);
      DEo[i] = std::get<1>(temp);
    }

    lost.assign(para.anz_Teilchen,0);
    lost_threshold = 4* sep.get_stat_max_sep();
  }
}
//
//
//


// particle tracking simulation
// voltage eh: u_1R * (sin(phi - u) - sin(phi_R));
// coltage dh: u_1R * (sin(phi - u) - sin(phi_R-u)) + u_2R (sin(2*phi-2*u+phidh_2)-sin(2*phi_R-2*u+phidh_2) )
//
void Bunch::tracking(Parameter & para, log_ref_i & lri,std::vector<double> u)
//u is control variable
{
  if(para.spannungs_Typ == "eh")
  {
    double DEo_a = sin(lri.phi_1R);
    double DEo_b = para.q / (lri.omega_R*h) * lri.u_1R;
    double DEo_c = DEo_b*DEo_a;
    double phi_a = 2*para.pi*pow(para.h,2)*lri.eta_R*lri.omega_R/(pow(lri.beta_R,2)*lri.w_R);
    double phi_b = 2*para.pi*para.h;
    for (int i = 0; i< para.anz_Teilchen;++i){
      if (lost[i] == 0){
        DEo[i] = DEo[i] + DEo_b * sin(phi[i]-u[particle_in_bunch[i]]) - DEo_c;
        phi[i] = (phi[i] + phi_a * DEo[i]);
        if (phi[i]>phi_b-para.pi) {phi[i] = phi[i] - phi_b ;} else if (phi[i] < -para.pi){phi[i] = phi[i]+phi_b;}
        if (fabs(DEo[i]) > lost_threshold) {lost[i] = 1; lost_count +=1.0;}
      }
    }
  }
  else if(para.spannungs_Typ == "dh")
  {
    double DEo_1p = -para.q/(lri.omega_R*h)*(lri.u_1R*sin(lri.phi_1R));
    double DEo_2p = -para.q/(lri.omega_R*h)*(lri.u_2R*sin(2*lri.phi_1R+lri.phi_2R));
    double DEo_1h = para.q/(lri.omega_R*h)*lri.u_1R;
    double DEo_2h = para.q/(lri.omega_R*h)*lri.u_2R;
    double phi_a = 2*para.pi*pow(para.h,2)*lri.eta_R*lri.omega_R/(pow(lri.beta_R,2)*lri.w_R);
    double phi_b = 2*para.pi*para.h;
    for (int i = 0; i< para.anz_Teilchen;++i){
      if (lost[i] == 0){
        DEo[i] = DEo[i] + DEo_1p + DEo_2p + DEo_1h*sin(phi[i]-u[particle_in_bunch[i]]) + DEo_2h*sin(2*phi[i]-2*u[particle_in_bunch[i]]+lri.phi_2R);
        phi[i] = (phi[i] + phi_a * DEo[i]);
        if (phi[i]>phi_b-para.pi) {phi[i] = phi[i] - phi_b ;} else if (phi[i] < -para.pi){phi[i] = phi[i]+phi_b;}
        if (fabs(DEo[i]) > lost_threshold) {lost[i] = 1;lost_count +=1.0;}
      }
    }
  }
}
//
//
//

//
//bunch is moved relative to reference DDS
//
void Bunch::phi_manip(std::vector<double> distortion){
  for(unsigned int i = 0; i< phi.size();i++){
    phi[i] = phi[i] + distortion[particle_in_bunch[i]];
  }
}
//
//
//

//first order moment corrector
void Bunch::first_order_moment_corrector(){
  for (int i = 0; i < h; i++){
    mp1_tracker[i].push_back(mp1[i]);
    md1_tracker[i].push_back(md1[i]);
    if(mp1_tracker[i].size() > 4){
      mp1_tracker[i].pop_front();
      md1_tracker[i].pop_front();
    }
  }
}
