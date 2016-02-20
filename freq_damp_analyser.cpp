#include "freq_damp_analyser.h"
#include "ui_freq_damp_analyser.h"

//Constructor
freq_Damp_Analyser::freq_Damp_Analyser(Parameter & para_, Log_Reference & log_ref_, const std::vector<Bunch> & bunch_, int begin_, save_Results * save_) :
  QWidget(nullptr),
  log_ref(log_ref_),
  para(para_),
  begin(begin_),
  end_max(log_ref.get_B().size()),
  save(save_),
  ui(new Ui::freq_Damp_Analyser),
  maxima_evaluate(0),
  minima_evaluate(0),
  frequency(0),
  frequency_time(0)
{
  ui->setupUi(this);
  bunch_vec.reserve(bunch_.size());
  copy(bunch_.begin(),bunch_.end(),std::back_inserter(bunch_vec));
  log_ref.set_log_ref_i(lri,begin);
  dist = new Distortion(para,lri,true,true);
}

freq_Damp_Analyser::~freq_Damp_Analyser()
{
  delete ui;
}

//
//local tracking, just the data acquisition
//
void freq_Damp_Analyser::local_tracking()
{
  //temporary variables and break conditioners
  bool flanc_rise = true;
  double extremum = 0.0;
  double extremum_time = 0.0;
  int extremum_counter = 0;
  double rising_slope = 0.0;
  double rising_slope_last = 0.0;
  double falling_slope = 0.0;
  double falling_slope_last = 0.0;
  bool write_data_pos = false;
  bool write_data_neg = false;
  bool continue_tracking = true;

  std::vector<double> u = std::vector<double> (bunch_vec.size(),0.0);

  int i=begin;

  while(i < end_max && continue_tracking)
  {
    double com = 0; //temporary variable for center of mass
    log_ref.set_log_ref_i(lri,i);
    //Bunch-tracking with induced distortion
    for (unsigned int j = 0; j< bunch_vec.size(); j++){
      bunch_vec[j].tracking(para,lri,u);
      bunch_vec[j].moment_actualisation(para);
      com += bunch_vec[j].get_bunch_mp1()[0];
    }
    phi.push_back(com/bunch_vec.size()-lri.phi_1R); //correction of phi_1R, so that bunch is nearly centered around 0
    time.push_back(lri.t_R);
    dist->process_distortion(para,lri,bunch_vec);

    //while flanc_rise, get new maximum values, until ten consecutive times
    //lower extremum values occur, switch to get new minimum values and everything in the opposite
    if (flanc_rise){
      if(phi.back() > extremum){
        extremum = phi.back();
        extremum_time = lri.t_R;
        extremum_counter = 0;
      }
      else{
        extremum_counter += 1;
        if (extremum_counter > 9){
          flanc_rise = false;
          extremum_counter = 0;
          maxima.push_back(extremum);
          maxima_time.push_back(extremum_time);
          if (maxima.size() > 2){
            int j = maxima.size()-1;
            rising_slope = (maxima[j]-maxima[j-1])/(maxima_time[j]-maxima_time[j-1]);
            rising_slope_last = (maxima[j-1]-maxima[j-2])/(maxima_time[j-1]-maxima_time[j-2]);

            //set points for evaluation;
            if (write_data_pos == false && rising_slope_last < rising_slope  && rising_slope_last < 0){
              maxima_evaluate = maxima.size()-3;
              qDebug() << maxima_evaluate << "maxima_evaluate";
              write_data_pos = true;
            }
            //break condition, when rising slope goes to positive values, doesnt belong
            //to monoton falling exponential decay
            if (write_data_pos == true && rising_slope >0){
              continue_tracking = false;
            }
          }
        }
      }
    }
    else{
      if(phi.back() < extremum){
        extremum = phi.back();
        extremum_time = lri.t_R;
        extremum_counter = 0;
      }
      else{
        extremum_counter += 1;
        if (extremum_counter > 9){
          flanc_rise = true;
          extremum_counter = 0;
          minima.push_back(extremum);
          minima_time.push_back(extremum_time);
          if (minima.size() > 2){
            int j = minima.size()-1;
            falling_slope = (minima[j]-minima[j-1])/(minima_time[j]-minima_time[j-1]);
            falling_slope_last = (minima[j-1]-minima[j-2])/(minima_time[j-1]-minima_time[j-2]);

            //set points for evaluation;
            if (write_data_neg == false && falling_slope_last > falling_slope && falling_slope_last > 0){
              minima_evaluate = minima.size()-3;
              qDebug() << minima_evaluate << "minimum evaluate";
              write_data_neg = true;
            }
            //break condition, when rising slope goes to positive values, doesnt belong
            //to monoton falling exponential decay
            if (write_data_neg == true && falling_slope < 0){
              continue_tracking = false;
            }
          }
        }
      }
    }
    if (maxima.size()-maxima_evaluate > 40 | minima.size()-minima_evaluate > 40) continue_tracking = false;
    i=i+1;
  }

  for (unsigned int i = 1; i< maxima.size(); i++){
    qDebug() << "Maximum" << maxima[i-1] << maxima_time[i-1] << "Minima" << minima[i-1] << minima_time[i-1];
  }
  for (int j = 0; j < maxima_evaluate; j++){
    maxima.pop_front();
    maxima_time.pop_front();
  }
  for (int j = 0; j < minima_evaluate; j++){
    minima.pop_front();
    minima_time.pop_front();
  }

  for (int j = 0; j < maxima.size(); j++){
    qDebug() << "maxima_ausgabe" << maxima_time[j] << maxima[j];
  }

  expo = new Exponential_Fitter(maxima_time,maxima);
  expu = new Exponential_Fitter(minima_time,minima);

  calculate_frequency();

  show_results();
  //lri
  //QString text = QString::number(log_ref[begin].f_syn) << QString::number(log_ref[begin].f_syn) << QString::number() << QString::number();
  //save->add_actual_state();
}

//
//show_results is a debugging tool, to see, if the data acquistion is ok.
//
void freq_Damp_Analyser::show_results(){
  ownPlot * op = new ownPlot(this,50,50,800,300);
  ownPlot * op2 = new ownPlot(this,50,400,800,300);
  op->plotXY(time,phi);
  op->plotExp(expo->get_A(),expo->get_lambda(),expo->get_b(),maxima_time.first(), maxima_time.last(),maxima.size());
  op->plotExp(expu->get_A(),expu->get_lambda(),expu->get_b(),minima_time.first(), minima_time.last(),minima.size());
  op2->plotXY(frequency_time,frequency);
  op2->plotXY2(log_ref.get_t_R().mid(begin,begin+frequency.length()),log_ref.get_f_syn().mid(begin,begin+frequency.length()));

  this->show();
}

//
//function for calculating the frequency and possible frequency shift, when the bunch is damped
//
void freq_Damp_Analyser::calculate_frequency(){
  for (int i = 0; i < maxima.size()-1; i++){
    frequency.push_back(1/(maxima_time[i+1]-maxima_time[i]));
    frequency_time.push_back((maxima_time[i+1]+maxima_time[i])/2);
  }
  for (int i = 0; i < minima.size()-1; i++){
    frequency.push_back(1/(minima_time[i+1]-minima_time[i]));
    frequency_time.push_back((minima_time[i+1]+minima_time[i])/2);
  }
}
