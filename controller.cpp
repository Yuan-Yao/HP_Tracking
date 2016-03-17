
#include "controller.h"

Controller::Controller(Parameter & para,log_ref_i & lri):
  a(32767),
  s_F(8192),
  h(para.h),
  increment_f_FIR(para.freq_mod),
  fd_delay_Td(pow(10,-5)), // 10µsec
  internal_time_count(3.22 * pow(10,-6)), // 3,22µs sampling time, constant of DSP_Software_implementation41

  gain_FIR(para.gain),
  offset (-1), //controller has to be filled up

  energy_sink_flag(std::vector<int>(h)),
  u(std::vector<double>(h))
{
  gain_FIR_Liwei = -2*M_PI*lri.f_syn*lri.T_R;
  fd_fir_fpass = increment_f_FIR*lri.f_syn;
  fd_delay_kd = (int) fd_delay_Td*lri.f_R+0.5;
  fd_delay_fir1 = (int) lri.f_R/(2*fd_fir_fpass)+0.5;
  fd_delay_fir2 = (int) lri.f_R/(fd_fir_fpass)+0.5;

  fd_xfir.assign(h, std::vector<double>(para.anz_Umdrehungen+1,0));
  fd_xI.assign(h, std::vector<double>(para.anz_Umdrehungen+1,0));
  Delta_phi_gap.assign(h, std::vector<double>(para.anz_Umdrehungen+2));
  DH_phi_gap.assign(h,std::vector<double>(para.anz_Umdrehungen+1));
  DH_phi_gap_10.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH_energy.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH_energy_10.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH_integrator.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH_integrator_energy.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH_pos.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH_vel.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH_sin_part_16.assign(h,std::vector< double> (16,0));
  DH_base.assign(h,0);
  DH_rad.assign(h,0);
  DH_phase.assign(h,0);
  DH_mode = -1;
  DH_SD = 0.05;
  DH_LDC.assign(h,0);
  DH_MD.assign(h,0);
  lock = false;

  DH2_phi_gap.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH2_base.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH2_u.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH2_integrator.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH2_energy.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));
  DH2_phi_gap_10.assign(h,std::vector<double>(para.anz_Umdrehungen+1,0));

  DH2_local_max.assign(h,0);
  DH2_local_min.assign(h,0);
  DH2_local_div.assign(h,std::vector< double >(para.anz_Umdrehungen+1,0));
  DH2_msl.assign(h,0);

  DH2_damp.assign(h,false);

  DH2_std_dev = 0;
  fir_phi_det.assign(h, std::vector<double>(para.anz_Umdrehungen+1));

  for (int k = 0; k<h; k++){
    DH_base[k] = 0;
    Delta_phi_gap[k][0] = 0;
    fd_xI[k][0] = 0; fd_xfir[k][0] = 0;
    energy_sink_flag[k] = 0;
    u[k] = 0;
    for(int j = 0; j< 16; j++){
      DH_sin_part_16[k][j] = 0;
    }
    DH_phi_gap_10[k][0] = 0.0;
  }
}

//para.reglertyp decides, which Controller will be used
void Controller::operate(int i, Log_Bunch & log,log_ref_i & lri, Parameter & para ){
  if (para.reglertyp == "3_TAP_Filter") {three_Tap_Filter(i,log,lri,para);}
  else if(para.reglertyp == "3_TAP_Filter_EXP") {three_Tap_Filter_Experiment(i,log,lri,para);}
  else if(para.reglertyp == "3_TAP_Filter_Liwei") {three_Tap_Filter_Liwei(i,log,lri,para);}
  else if(para.reglertyp == "Zustandsregler") {Zustandsregler(i,log,lri,para);}
  else if(para.reglertyp == "Energy_Sink_Regler") {Energy_Sink(i,log,lri,para);}
  else if(para.reglertyp == "Hamilton_Damp") {Hamilton_Damp(i,log,lri,para);}
  else if(para.reglertyp == "Hamilton_Damp_2") {Hamilton_Damp_2(i,log,lri,para);}
  else {no_controller(i,log);}
}

