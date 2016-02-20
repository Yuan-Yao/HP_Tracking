#include "seperatrix.h"

Seperatrix::Seperatrix(Parameter & para, int standard_length, int turbo_length):
  q(para.q),
  pi(para.pi),
  h(para.h),
  h_pow(3),
  spannungstyp(para.spannungs_Typ),
  u_1R_base(para.u_1base),
  u_2R_base(para.u_2base),
  var(0.5),
  phi_ring_sequence(std::vector<double>(standard_length)),
  phi_ring_sequence_turbo(std::vector<double>(turbo_length)),
  seperatrix_pos(std::vector<double>(standard_length)),
  seperatrix_neg(std::vector<double>(standard_length)),
  seperatrix_turbo(std::vector<double>(turbo_length)),
  seperatrix_08(std::vector<double>(standard_length)),
  seperatrix_06(std::vector<double>(standard_length)),
  seperatrix_04(std::vector<double>(standard_length)),
  seperatrix_02(std::vector<double>(standard_length)),
  seperatrix_var(std::vector<double>(standard_length)),
  stat_seperatrix_pos(std::vector<double>(standard_length)),
  stat_seperatrix_neg(std::vector<double>(standard_length)),
  voltage(std::vector<double>(standard_length)),
  turbo_front(-M_PI),
  turbo_back(M_PI)
{
  for(int i = 0; i< standard_length; i++) phi_ring_sequence[i] = -para.pi +2*para.pi/(standard_length-1)*i*h;
  for(int i = 0; i< turbo_length; i++) phi_ring_sequence_turbo[i] = -para.pi +2*para.pi/(turbo_length-1)*(i);
}

//
//Synchrotron_Referenzwerte werden an die Seperatrix-Klasse übertragen
//
void Seperatrix::get_from_ref(std::tuple<double,double,double,double,double,double,double,double,double,double> ref_tup){
   u_1R = std::get<0>(ref_tup);
   UFP = std::get<1>(ref_tup);
   u_R = std::get<2>(ref_tup);
   beta_R = std::get<3>(ref_tup);
   w_R = std::get<4>(ref_tup);
   eta_R = std::get<5>(ref_tup);
   omega_R = std::get<6>(ref_tup);
   phi_R = std::get<7>(ref_tup);
   phi_dh2 = std::get<8>(ref_tup);
   u_2R = std::get<9>(ref_tup);
}

//
//Synchrotron reference values from actual turn will be given to seperatrix to calculate dense seperatrix which is to be saved with a bunch
//
void Seperatrix::get_from_lri(log_ref_i & lri){
  u_1R = lri.u_1R;
  UFP = lri.UFP;
  u_R = lri.u_R;
  beta_R = lri.beta_R;
  w_R = lri.w_R;
  eta_R = lri.eta_R;
  omega_R = lri.omega_R;
  phi_R = lri.phi_1R;
  phi_dh2 = lri.phi_2R;
  u_2R = lri.u_2R;
}

