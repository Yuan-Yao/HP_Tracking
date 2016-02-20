#include "own_math.h"

namespace own_math2
{

  std::vector<double> reduc_vec(std::vector<double> & iv, int begin, int end){
    std::vector<double>::const_iterator first = iv.begin() + begin;
    std::vector<double>::const_iterator last = iv.begin() + end;
    std::vector<double> temp = std::vector<double>(first,last);
    return (temp);
  }

  double mean(std::vector<double> & iv){
    double temp = 0;
    for (unsigned int j = 0; j < iv.size() ; j++){
      temp = temp+iv[j];
    }
    temp = temp/iv.size();
    return (temp);
  }

  double standard_deviation(std::vector<double> & iv){
    double temp_mean = own_math2::mean(iv);
    double temp = 0.0;
    for (unsigned int i = 0; i < iv.size(); i++){
      temp = temp + pow(iv[i]-temp_mean,2);
    }
    temp = sqrt(1/(0.0+iv.size()-1))*sqrt(temp);
    return temp;
  }

  double DFT_1_rad(std::vector<double> & iv){
    double tempr = 0.0;
    double tempp = 0.0;
    for (unsigned int i = 0; i < iv.size(); i++){
      tempr += cos(2*M_PI/iv.size()*i)*iv[i];
      tempp += sin(2*M_PI/iv.size()*i)*iv[i];
    }
    tempr = tempr/iv.size();
    tempp = tempp/iv.size();
    return 2*sqrt(pow(tempr,2)+pow(tempp,2));
  }
  double DFT_1_phase(std::vector<double> & iv){
    double tempr = 0.0;
    double tempp = 0.0;
    for (unsigned int i = 0; i < iv.size(); i++){
      tempr += cos(2*M_PI/iv.size()*i)*iv[i];
      tempp += sin(2*M_PI/iv.size()*i)*iv[i];
    }
    tempr = tempr/iv.size();
    tempp = tempp/iv.size();
    double angle = atan(fabs(tempp)/fabs(tempr));
    if (tempp < 0) angle = M_PI-angle;
    if (tempr < 0) angle = angle+M_PI;
    return angle;
  }

  std::vector<double> energy(std::vector<double> & iv, double frequency, double Ts){
    std::vector<double> back = std::vector<double>(3,0);
    if (iv.size() == 9)
    {
      back[0] = 0.377778 * iv[0] + 0.31111*iv[1] + 0.244444*iv[2] + 0.177778*iv[3] + 0.11111*iv[4]+ 0.044444 *iv[5] -0.0222222 * iv[6] -0.0888889*iv[7] - 0.155556 * iv[8];
      back[1] = (-0.0666667 * iv[0] - 0.05 * iv[1] - 0.03333333*iv[2] - 0.0166667*iv[3] + 0.0166667 *iv[5] +0.03333333 * iv[6] + 0.05*iv[7] + 0.066667 * iv[8]);
      back[0] = back[0] + 4*back[1];
      back[1] = back[1]/Ts;
      back[2] = pow(back[0],2) + pow(back[1]/(2*0.95*M_PI*frequency),2);
    }
    return back;
  }

}