//
//dummy_control, no action
//
void Controller::no_controller(int i, Log_Bunch & log){
  for (int k = 0; k<h; k++){
    fir_phi_det[k][i] = log.get_log_fft_phase_0_control(k,i)-Delta_phi_gap[k][i];
    fd_xfir[k][i-1] = 0;
    fd_xI[k][i-1] = 0;
    Delta_phi_gap[k][i+1] = 0;
    u[k] = Delta_phi_gap[k][i+1];
  }
}

//
//3-tap-filter every turn calculation
//
void Controller::three_Tap_Filter(int i,Log_Bunch & log, log_ref_i & lri, Parameter & para){
  //refer to paper "A Digital Beam-Phase Control System for Heavy-Ion Synchrotrons"
  //i = Umdrehung
  //k = Zähler für h ( Anzahl des Bunchs)
  //3-tap-filter
    qDebug()<<"3-Tap-FIR Filter with i= "<<i;
  for(int k = 0; k <h; k++){

    fir_phi_det[k][i] = log.get_log_fft_phase_0_control(k,i)-Delta_phi_gap[k][i];
    //qDebug()<<log.get_log_fft_phase_0_control(k,i)<<""<<Delta_phi_gap[k][i];

    //qDebug()<<"fir_phi_det["<<k<<"]["<<i<<"]="<<fir_phi_det[k][i];

  }
  //If there are enough values for controller available
  if ((i > fd_delay_fir2) && (para.coasting == false)){//no acceleration
    for (int k = 0; k < h; k++){
      qDebug()<<"fd_fir_fpass is: "<<fd_fir_fpass;//const???
      //qDebug()<<"fd_delay_kd is: "<<fd_delay_kd;//const??
      //qDebug()<<"fd_delay_fir1 is: "<<fd_delay_fir1;//const???
      //qDebug()<<"fd_delay_fir2 is: "<<fd_delay_fir2;//const???
      //qDebug()<<"s_F is "<<s_F;
      fd_xfir[k][i] = s_F*(-fir_phi_det[k][i]+2*fir_phi_det[k][i-fd_delay_fir1]-fir_phi_det[k][i-fd_delay_fir2])/32768;
      //qDebug()<<"FIR1 is "<<fd_delay_fir1;
      //qDebug()<<"FIR2 is "<<fd_delay_fir2;
      qDebug()<<"SF is "<<s_F;
      qDebug()<<"a is "<<a;
      //qDebug()<<""


      fd_xI[k][i] =  a*(fd_xI[k][i-1])/32768+fd_xfir[k][i]; //integration
      Delta_phi_gap[k][i+1] = gain_FIR * fd_xI[k][i-fd_delay_kd]; //delay time for transmission to plant
      qDebug()<<"Fd_delay"<<fd_delay_kd;
      qDebug()<<"gain_FIR is "<<gain_FIR;
      double t_hilf = 1.0/(1.0+2.0/3.2);
      qDebug()<<"t-hilf is "<<t_hilf;

      Delta_phi_gap[k][i+1] = t_hilf * (Delta_phi_gap[k][i+1]-Delta_phi_gap[k][i]) + Delta_phi_gap[k][i];
    }
  }
  //otherwise, values are gathered
  else{
    for(int k = 0; k<h; k++){
      Delta_phi_gap[k][i+1] = Delta_phi_gap[k][i];
      fd_xfir[k][i] = fd_xfir[k][i-1];
      fd_xI[k][i] = fd_xI[k][i-1];
    }
  }
  for(int k = 0; k<h; k++){
    u[k] = Delta_phi_gap[k][i+1];
  }

  //fit zero taps to new synchrotron-frequency; important in case of acceleration
  fd_fir_fpass = increment_f_FIR*lri.f_syn;
  qDebug()<<increment_f_FIR<<"*"<<lri.f_syn<<"="<<fd_fir_fpass;
  fd_delay_kd = (int) (fd_delay_Td*lri.f_R+0.5);
  qDebug()<<fd_delay_Td<<"*"<<lri.f_R<<"+"<<"0.5";
  fd_delay_fir1 = (int) (lri.f_R/(2*fd_fir_fpass)+0.5);
  fd_delay_fir2 = (int) (lri.f_R/(fd_fir_fpass)+0.5);
}

