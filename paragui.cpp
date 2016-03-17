//define get- and set-funcs in details
#include "paragui.h"
#include "ui_paragui.h"

ParaGUI::ParaGUI(QWidget *parent)
    :QWidget(parent),ui(new Ui::ParaGUI)
{
  ui->setupUi(this);
}

ParaGUI::~ParaGUI()
{
  delete ui;
}

//define functions in detail in GUI which read out variales from the GUI
bool ParaGUI::get_acc(){return ui->beschleunigung->isChecked();}
int ParaGUI::get_anz_um(){return ui->anz_um->value();}
int ParaGUI::get_anz_teil(){return ui->anz_teil->value();}
int ParaGUI::get_anz_threads(){return ui->anz_threads->value();}
bool ParaGUI::get_coasting(){return ui->coasting->isChecked();}
int ParaGUI::get_hist_anz(){return ui->hist_anz->value();}
QString ParaGUI::get_ion_sort(){return ui->ionen_sorte->currentText();}
double ParaGUI::get_nukl_ener(){return ui->nukl_ener->value()*1000000;}
QString ParaGUI::get_regler(){return ui->regler->currentText();}
int ParaGUI::get_save_gap(){return ui->save_Gap->value();}
QString ParaGUI::get_spannung(){return ui->spannung->currentText();}
bool ParaGUI::get_noise(){return ui->noise->isChecked();}
double ParaGUI::get_noise_power(){return ui->noise_power->value();}
double ParaGUI::get_FIR_gain(){return ui->FIR_gain->value();}
double ParaGUI::get_fuellgrad(){return ui->fuellgrad->value();}
double ParaGUI::get_u_1(){return ui->u_1_spinbox->value();}
int ParaGUI::get_distortion_angle(){return ui->distortion_angle->value();}
int ParaGUI::get_distortion_steps_coasting(){return ui->distortion_steps_coasting->value();}
bool ParaGUI::get_distortion_active(){return ui->distortion_active->isChecked();}
bool ParaGUI::get_distortion_repeat_active(){return ui->distortion_repeat_active->isChecked();}
int ParaGUI::get_distortion_time_repeat(){return ui->distortion_time_repeat->value();}
int ParaGUI::get_harmonic_number(){return ui->harmonic_number->value();}
double ParaGUI::get_freq_mod(){return ui->frequenz_modifier->value();}
bool ParaGUI::get_single_particle(){return ui->single_particle->isChecked();}

void ParaGUI::set_anz_um(int i){ui->anz_um->setValue(i);}
void ParaGUI::set_anz_teil(int i){ ui->anz_teil->setValue(i);}
void ParaGUI::set_anz_threads(int i){ ui->anz_threads->setValue(i);}
void ParaGUI::set_save_gap(int i){ ui->save_Gap->setValue(i);}
void ParaGUI::set_hist_anz(int i){ ui->hist_anz->setValue(i);}
void ParaGUI::set_nukl_ener(double d){ ui->nukl_ener->setValue(d/1000000);}
void ParaGUI::set_fuellgrad(double d){ ui->fuellgrad->setValue(d);}
void ParaGUI::set_spannung(QString s){ ui->spannung->setCurrentText(s);}
void ParaGUI::set_regler(QString s){ ui->regler->setCurrentText(s);}
void ParaGUI::set_ion_sort(QString s){ ui->ionen_sorte->setCurrentText(s);}
void ParaGUI::set_coasting(bool b){ ui->coasting->setChecked(b);}
void ParaGUI::set_acc(bool b){ ui->beschleunigung->setChecked(b);}
void ParaGUI::set_noise(bool b){ ui->noise->setChecked(b);}
void ParaGUI::set_noise_power(double d){ ui->noise_power->setValue(d);}
void ParaGUI::set_FIR_gain(double d){ ui->FIR_gain->setValue(d);}
void ParaGUI::set_u_1(double d){ ui->u_1_spinbox->setValue(d);}
void ParaGUI::set_distortion_angle(int i){ ui->distortion_angle->setValue(i);}
void ParaGUI::set_distortion_steps_coasting(int i){ ui->distortion_steps_coasting->setValue(i);}
void ParaGUI::set_distortion_active(bool b){ ui->distortion_active->setChecked(b);}
void ParaGUI::set_distortion_repeat_active(bool b){ ui->distortion_repeat_active->setChecked(b);}
void ParaGUI::set_distortion_time_repeat(int i){ ui->distortion_time_repeat->setValue(i);}
void ParaGUI::set_harmonic_number(int i){ ui->harmonic_number->setValue(i);}
void ParaGUI::set_freq_mod(double d){ ui->frequenz_modifier->setValue(d);}
void ParaGUI::set_single_particle(bool b){ ui->single_particle->setChecked(b);}

