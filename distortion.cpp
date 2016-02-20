#include "distortion.h"
Distortion::Distortion(Parameter & para, log_ref_i & lri, bool homogen_, bool fda_):
  distortion_strength(2*M_PI/360*para.distortion_angle),
  distortion_time_intervall(para.distortion_time_repeat),
  homogen(homogen_),
  fda(fda_),
  distortion_offset(para.distortion_steps_coasting*lri.T_R),
  distortion_end(distortion_offset+0.000250),
  distortion_vector(std::vector<double> (para.h)),
  random_distortion(std::vector<double> (para.h)),
  active(para.distortion_active),
  active_repeat(para.distortion_repeat_active)
{
  std::random_device rd; //initialize random distortion vector
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-1.3*distortion_strength, 1.3*distortion_strength);
  for (unsigned int i = 0; i < random_distortion.size(); i++){
    random_distortion[i] = dis(gen);
  }

  if (fda){
    distortion_offset = lri.t_R;
    distortion_end = distortion_offset + 0.00025;
    if (para.h>1) qDebug() << "not intended to be used as such";
  }
}

void Distortion::distort_homogen(Parameter &para, log_ref_i & lri, std::vector<Bunch> &bunch){
  if (active == true){
    if(para.coasting == true){
      distortion_offset += lri.T_R; //distortion_time after coasting
      distortion_end = distortion_offset + 0.00025; //distortion_end after 250µs
    }
    else if (lri.t_R > distortion_offset && lri.t_R < distortion_end){
      //qDebug() << "distortion"<< lri.t_R << ">" << distortion_offset << "+++" << lri.t_R << "<" << distortion_end << "T_R" << lri.T_R;
      for (unsigned int i = 0; i < distortion_vector.size(); i++){
        distortion_vector[i] = distortion_strength/(distortion_end-distortion_offset)*lri.T_R;
      }
      for (unsigned int i = 0; i< bunch.size();i++){
        bunch[i].phi_manip(distortion_vector);
      }
    }
    else if (lri.t_R > distortion_end && active_repeat){
      qDebug() << "distortion end" << lri.t_R << " > " << distortion_end;
      distortion_offset = distortion_end + distortion_time_intervall/1000000; //next distortion start
      distortion_end = distortion_offset + 0.00025; //nex distortion end
      qDebug() << "set new" << distortion_offset << distortion_end;
    }
  }
}

void Distortion::distort_random(Parameter &para, log_ref_i & lri, std::vector<Bunch> &bunch){
  if (active == true){
    if(para.coasting == true){
      distortion_offset += lri.T_R; //distortion_time after coasting
      distortion_end  = distortion_offset + 0.00025; // distortion_end after 250µs
    }
    else if (lri.t_R > distortion_offset && lri.t_R < distortion_end){
      for (unsigned int i = 0; i < distortion_vector.size(); i++){
        distortion_vector[i] = random_distortion[i]/(distortion_end-distortion_offset)*lri.T_R;
      }
      for (unsigned int i = 0; i< bunch.size();i++){
        bunch[i].phi_manip(distortion_vector);
      }
    }
    else if (lri.t_R > distortion_end && active_repeat){
      distortion_offset += distortion_time_intervall; //next distortion start
      distortion_end += distortion_offset + 0.00025;  //next distortion end
      std::random_device rd; //new random_distortion_values
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> dis(-1.3*distortion_strength, 1.3*distortion_strength);
      for (unsigned int i = 0; i < random_distortion.size(); i++){
        random_distortion[i] = dis(gen);
      }
    }
  }
}

void Distortion::distort_homogen_fda(log_ref_i & lri, std::vector<Bunch> & bunch){
  if (lri.t_R > distortion_offset && lri.t_R < distortion_end){
    for (unsigned int i = 0; i < distortion_vector.size(); i++){
      distortion_vector[i] = distortion_strength/(distortion_end-distortion_offset)*lri.T_R;
      //qDebug() << i << distortion_vector[i] << "dist";
    }
    for (unsigned int i = 0; i< bunch.size();i++){
      bunch[i].phi_manip(distortion_vector);
    }
  }
}

void Distortion::process_distortion(Parameter & para, log_ref_i & lri, std::vector<Bunch> & bunch){
  if (fda){
    distort_homogen_fda(lri,bunch);
  }
  else if (homogen){
    distort_homogen(para,lri,bunch);
  }
  else{
    distort_random(para,lri,bunch);
  }
}