//
// experimental 3-tap-filter
//
void Controller::three_Tap_Filter_Experiment(int i, Log_Bunch &log, log_ref_i & lri, Parameter &para){
  //filter used like in experiment. Only all 3.22µs action.
  for(int k = 0; k<h; k++){
    fir_phi_det[k][i] = log.get_log_fft_phase_0_control(k,i)-Delta_phi_gap[k][i];
  }

  if (offset < 0){
    if (lri.T_R < internal_time_count){
      offset = lri.t_R;
      for(int k = 0; k<h; k++){
        Delta_phi_gap[k][i+1] = 0;
        fd_xfir[k][i] = fd_xfir[k][i-1];
        fd_xI[k][i] = fd_xI[k][i-1];

      }
    }
  }
  //actual controller
  else if((lri.t_R - offset > internal_time_count) && (para.coasting == false)){
    offset += internal_time_count;
    if(i > fd_delay_fir2){
      for(int k = 0; k < h; k++) {
        fd_xfir[k][i] = s_F*(-fir_phi_det[k][i]+2*fir_phi_det[k][i-fd_delay_fir1]-fir_phi_det[k][i-fd_delay_fir2])/32768;
        fd_xI[k][i] =  a*(fd_xI[k][i-1])/32768+fd_xfir[k][i]; //integration
        Delta_phi_gap[k][i+1] = gain_FIR * fd_xI[k][i-fd_delay_kd]; //delay time to plant
        double t_hilf = 1/(1 + 2/3.22);//cayity as PT1
        Delta_phi_gap[k][i+1] = t_hilf * (Delta_phi_gap[k][i+1] -Delta_phi_gap[k][i]) + Delta_phi_gap[k][i];
      }
    }
  }
  //if nothing matches, no action takes place
  else{
    for(int k = 0; k<h; k++){
      Delta_phi_gap[k][i+1] = Delta_phi_gap[k][i];
      fd_xfir[k][i] = fd_xfir[k][i-1];
      fd_xI[k][i] = fd_xI[k][i-1];
    }
  }

  for(int k = 0; k<h; k++){
    u[k] = Delta_phi_gap[k][i+1];
  }

  //fit registers to new synchrotron frequency; important in case of acceleration
  fd_fir_fpass = increment_f_FIR*lri.f_syn;
  fd_delay_kd = (int) (fd_delay_Td*lri.f_R+0.5);
  fd_delay_fir1 = (int) (lri.f_R/(2*fd_fir_fpass)+0.5);
  fd_delay_fir2 = (int) (lri.f_R/(fd_fir_fpass)+0.5);
}



//
//3-Tap-Filter- von Masterarbeit Liwei Shen
//
void Controller::three_Tap_Filter_Liwei(int i, Log_Bunch &log, log_ref_i & lri, Parameter &para){
  //refer to paper "A Digital Beam-Phase Control System for Heavy-Ion Synchrotrons"
  //3-tap-filter
  for(int k = 0; k <h; k++){

    fir_phi_det[k][i] = log.get_log_fft_phase_dis_0_control(k,i)-Delta_phi_gap[k][i];
  }
  //If there are enough values for controller available
  if ((i > fd_delay_fir2) && (para.coasting == false)){
    for (int k = 0; k < h; k++){
      fd_xfir[k][i] = s_F*(-fir_phi_det[k][i]+2*fir_phi_det[k][i-fd_delay_fir1]-fir_phi_det[k][i-fd_delay_fir2])/32768;
      fd_xI[k][i] =  a*(fd_xI[k][i-1])/32768+fd_xfir[k][i]; //integration
      Delta_phi_gap[k][i+1] = gain_FIR_Liwei * fd_xI[k][i-fd_delay_kd]; //delay time for transmission to plant
      double t_hilf = 1.0/(1.0+2.0/3.2);
      Delta_phi_gap[k][i+1] = t_hilf * (Delta_phi_gap[k][i+1]-Delta_phi_gap[k][i]) + Delta_phi_gap[k][i];
    }
  }
  //otherwise, values are gathered
  else{
    for(int k = 0; k<h; k++){
      Delta_phi_gap[k][i+1] = Delta_phi_gap[k][i];
      fd_xfir[k][i] = fd_xfir[k][i-1];
      fd_xI[k][i] = fd_xI[k][i-1];
    }
  }
  for(int k = 0; k<h; k++){
    u[k] = Delta_phi_gap[k][i+1];
  }

  //fit zero taps to new synchrotron-frequency; important in case of acceleration
  fd_fir_fpass = increment_f_FIR*lri.f_syn;
  fd_delay_kd = (int) (fd_delay_Td*lri.f_R+0.5);
  fd_delay_fir1 = (int) (lri.f_R/(2*fd_fir_fpass)+0.5);
  fd_delay_fir2 = (int) (lri.f_R/(fd_fir_fpass)+0.5);
  gain_FIR_Liwei = -2*M_PI*lri.f_syn*lri.T_R;

  for(int k = 0; k<h; k++){
    DH2_u[k][i] = u[k];
  }
}
//
//3-Tap-Filter- von Masterarbeit Liwei Shen
//