//
//Variable Seperatrixberechnung
//
void Seperatrix::set_var_seperatrix(std::vector<double> & sep,double reduk){
  if (spannungstyp == "eh"){
    double sepfactor = - pow(beta_R,2) * w_R * q * u_1R / (pi * pow(h,h_pow) * eta_R* pow(omega_R,2));
    double help1 = sin(phi_R);
    double sepfactorenh = sepfactor*(cos(phi_R)-(pi-phi_R)*help1 - (2.0*reduk - pow(reduk,2))*(2.0*cos(phi_R)-(pi-2.0*phi_R)*help1));
    double help2 = pi - phi_R;
    for (unsigned int i = 0; i< phi_ring_sequence.size();i++){
      if (phi_ring_sequence[i] > help2) {
        sep[i] =0;}
      else {
      sep[i] = sepfactorenh + sepfactor * (cos(phi_ring_sequence[i])+help1*phi_ring_sequence[i]);
      sep[i] = (sep[i]>0) ? sqrt(sep[i]) : 0;
      }
    }
  }
  else if (spannungstyp == "dh"){
    double temp = pi;
    for(int i = 0; i < 10; i++){
      temp = temp - (u_1R * sin(temp) + u_2R*(sin(2*temp+phi_dh2))-u_R)/(u_1R * cos(temp) + u_2R*(2.0*cos(2.0*temp+phi_dh2)));
    }
    UFP = temp;
    //if(coasting == true) UFP = para.pi;
    double sepfactor = 2.0 * pow(beta_R,2.0) * w_R / (pow(h,h_pow)* pow(omega_R,2.0) * eta_R);
    double Hsx = q/(2.0*pi) * (u_1R*(cos(UFP)-cos(phi_R)+(UFP-phi_R)*sin(phi_R))+u_2R*(0.5*cos(2.0*UFP+phi_dh2)-0.5*cos(2.0*phi_R+phi_dh2)+(UFP-phi_R)*sin(2.0*phi_R+phi_dh2)));
    for (unsigned int i = 0; i< phi_ring_sequence.size();i++){
      if (phi_ring_sequence[i] > UFP) {
        sep[i] =0;}
      else {
      sep[i] = sepfactor * (Hsx*(1.0-2.0*reduk+pow(reduk,2))-q/(2*pi)*(u_1R*(cos(phi_ring_sequence[i])-cos(phi_R)+(phi_ring_sequence[i]-phi_R)*sin(phi_R))+u_2R*(0.5*cos(2.0*phi_ring_sequence[i]+phi_dh2)-0.5*cos(2.0*phi_R+phi_dh2)+(phi_ring_sequence[i]-phi_R)*sin(2.0*phi_R+phi_dh2))));
      sep[i] = (sep[i]>0) ? sqrt(sep[i]) : 0;
      }
    }
  }
}

//
//Berechnung aller Seperatizen, mit unvoller Länge
//
void Seperatrix::set_all_small_seperatrix(){
  set_var_seperatrix(seperatrix_var,var);
  set_var_seperatrix(seperatrix_08,0.8);
  set_var_seperatrix(seperatrix_08,0.6);
  set_var_seperatrix(seperatrix_08,0.4);
  set_var_seperatrix(seperatrix_08,0.2);
}

//
//statische Seperatrixberechnung; Das findet nur einmal statt am Anfang der Simulation + Voltage
//
void Seperatrix::set_static_seperatrix(){
  stat_max_sep = 0.0;
  if (spannungstyp == "eh"){
    double sepfactor = - pow(beta_R,2) * w_R * q * u_1R_base / (pi * pow(h,h_pow) * eta_R* pow(omega_R,2));
    stat_sepfactor = sepfactor;
    double help1 = sin(phi_R);
    double sepfactorenh = sepfactor*(cos(phi_R)-(pi-phi_R)*help1);
    for (unsigned int i = 0; i< phi_ring_sequence.size();i++){
      stat_seperatrix_pos[i] = sepfactorenh + sepfactor * (cos(phi_ring_sequence[i])+help1*phi_ring_sequence[i]);
      stat_seperatrix_pos[i] = (stat_seperatrix_pos[i]>0) ? sqrt(stat_seperatrix_pos[i]) : 0;
      if (stat_seperatrix_pos[i] > stat_max_sep) stat_max_sep = stat_seperatrix_pos[i];
      stat_seperatrix_neg[i] = -stat_seperatrix_pos[i];

    }
  }
  else if (spannungstyp == "dh"){
    double sepfactor = 2 * pow(beta_R,2) * w_R / (pow(h,h_pow)* pow(omega_R,2) * eta_R);
    double Hsx = q/(2*pi) * (u_1R_base*(cos(UFP)-cos(phi_R)+(UFP-phi_R)*sin(phi_R))+u_2R_base*(0.5*cos(2*UFP)-0.5*cos(2*phi_R+phi_dh2)+(UFP-phi_R)*sin(2*phi_R+phi_dh2)));
    for (unsigned int i = 0; i< phi_ring_sequence.size();i++){
      stat_seperatrix_pos[i] = sepfactor * (Hsx-q/(2*pi)*(u_1R_base*(cos(phi_ring_sequence[i])-cos(phi_R)+(phi_ring_sequence[i]-phi_R)*sin(phi_R))+u_2R_base*(0.5*cos(2*phi_ring_sequence[i]+phi_dh2)-0.5*cos(2*phi_R+phi_dh2)+(phi_ring_sequence[i]-phi_R)*sin(2*phi_R+phi_dh2))));
      stat_seperatrix_pos[i] = (stat_seperatrix_pos[i]>0) ? sqrt(stat_seperatrix_pos[i]) : 0;
      if (stat_seperatrix_pos[i] > stat_max_sep) stat_max_sep = stat_seperatrix_pos[i];
      stat_seperatrix_neg[i] = -stat_seperatrix_pos[i];
    }
  }

  stat_bucket = sum_seperatrix(stat_seperatrix_pos);
}

