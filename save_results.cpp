#include "save_results.h"

save_Results::save_Results(Parameter & para_)
{
  file = new QFile();
  bool exists = true;
  int i=0;

  //check if folder results exists
  if (!dir.exists("results")) dir.mkdir("results");
  dir.setCurrent("results");

  bool already_exists = false;
  filename = "result_0" + QString::number(1) + ".txt";
  file->setFileName(filename);
  //bool already_exists = true;
  i = 1;
  while (already_exists == true){
    if (i<10) filename = "result_0" + QString::number(i) + ".txt";
    if (i>9) filename = "result_" + QString::number(i) +".txt";
    file->setFileName(filename);
    if (!file->exists()) already_exists = false;
    i++;
  }

  file->open(QIODevice::WriteOnly);
  textStream = new QTextStream(file);

  *textStream << "Anzahl Teilchen:" << para_.anz_Teilchen << "\nAnzahl Umdrehungen:" << para_.anz_Umdrehungen << "\nBDot_max:"
              << para_.BDot_max<< "\nBeschleunigung:" << para_.beschleunigung << "\nCoasting:" << para_.coasting << "\nDistortion_active:"
              << para_.distortion_active << "\nDistortion_angle:" << para_.distortion_angle << "\nDistortion_repeat_active:" << para_.distortion_repeat_active
              << "\nDistortion_steps_coasting:" << para_.distortion_steps_coasting << "\nDistortion_time_repeat:" << para_.distortion_time_repeat
              << "\nFilter_type:" << para_.filter_Typ << "\nFrequenz_modification:" << para_.freq_mod << "\nFuellgrad:" << para_.fuellgrad
              << "\nHarmonic_number:" << para_.h << "\nHistogram_resolution:" << para_.histo_anz << "\nIon_Type:" << para_.ionen_Sorte
              << "\nRing_Length:" << para_.L << "\nNoise:" << para_.noise << "\nNoise_power:" << para_.noise_power << "\nInitial_nucleon_energy:"
              << para_.nukleonen_Energie << "\nController_type:" << para_.reglertyp << "\nSave_gap:" << para_.save_Gap
              << "\nVoltage_type:" << para_.spannungs_Typ << "\nBase_voltage_1:" << para_.u_1base << "\nBase_voltage_2:" << para_.u_2base << "\n";
  *textStream << "f_syn_theo    f_syn_real    lambda    damping\n";
  file->close();

  dir.cdUp();
  dir.setCurrent(dir.path());
}

void save_Results::add_actual_state(QString string_){
  file->open(QIODevice::Append);
  *textStream << string_;
  file->close();
}
