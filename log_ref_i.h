#ifndef LOG_REF_I_H
#define LOG_REF_I_H
#include <qdebug.h>

struct log_ref_i
{
   // all log_reference variables
   double t_R;
   double T_R;
   double u_R;
   double u_1R;
   double u_2R;
   double phi_1R;
   double phi_2R;
   double f_R;
   double omega_R;
   double f_syn;
   double omega_syn;
   double w_R;
   double BDot;
   double B;
   double beta_R;
   double eta_R;
   double bucket;
   double UFP;

   log_ref_i(){}
   void show(){
     qDebug() << t_R << T_R << u_R << u_1R << u_2R << phi_1R << phi_2R << f_R << omega_R << f_syn << omega_syn << w_R << BDot << B << beta_R << eta_R << bucket << UFP;}
};


#endif // LOG_REF_I_H
