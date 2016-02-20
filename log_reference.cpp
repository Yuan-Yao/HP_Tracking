#include "log_reference.h"

Log_Reference::Log_Reference(int i):
  t_R(std::vector<double>(i+1)),
  T_R(std::vector<double>(i+1)),
  u_R(std::vector<double>(i+1)),
  u_1R(std::vector<double>(i+1)),
  u_2R(std::vector<double>(i+1)),
  phi_1R(std::vector<double>(i+1)),
  phi_2R(std::vector<double>(i+1)),
  f_R(std::vector<double>(i+1)),
  omega_R(std::vector<double>(i+1)),
  f_syn(std::vector<double>(i+1)),
  omega_syn(std::vector<double>(i+1)),
  w_R(std::vector<double>(i+1)),
  BDot(std::vector<double>(i+1)),
  B(std::vector<double>(i+1)),
  beta_R(std::vector<double>(i+1)),
  eta_R(std::vector<double>(i+1)),
  bucket(std::vector<double>(i+1)),
  UFP(std::vector<double>(i+1))
{
}

void Log_Reference::save_stat(int i,Referenz & ref, Seperatrix & sep){
  t_R[i] = ref.t_R;
  T_R[i] = ref.T_R;
  u_R[i] = ref.u_R;
  u_1R [i] = ref.u_1R;
  u_2R [i] = ref.u_2R;
  phi_1R [i] = ref.phi_R;
  phi_2R [i] = ref.phi_dh2;
  f_R[i] = ref.f_R;
  omega_R[i] = ref.omega_R;
  f_syn[i] = ref.f_syn;
  omega_syn [i]  = ref.omega_syn;
  w_R [i] = ref.w_R;
  BDot [i] = ref.BDot;
  B[i] = ref.B;
  beta_R[i] = ref.beta_R;
  eta_R[i] = ref.eta_R;
  bucket[i] = sep.bucket;
  UFP[i] = ref.UFP;
}

void Log_Reference::set_log_ref_i(log_ref_i & lri, int i){
  lri.t_R = t_R[i];
  lri.T_R = T_R[i];
  lri.u_R = u_R[i];
  lri.u_1R = u_1R[i];
  lri.u_2R = u_2R[i];
  lri.phi_1R = phi_1R[i];
  lri.phi_2R = phi_2R[i];
  lri.f_R = f_R[i];
  lri.omega_R = omega_R[i];
  lri.f_syn = f_syn[i];
  lri.omega_syn = omega_syn[i];
  lri.w_R = w_R[i];
  lri.BDot = BDot[i];
  lri.B = B[i];
  lri.beta_R = beta_R[i];
  lri.eta_R = eta_R[i];
  lri.bucket = bucket[i];
  lri.UFP = UFP[i];
}

void Log_Reference::show_stat(int i){
  qDebug() << "t_R " << t_R[i] << " T_R " << T_R[i] << " u_R " << u_R[i] << " u_1R " << u_1R[i] << " u_2R " << u_2R [i]
              << " phi_1R " << phi_1R[i] << " phi_2R " << phi_2R[i] << "f_R"<< f_R[i] <<" omega_R " << omega_R[i] << "f_syn"<< f_syn[i] <<" omega_syn " << omega_syn[i]
                 << " w_R " << w_R[i] << " BDot " << BDot[i]
                    << " B " << B[i] << " beta_R " << beta_R[i] << " eta_R " << eta_R[i] << " bucket " << bucket[i] << " UFP " << UFP[i] << " u_R real" << (u_1R[i]*sin(phi_1R[i])+u_2R[i]*sin(2*phi_1R[i]+phi_2R[i])) ;
}