//
//Seperatrix Berechnung mit Standard_Länge, zur Speicherung in Log_bunch
//
void Seperatrix::set_full_length_dynamic_sep(double reduk){
  int iterator = phi_ring_sequence.size()/h;
  if (spannungstyp == "eh"){
    double sepfactor = - pow(beta_R,2) * w_R * q * u_1R / (pi * pow(h,h_pow) * eta_R* pow(omega_R,2));
    double help1 = sin(phi_R);
    double sepfactorenh = sepfactor*(cos(phi_R)-(pi-phi_R)*help1 - (2.0*reduk - pow(reduk,2))*(2.0*cos(phi_R)-(pi-2.0*phi_R)*help1));
    double help2 = pi - phi_R;
    for (int i = 0; i < iterator;i++){
      if (phi_ring_sequence[i] > help2) {
        seperatrix_pos[i] =0;}
      else {
        seperatrix_pos[i] = sepfactorenh + sepfactor * (cos(phi_ring_sequence[i])+help1*phi_ring_sequence[i]);
        seperatrix_pos[i] = (seperatrix_pos[i]>0) ? sqrt(seperatrix_pos[i]) : 0;
        seperatrix_neg[i] = -seperatrix_pos[i];
        for (int j = 1; j < h; j++){
          seperatrix_pos[i+j*iterator] = seperatrix_pos[i];
          seperatrix_neg[i+j*iterator] = seperatrix_neg[i];
        }
      }
    }
  }
  else if (spannungstyp == "dh"){
    double temp = pi;
    for(int i = 0; i < 10; i++){
      temp = temp - (u_1R * sin(temp) + u_2R*(sin(2*temp+phi_dh2))-u_R)/(u_1R * cos(temp) + u_2R*(2.0*cos(2.0*temp+phi_dh2)));
    }
    UFP = temp;
    //if(coasting == true) UFP = para.pi;
    double sepfactor = 2.0 * pow(beta_R,2.0) * w_R / (pow(h,h_pow)* pow(omega_R,2.0) * eta_R);
    double Hsx = q/(2.0*pi) * (u_1R*(cos(UFP)-cos(phi_R)+(UFP-phi_R)*sin(phi_R))+u_2R*(0.5*cos(2.0*UFP+phi_dh2)-0.5*cos(2.0*phi_R+phi_dh2)+(UFP-phi_R)*sin(2.0*phi_R+phi_dh2)));
    for (int i = 0; i< iterator;i++){
      if (phi_ring_sequence[i] > UFP) {
        seperatrix_pos[i] =0;}
      else {
      seperatrix_pos[i] = sepfactor * (Hsx*(1.0-2.0*reduk+pow(reduk,2))-q/(2*pi)*(u_1R*(cos(phi_ring_sequence[i])-cos(phi_R)+(phi_ring_sequence[i]-phi_R)*sin(phi_R))+u_2R*(0.5*cos(2.0*phi_ring_sequence[i]+phi_dh2)-0.5*cos(2.0*phi_R+phi_dh2)+(phi_ring_sequence[i]-phi_R)*sin(2.0*phi_R+phi_dh2))));
      seperatrix_pos[i] = (seperatrix_pos[i]>0) ? sqrt(seperatrix_pos[i]) : 0;
      }
      seperatrix_neg[i] = -seperatrix_pos[i];
      for (int j = 1; j < h; j++){
        seperatrix_pos[i+j*iterator] = seperatrix_pos[i];
        seperatrix_neg[i+j*iterator] = seperatrix_neg[i];
      }
    }
  }
  bucket = sum_seperatrix(seperatrix_pos)/stat_bucket;
}