//
//state-feedback-controller
//
void Controller::Zustandsregler(int i, Log_Bunch & log, log_ref_i & lri, Parameter & para){

  //Do nothing, when particles are to slow
  if (offset < 0){
    if (lri.T_R < internal_time_count){
      offset = lri.t_R;
      for(int k = 0; k<h; k++){
        Delta_phi_gap[k][i+1] = Delta_phi_gap[k][i];
      }
    }
  }
  //actual controller
  else if((lri.t_R - offset > internal_time_count) && (para.coasting == false)){
    offset += internal_time_count;
    for(int k = 0; k < h ; k++){
      Delta_phi_gap[k][i+1] = log.get_log_fft_phase_0_control(k,i)/2+log.get_Deo_control(k,i)*pow(10,18)*3;
    }
  }
  //if nothing matches, no action takes place
  else{
    for(int k = 0; k<h; k++){
      Delta_phi_gap[k][i+1] = Delta_phi_gap[k][i];
    }
  }

  for(int k = 0; k<h; k++)
  {
    u[k] = Delta_phi_gap[k][i+1];
  }
}

//
//energy_Sink_Controller
//
void Controller::Energy_Sink(int i, Log_Bunch &log, log_ref_i & lri, Parameter &para){

  //Delta_phi_gap is a storage for delayed u
  for (int k = 0; k < h; k++){

    double off = lri.phi_1R;
    double pos = log.get_log_fft_phase_0_control(k,i);
  //flag 0 => no distinct behaviour; voltage is shifted to half the phase-deviation of the bunch
  if (energy_sink_flag[k] == 0){
    qDebug() << i << k << energy_sink_flag[k] ;
    //Control law; energy might not drop any more? check, if this is the case, when motion is delayed
    if (i+5 < para.anz_Umdrehungen) Delta_phi_gap[k][i+5] = (pos-off)/2;
    u[k] = Delta_phi_gap[k][i];

    if (pos > M_PI /360) energy_sink_flag[k] = 1;
    if (pos < M_PI /360) energy_sink_flag[k] = 2;
  }

  //flag 1 => particle in phase space (program) is shifted to positive phase deviation and has positive energy deviation;
  //in positive part, Gapvoltage to phase deviation half; in negative part, Gapvoltage = 0; when energy gets negative energy deviation go to flag 2;
  else if (energy_sink_flag[k] == 1){
    qDebug() << i << k << energy_sink_flag[k] << Delta_phi_gap[k][i];
    //Control law
    //if (log.get_log_fft_phase_0_control(k,i) > 0){
      double temp = (pos - log.get_log_fft_phase_0_control(k,i-1))*100;
      if (temp > fabs((pos-off)/4)) temp = temp/fabs(temp)*(pos-off)/4;
      if (i+5 < para.anz_Umdrehungen) Delta_phi_gap[k][i+5] = (pos-off)/2 - temp;
      u[k] = Delta_phi_gap[k][i];
    //}
    //else{
    //  u[k] = 0;
    //}
    //Switch
    if ((pos-off) < 0 && (pos -log.get_log_fft_phase_0_control(k,i-1) > 0)) energy_sink_flag[k] = 2;
  }

  //flag 1 => particle in phase space (program) is shifted to positive phase deviation and has positive energy deviation;
  //in positive part, Gapvoltage to phase deviation half; in negative part, Gapvoltage = 0; when energy gets negative energy deviation go to flag 2;
  else if (energy_sink_flag[k] == 2){
    qDebug() << i << k << energy_sink_flag[k];
    //Control law
    //if (log.get_log_fft_phase_0_control(k,i) < 0){
      double temp = (pos - log.get_log_fft_phase_0_control(k,i-1))*100;
      if (temp > fabs((pos-off)/4)) temp = temp/fabs(temp)*(pos-off)/4;
      if (i+5 < para.anz_Umdrehungen) Delta_phi_gap[k][i+5] = (pos-off)/2 - temp;
      u[k] = Delta_phi_gap[k][i];
    //}
    //else{
    //  u[k] = 0;
    //}
    //Switch
    if ((pos-off) > 0 && (pos - log.get_log_fft_phase_0_control(k,i-1) < 0)) energy_sink_flag[k] = 1;
  }

  }
}

