#include "referenz.h"

Referenz::Referenz()
  {
  }

//INIT ENDE
void Referenz::init_ref(Parameter & para){
  w_Kin = para.nukleonen_Energie;
  w_Kin_R =w_Kin * para.e * para.m_Ion;
  w_R =w_Kin_R + para.m_0* pow(para.c,2);
  gamma_R=w_Kin_R / (para.m_0 * pow(para.c,2))+1;
  beta_R=sqrt(1-1/pow(gamma_R,2));
  u_R=0;
  u_1R = (para.coasting == true) ? 0 : para.u_1base;
  phi_dh2 = 0;
  u_2R = -u_1R/2;
  f_R = beta_R * para.c/para.L;
  f_RF = para.h*f_R;
  omega_R = 2*para.pi * f_R;
  eta_R = -1/pow(gamma_R,2)+1/pow(para.gamma_t,2);
  omega_RF = para.h*omega_R;
  T_R = para.L/beta_R/para.c;
  t_R = 0;
  phi_R = 0;
  p_R = sqrt(pow(gamma_R,2)-1)*para.m_0*para.c;
  B = p_R / para.q/para.r;
  B2 = B;
  BDot = 0;
  particle_Lost = 0;
  f_syn = 1 / T_R * sqrt( para.h * (-eta_R * cos(phi_R)) * para.q * u_1R /(2 * para.pi * w_R * pow(beta_R,2)));
  omega_syn = 2 * para.pi * f_syn;
  Q_syn = omega_syn/omega_R;
  bucket = 0;
  stat_bucket = 0.0;
  stat_beta_R = 0.0;
  stat_gamma_R = 0.0;
  stat_w_R = 0.0;
  stat_u_1R = 0.0;
  stat_eta_R = 0.0;
  stat_omega_R = 0.0;
  stat_save = false;
  UFP = para.pi;
  para.ramp_raise = para.ramp_round1 + ((1.8 - B - para.BDot_max/2*para.ramp_round1_raw - para.BDot_max/2*para.ramp_round2_raw)/4);
  para.ramp_round2 = para.ramp_raise+para.ramp_round2_raw;
}//INIT ENDE




//Debug Ausgabe der intern gespeicherten Werte
void Referenz::ausgabe_debug(){
  qDebug() << "w_Kin, " << w_Kin
           << "\n w_Kin_R, " << w_Kin_R
           << "\n w_R, " << w_R
           << "\n gamma_R, " << gamma_R
           << "\n beta_R, " << beta_R
           << "\n u_1R, " << u_1R
           << "\n phi_dh2, " << phi_dh2
           << "\n u_2R, " << u_2R
           << "\n omega_R, " << omega_R
           << "\n eta_R, " << eta_R
           << "\n omega_RF, " << omega_RF
           << "\n T_R, " << T_R
           << "\n t_R, " << t_R
           << "\n phi_R, " << phi_R
           << "\n p_R, " << p_R
           << "\n B, " << B
           << "\n BDot, " << BDot
           << "\n particle_Lost, " << particle_Lost
           << "\n f_syn, " << f_syn
           << "\n omega_syn, " << omega_syn
           << "\n bucket, " << bucket
           << "\n stat_bucket, " << stat_bucket
           << "\n stat_beta_R, " << stat_beta_R
           << "\n stat_gamma_R, " << stat_gamma_R
           << "\n stat_w_R, " << stat_w_R
           << "\n stat_u_1R, " << stat_u_1R
           << "\n stat_eta_R, " << stat_eta_R
           << "\n stat_omega_R, " << stat_omega_R;
}

//Rampenevent-Funktion
void Referenz::rampenevent(Parameter & para){
  //Aus einem Coasting Beam wird ein gebunchter Beam gemacht, hier wird nur die Spannung hochgefahren
  if (para.coasting == true){
    if(t_R < para.ramp_coast){
      u_1R = para.u_1base*(t_R/para.ramp_coast);
    }
    else {
        u_1R = para.u_1base;
        para.coasting = false;
    };
  };

  if (para.beschleunigung == true){
    if (t_R < para.ramp_static) {BDot = 0;}
    else if(t_R < para.ramp_round1) {BDot = para.BDot_max*(t_R-para.ramp_static)/(para.ramp_round1-para.ramp_static);  }
    else if(t_R < para.ramp_raise) {BDot = para.BDot_max; }
    else if(t_R < para.ramp_round2) {BDot = para.BDot_max*(1-(t_R-para.ramp_raise)/(para.ramp_round2-para.ramp_raise)); }
    else{BDot = 0; }
  }
}
//Rampenfunktion-Ende


//
//
//TRACKING ANFANG
//
//
void Referenz::tracking(Parameter & para, Seperatrix & sep){
  //neue Zeit einstellen
  t_R = t_R+T_R;
  //Rampe
  rampenevent(para);
  u_R = para.L * para.r * BDot;
  B = B + T_R * BDot;

  //Static-Werte speichern
  if (stat_save == false) {
    if (coasting == false) {
      stat_beta_R = beta_R;
      stat_eta_R = eta_R;
      stat_gamma_R = gamma_R;
      stat_omega_R = omega_R;
      stat_u_1R = u_1R;
      stat_w_R = w_R;
      para.ramp_raise = para.ramp_round1+(1.8 - B - (para.ramp_round1_raw + para.ramp_round2_raw) * para.BDot_max *0.5)/para.BDot_max;
      para.ramp_round2 = para.ramp_raise + para.ramp_round2_raw;
      stat_save = true;
    }
  }

  //Magnetfeld != 0, dann verändert sich die Energie des Referenzteilchens und alles andere auch
  if (BDot > 0.0){
    //Anpassung im einfachharmonischen Fall
    if (para.spannungs_Typ == "eh"){
      sep.get_from_ref(this->ref_to_seperatrix());
      sep.set_turbo_length_dynamic_sep();
      u_1R = sep.get_u_1R();
      phi_R = asin(u_R/u_1R);
    }
    //Anpassung im doppeltharmonischen Fall
    if (para.spannungs_Typ == "dh"){
      sep.get_from_ref(this->ref_to_seperatrix());
      sep.set_turbo_length_dynamic_sep();
      u_1R = sep.get_u_1R();
      phi_R = asin(4.0/3.0*u_R/u_1R);
    }
  }
  phi_dh2 = atan(0.5 * tan(phi_R))-2*phi_R;
  u_2R = -u_1R * (cos(phi_R)/(2*cos(2*phi_R+phi_dh2)));

  //Aktualisierung des Referenzteilchens
  w_Kin_R = w_Kin_R + u_R * para.q;
  gamma_R = w_Kin_R / (para.m_0 * pow(para.c,2)) + 1;
  beta_R = sqrt(1 - 1 / pow(gamma_R,2));
  p_R = sqrt(pow(gamma_R,2)-1) * para.m_0*para.c;
  B2 = p_R/(para.q*para.r);
  T_R = para.L / (beta_R * para.c);
  f_R = 1 / T_R;
  f_RF = para.h*f_R;
  omega_R = 2* para.pi* f_R;
  omega_RF =para.h*omega_R;
  eta_R  = -1/pow(gamma_R,2) + 1/pow(para.gamma_t,2);
  w_R = w_Kin_R +para.m_0*pow(para.c,2);
  f_syn = (f_R/beta_R) * sqrt(para.h*(-eta_R)*cos(phi_R)*para.q*u_1R/(2 * para.pi * w_R ));
  omega_syn = 2*para.pi*f_syn;
  Q_syn = omega_syn/omega_R;

}
//
//
//TRACKING ENDE
//
//