//
//Seperatrix Berechnung mit Turbo_Länge, zur Anpassung der Spannung, damit die Bucketgröße einigermaßen gleich groß bleibt
//
void Seperatrix::set_turbo_length_dynamic_sep(){
  bool set_turbo_front = true;
  bool set_turbo_back = true;
  double turbo_intervall = turbo_back - turbo_front;
  for(int i = 0; i< phi_ring_sequence_turbo.size(); i++) phi_ring_sequence_turbo[i] = turbo_front + (turbo_intervall)/(phi_ring_sequence_turbo.size()-1)*i;
  qDebug() << turbo_intervall << phi_ring_sequence_turbo[0] << phi_ring_sequence_turbo[phi_ring_sequence_turbo.size()-1];
  double u = u_1R;
  double temp_u = u_1R;
  double UFP_neu = UFP;
  double temp_UFP;
  double abstand = 1;
  double temp;

  for(int k = -4; k<4; k++){
    if (spannungstyp == "eh"){
     u= u_1R + k;
      double phi = asin(u_R/u);
      double sepfactor = - pow(beta_R,2) * w_R * q * u / (pi * pow(h,h_pow) * eta_R* pow(omega_R,2));
      double help1 = sin(phi);
      double sepfactorenh = sepfactor*(cos(phi)-(pi-phi)*help1);
      double help2 = pi - phi;
      for (unsigned int i = 0; i< phi_ring_sequence_turbo.size();i++){
        if (phi_ring_sequence_turbo[i] > help2) {
          seperatrix_turbo[i] =0;
          if (set_turbo_back == true) {set_turbo_back = false; turbo_back = (help2 + M_PI/18 > M_PI) ? M_PI : help2 + M_PI/18;}
        }
        else {
        seperatrix_turbo[i] = sepfactorenh + sepfactor * (cos(phi_ring_sequence_turbo[i])+help1*phi_ring_sequence_turbo[i]);
        seperatrix_turbo[i] = (seperatrix_turbo[i]>0) ? sqrt(seperatrix_turbo[i]) : 0;
        if (set_turbo_front == true && seperatrix_turbo[i] > 0) {set_turbo_front = false; turbo_front = (phi_ring_sequence_turbo[i]-M_PI/18 < -M_PI)? -M_PI : phi_ring_sequence_turbo[i]-M_PI/18 ;}
        }
      }
    }
    else if (spannungstyp == "dh"){
      u = u_1R + k;
      double phi = asin(4.0/3.0*u_R/u);
      double temp_phi_dh2 = atan(0.5*tan(phi))-2*phi;
      double temp_u_2R = -u*(cos(phi)/(2*cos(2*phi+temp_phi_dh2)));
      temp_UFP = pi;

      for(int i = 0; i < 10; i++){
        temp_UFP = temp_UFP - (u * sin(temp_UFP) + temp_u_2R*(sin(2*temp_UFP+temp_phi_dh2))-u_R)/(u * cos(temp_UFP) + temp_u_2R*(2*cos(2*temp_UFP+temp_phi_dh2)));
      }
      //if(coasting == true) temp_UFP = pi;
      double sepfactor = 2 * pow(beta_R,2) * w_R / (pow(h,h_pow)* pow(omega_R,2) * eta_R);
      double Hsx = q/(2*pi) * (u*(cos(temp_UFP)-cos(phi)+(temp_UFP-phi)*sin(phi))+temp_u_2R*(0.5*cos(2*temp_UFP+temp_phi_dh2)-0.5*cos(2*phi+temp_phi_dh2)+(temp_UFP-phi)*sin(2*phi+temp_phi_dh2)));
      for (unsigned int i = 0; i< phi_ring_sequence_turbo.size();i++){
        if (phi_ring_sequence_turbo[i] > UFP) {
          seperatrix_turbo[i] =0;
        }
        else {
        seperatrix_turbo[i] = sepfactor * (Hsx-q/(2*pi)*(u*(cos(phi_ring_sequence_turbo[i])-cos(phi)+(phi_ring_sequence_turbo[i]-phi)*sin(phi))+temp_u_2R*(0.5*cos(2*phi_ring_sequence_turbo[i]+temp_phi_dh2)-0.5*cos(2*phi+temp_phi_dh2)+(phi_ring_sequence_turbo[i]-phi)*sin(2*phi+temp_phi_dh2))));
        seperatrix_turbo[i] = (seperatrix_turbo[i]>0) ? sqrt(seperatrix_turbo[i]) : 0;
        }
      }
    }
    temp = sum_seperatrix(seperatrix_turbo)/2/M_PI*turbo_intervall;
    if(fabs(temp - stat_bucket) < abstand) {temp_u = u; abstand = fabs(temp -stat_bucket); UFP_neu = temp_UFP;}
  }
  u_1R = temp_u;
}