//
//Hamilton_Damp_Controller
//
void Controller::Hamilton_Damp(int i, Log_Bunch &log, log_ref_i &log_ref, Parameter &para){

//always happens starts
//
  //internal Delta_phi_gap,like in accelerator, Bunch_position - control_phase_shift - coherent_phase
  for(int k = 0; k <h; k++){
    DH_phi_gap[k][i] = (log.get_log_fft_phase_dis_0_control(k,i))-log_ref.phi_1R-u[k]; //u[k] is 0, when the Fourier Transform is derived; DH_phi_gap may contain offset
    DH_integrator[k][i] = DH_integrator[k][i-1] + (DH_phi_gap[k][i])*log_ref.T_R;
    //DH_integrator[k][i] = DH_integrator[k][i-1] + (DH_phi_gap[k][i])*log_ref.T_R;
  //as acceleration starts, additional shift by 0,01, thats within small control interval, < 1° in rad
    //if (i == 1000) {
      //std::vector<double> temp1 = own_math2::reduc_vec(DH_phi_gap[k],0,999);
      //std::vector<double> temp2 = own_math2::reduc_vec(DH_phi_gap[k],900,999);
      //std::vector<double> temp3 = own_math2::reduc_vec(DH_phi_gap[k],990,999);
      //std::vector<double> temp4 = own_math2::reduc_vec(DH_phi_gap[k],995,999);
      //qDebug() << "Standardabweichung schätzen 1000" << own_math2::mean(temp1) << own_math2::standard_deviation(temp1);
      //qDebug() << "Standardabweichung schätzen 100" << own_math2::mean(temp2) << own_math2::standard_deviation(temp2);
      //qDebug() << "Standardabweichung schätzen 10" << own_math2::mean(temp3) << own_math2::standard_deviation(temp3);
      //qDebug() << "Standardabweichung schätzen 5" << own_math2::mean(temp4) << own_math2::standard_deviation(temp4);
    //}
  }

  //continuous Mean-buildung of 10 values.
  if (i < 8){
    for(int k = 0; k < h; k++)
    {
      DH_energy[k][i] = 0;
      DH_pos[k][i] = 0;
      DH_vel[k][i] = 0;
    }
  }
  else{
    for (int k = 0; k < h; k++){
      std::vector<double> temp = own_math2::reduc_vec(DH_phi_gap[k],i-8,i+1);
      DH_energy[k][i] = own_math2::energy(temp,log_ref.f_syn,log_ref.T_R)[2];
      DH_pos[k][i] = own_math2::energy(temp,log_ref.f_syn,log_ref.T_R)[0];
      DH_vel[k][i] = own_math2::energy(temp,log_ref.f_syn,log_ref.T_R)[1];
    }
  }
  if (i < 10){
    for(int k = 0; k < h; k++){
      std::vector<double> temp = own_math2::reduc_vec(DH_phi_gap[k],0,i);
      std::vector<double> temp2 = own_math2::reduc_vec(DH_energy[k],0,i);
      DH_phi_gap_10[k][i] = own_math2::mean(temp);
      DH_energy_10[k][i] = own_math2::mean(temp2);
    }
  }
  else{
    for(int k = 0; k < h ; k++){
      DH_phi_gap_10[k][i] = (DH_phi_gap_10[k][i-1]*9 + DH_phi_gap[k][i])/10;
      DH_energy_10[k][i] = (DH_energy_10[k][i-1]*9+DH_energy[k][i-1])/10;
      DH_integrator_energy[k][i] = 0;
    }
  }

//
//always happens ends
//


  //
  //continuous Mean-building for 16 part DFT for estimated Sinus:: 0.95 of linear synchrotron frequency
  //
  //init mode -1; because synchrotronfrequency is needed and standard-deviation also
  if(DH_mode == -1 && para.coasting == false && i > 99){
    double f_syn_temp = 0.95*log_ref.f_syn;
    DHt_start = log_ref.t_R;
    DHt_end = log_ref.t_R + 1/f_syn_temp;
    DHt_ptime = 1/f_syn_temp/16;
    DHt_pnum = 0;
    DHt_count = 1;
    DH_mode = 0;
    std::vector<double> temp = own_math2::reduc_vec(DH_phi_gap[0],0,99); //einmalig
    DH_SD = own_math2::standard_deviation(temp);
    for(int k = 0; k < h; k++){
      DH_sin_part_16[k][0] = DH_phi_gap[k][i];
    }
  }
  // mode 0; add values for sin_part_16_mean
  else if(DH_mode == 0){
    for(int k = 0; k < h; k++){
      u[k] = 0;
    }
    //check for large deviation -> more than 2 SD two times
    for(int k = 0; k < h; k++){
      if(fabs(DH_phi_gap_10[k][i]-DH_base[k]) > fabs(2 * DH_SD) && lock == false){
       DH_LDC[k]++;
       if (DH_LDC[k] > 5) {
         DH_integrator_energy[k][i] = 0;
         std::vector<double> temp = own_math2::reduc_vec(DH_phi_gap[k],i-100,i);
         qDebug() << "first set"<< own_math2::mean(temp) << temp.size() << log_ref.T_R << i;
         DH_integrator[k][i] = own_math2::mean(temp)*temp.size()*log_ref.T_R;
         DH_mode = 1;
         DH_LDC[k] = 0;
       }
     }
    }

    //add data to sin_part
    if (log_ref.t_R < DHt_start + (DHt_pnum+1)*DHt_ptime){
      for(int k = 0; k < h; k++){
        DH_sin_part_16[k][DHt_pnum] = DH_sin_part_16[k][DHt_pnum]+DH_phi_gap[k][i];
        DHt_count++;
      }
    }
    //interval completed; reset for new acqusition; analyse data
    else if (log_ref.t_R > DHt_end){
      for(int k = 0; k < h; k++){
        DH_sin_part_16[k][DHt_pnum] = DH_sin_part_16[k][DHt_pnum]/DHt_count;
      }

      DHt_count = 0;
      DHt_pnum = 0;
      DHt_start = DHt_end;
      DHt_ptime = 1/log_ref.f_syn/16;
      DHt_end = DHt_start + 1/log_ref.f_syn;

      for(int k = 0; k < h; k++){
        //DH_integrator[k][i] = DH_integrator[k][i]- DH_integrator[k][i-100];
        DH_base[k] = own_math2::mean(DH_sin_part_16[k]);
        DH_rad[k] = own_math2::DFT_1_rad(DH_sin_part_16[k]);
        DH_phase[k] = own_math2::DFT_1_phase(DH_sin_part_16[k]);
        if (DH_lock_counter == 0) {DH_lock_counter++;}
        else{lock = false; DH_lock_counter = 0;}
        qDebug() << "Fourier" << DH_base[k] << DH_rad[k] << DH_phase[k] << i;
        //DH_mode = 2;
        DH_small_dev_start = 0.0;
        DH_small_dev_end = 0.0;
      }

      //qDebug() << DH_sin_part_16[0][0] << DH_sin_part_16[0][1] << DH_sin_part_16[0][2] << DH_sin_part_16[0][3] << DH_sin_part_16[0][4] << DH_sin_part_16[0][5] << DH_sin_part_16[0][6] << DH_sin_part_16[0][7] << DH_sin_part_16[0][8] << DH_sin_part_16[0][9] << DH_sin_part_16[0][10] << DH_sin_part_16[0][11] << DH_sin_part_16[0][12] << DH_sin_part_16[0][13] << DH_sin_part_16[0][14] << DH_sin_part_16[0][15];
      //qDebug() << "Störungsermittelung" << i;
      //qDebug() << own_math2::mean(DH_sin_part_16[0]);
      //qDebug() << own_math2::DFT_1_rad(DH_sin_part_16[0]);
      //qDebug() << own_math2::DFT_1_phase(DH_sin_part_16[0]) << own_math2::DFT_1_phase(DH_sin_part_16[0])/2/M_PI*360;

      //reset all DH_sin_part_16 values for new acqusition
      for (int k = 0; k < h; k++){
        std::fill(DH_sin_part_16[k].begin(),DH_sin_part_16[k].end(),0.0);
      }
    }
    //move to new part; mean value of old part
    else if(log_ref.t_R > DHt_start + (DHt_pnum+1)*DHt_ptime){
      for(int k = 0; k < h; k++){
        DH_sin_part_16[k][DHt_pnum] = DH_sin_part_16[k][DHt_pnum]/DHt_count;
      }
      DHt_pnum++;
      DHt_count = 0;
      for(int k = 0; k < h; k++){
        DH_sin_part_16[k][DHt_pnum] = DH_sin_part_16[k][DHt_pnum]+DH_phi_gap[k][i];
        DHt_count++;
      }
    }
  }
  // mode else
  else if(DH_mode == 1){
    for(int k = 0; k < h; k++){
      //if(fabs(DH_phi_gap_10[k][i]) > fabs(DH_MD[k])) {
        DH_integrator_energy[k][i] = pow(u[k]+DH_phi_gap[k][i],2)+pow(DH_integrator[k][i]*log_ref.f_syn*2*M_PI,2);
        if(sqrt(DH_integrator_energy[k][i])/2 > fabs(DH_MD[k])) DH_MD[k] = sqrt(DH_integrator_energy[k][i])/2;
      //}
      u[k] = DH_MD[k];
      //if(fabs(DH_phi_gap_10[k][i]-DH_base[k]) < fabs(2*DH_SD)) DH_LDC[k]++;
      //if(DH_LDC[k] > 25){
      if(DH_integrator[k][i] < 0){
        DH_LDC[k] = 0;
        u[k] = 0;
        DH_mode = 0;
        DH_MD[k] = 0;
        lock = true;
        DH_integrator[k][i] = 0;
        qDebug() << "kaputt" << i;

        //reset DH_sin_16 for Fourier Trafo
        DHt_count = 0;
        DHt_pnum = 0;
        DHt_start = log_ref.t_R;
        DHt_ptime = 1/log_ref.f_syn/16;
        DHt_end = DHt_start + 1/log_ref.f_syn;
      }
    }
  }
  else if(DH_mode == 2){

  }
}

//
//Hamilton_Damp_Controller_2
//
void Controller::Hamilton_Damp_2(int i, Log_Bunch &log, log_ref_i &log_ref, Parameter &para){

//always happens

  //internal Delta_phi_gap,like in accelerator, Bunch_position - control_phase_shift - coherent_phase
  for(int k = 0; k <h; k++){
    DH2_base[k][i] = DH2_base[k][i-1];
    DH2_phi_gap[k][i] = log.get_log_fft_phase_dis_0_control(k,i)-log_ref.phi_1R-DH2_u[k][i]+0.05; //u[k] is 0, DH_phi_gap may contain offset
    DH2_integrator[k][i] = DH2_integrator[k][i-1] + (DH2_phi_gap[k][i]-DH2_base[k][i])*log_ref.T_R*log_ref.f_syn;
  }

  //StandardAbweichung einmal festlegen, nach 100 Schritten. Erstmal wird davon ausgegangen, dass kein Sprung und keine starken Schwingungen vorliegen;
  if (i == 100){
    std::vector<double> temp = own_math2::reduc_vec(DH2_phi_gap[0],0,99);
    DH2_std_dev = own_math2::standard_deviation(temp);
    qDebug() << DH2_std_dev;
  }

  //Mittelwerte bilden über 10 Werte
  if (i < 10){
    for(int k = 0; k < h; k++){
      DH2_phi_gap_10[k][i] = (DH2_phi_gap_10[k][i-1]*(i-1)+DH2_phi_gap[k][i])/i;
      DH2_base[k][i] = DH2_phi_gap_10[k][i];
      DH2_integrator[k][i] = 0;
    }
  }
  else{
    for(int k = 0; k < h; k++){
    DH2_phi_gap_10[k][i] = DH2_phi_gap_10[k][i-1] + 0.1*(DH2_phi_gap[k][i] - DH2_phi_gap[k][i-10]);
    double temp = 1/log_ref.f_syn/log_ref.T_R;
    qDebug() << temp;
    if (DH2_damp[k] == false) DH2_base[k][i] = (DH2_base[k][i]*(temp-1) + DH2_phi_gap[k][i])/temp;
    }
  }

//Maximum und Minimum tracken; DH2_msl == 0 init;
  for (int k = 0; k < h; k++){
    if (DH2_msl[k] == 0){
      if (DH2_integrator[k][i] > DH2_local_max[k] + 0.001){DH2_local_max[k] = DH2_integrator[k][i]; DH2_msl[k] = 1;}
      if (DH2_integrator[k][i] < DH2_local_min[k] - 0.001){DH2_local_min[k] = DH2_integrator[k][i]; DH2_msl[k] = 2;}
    }
    //Maximum und Minimum tracken; DH2_msl == 1;
    else if (DH2_msl[k] == 1){
      if (DH2_integrator[k][i] > DH2_local_max[k]){
        DH2_local_max[k] = DH2_integrator[k][i];
        DH2_local_div[k][i] = DH2_local_max[k]-DH2_local_min[k];
        if (fabs(DH2_local_div[k][i]) > 0.004) DH2_damp[k] = true;
      }
      else if (DH2_integrator[k][i] < DH2_local_max[k] - 0.001){
        double temp = (DH2_local_max[k]+DH2_local_min[k])/2;
        if (fabs(DH2_local_div[k][i-1]) > 0.004) temp = 0;
        qDebug() << "before switch max to min" << i << DH2_local_div[k][i-1] << DH2_integrator[k][i] << DH2_local_max[k] << DH2_local_min[k]<< temp;
        DH2_integrator[k][i] = DH2_integrator[k][i] - temp;
        DH2_local_max[k] = DH2_local_max[k]-temp;
        DH2_local_min[k] = DH2_integrator[k][i];
        DH2_local_min[k] = DH2_integrator[k][i];
        DH2_local_div[k][i] = DH2_local_max[k]-DH2_local_min[k];
        DH2_msl[k] = 2;
        if(temp == 0) {DH2_damp[k] = true;}
        else {DH2_damp[k] = false;}
        qDebug() << i << "after switch" << DH2_local_div[k][i] << DH2_integrator[k][i] << DH2_local_max[k] << DH2_local_min[k];
      }
      else{
        DH2_local_div[k][i] = DH2_local_max[k]-DH2_local_min[k];
      }
    }
    //Maximum und Minimum tracken; DH2_msl == 2;
    else if(DH2_msl[k] == 2){
      if (DH2_integrator[k][i] < DH2_local_min[k]){
        DH2_local_min[k] = DH2_integrator[k][i];
        DH2_local_div[k][i] = DH2_local_max[k]-DH2_local_min[k];
        if (fabs(DH2_local_div[k][i]) > 0.004) DH2_damp[k] = true;
      }
      else if (DH2_integrator[k][i] > DH2_local_min[k] + 0.001){
        double temp = (DH2_local_max[k]+DH2_local_min[k])/2;
        if (fabs(DH2_local_div[k][i-1]) > 0.004) temp = 0;
        qDebug() << "before switch min to max" << i << DH2_local_div[k][i-1] << DH2_integrator[k][i] << DH2_local_max[k] << DH2_local_min[k]<< temp;
        DH2_integrator[k][i] = DH2_integrator[k][i] - temp;
        DH2_local_max[k] = DH2_integrator[k][i];
        DH2_local_min[k] = DH2_local_min[k]-temp;
        DH2_local_div[k][i] = DH2_local_max[k]-DH2_local_min[k];
        DH2_msl[k] = 1;
        if(temp == 0) {DH2_damp[k] = true;}
        else {DH2_damp[k] = false;}
        qDebug() << i << "after switch" << DH2_local_div[k][i] << DH2_integrator[k][i] << DH2_local_max[k] << DH2_local_min[k];
      }
     else{
       DH2_local_div[k][i] = DH2_local_max[k]-DH2_local_min[k];
     }
   }
 }
}