void Seperatrix::set_voltage(std::vector<double> u){
  int counter = voltage.size();
  int divider = counter / h;
  if (spannungstyp == "eh"){
    for (int i = 0; i < counter; i++){
      voltage[i] = sin(phi_ring_sequence[i] - u[(int) (i/divider)])*stat_max_sep;
    }
  }
  else if (spannungstyp == "dh"){
    for (int i = 0; i < counter; i++){
      voltage[i] = (u_1R*sin(phi_ring_sequence[i] - u[(int) (i/divider)])+u_2R*sin(2*phi_ring_sequence[i]-2*u[(int) (i/divider)]+phi_dh2))*stat_max_sep/(u_1R-u_2R);
    }
  }
}

double Seperatrix::sum_seperatrix(std::vector<double> & sep){
  double speicher = 0.0;
  for (unsigned int i = 1; i< sep.size();i++){
    speicher += (sep[i]+sep[i-1])/2;
  }
  speicher = speicher/(sep.size()-1);
  return speicher;
}

void Seperatrix::integrator_debug(){
  double integ_stat = 0;
  double integ_turbo = 0;
  double integ_anzeige = 0;
  for (int i = 1; i< stat_seperatrix_pos.size(); i++){
    integ_stat += (stat_seperatrix_pos[i]+stat_seperatrix_pos[i-1])/2/(seperatrix_turbo.size()-1);
    integ_turbo += (seperatrix_turbo[i]+seperatrix_turbo[i-1])/2/(seperatrix_turbo.size()-1);
    integ_anzeige += (seperatrix_pos[i]+seperatrix_pos[i-1])/2/(seperatrix_pos.size()-1);
    qDebug() << "Integrator:" << i << integ_stat << integ_turbo << integ_anzeige << (stat_seperatrix_pos[i]+stat_seperatrix_pos[i-1])/2/(seperatrix_turbo.size()-1)<< (seperatrix_turbo[i]+seperatrix_turbo[i-1])/2/(seperatrix_turbo.size()-1) << (seperatrix_pos[i]+seperatrix_pos[i-1])/2/(seperatrix_pos.size()-1);
  }
}

QVector<double> Seperatrix::get_sep_difference(QVector<double> inputvector){
  QVector<double> temp = QVector<double>(stat_seperatrix_pos.size());
  for (unsigned int i = 0; i < stat_seperatrix_pos.size(); i++){
    temp[i] = stat_seperatrix_pos[i] - inputvector[i];
  }
  return temp;
}
