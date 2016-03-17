#include "monitoring.h"
#include "ui_monitoring.h"

Monitoring::Monitoring(QVector<double> & counter, Log_Reference & log_ref, Log_Bunch & log_bunch, Controller & control, int h_) :
  QWidget(nullptr),
  ui(new Ui::Monitoring),
  h(h_),
  plot_1(new QwtPlot(this)),
  plot_2(new QwtPlot(this)),
  plot_3(new QwtPlot(this)),
  plot_4(new QwtPlot(this)),
  plot_5(new QwtPlot(this)),
  plot_6(new QwtPlot(this)),
  plot_7(new QwtPlot(this)),
  plot_8(new QwtPlot(this)),
  plot_9(new QwtPlot(this)),
  plot_10(new QwtPlot(this)),
  plot_11(new QwtPlot(this)),
  plot_12(new QwtPlot(this)),
  curve_1(new QwtPlotCurve()),
  curve_2(new QwtPlotCurve()),
  curve_3(new QwtPlotCurve()),
  curve_4(new QwtPlotCurve()),
  curve_5(new QwtPlotCurve()),
  curve_6(new QwtPlotCurve()),
  curve_7(new QwtPlotCurve()),
  curve_8(new QwtPlotCurve()),
  curve_9(new QwtPlotCurve()),
  curve_10(new QwtPlotCurve()),
  curve_11(new QwtPlotCurve()),
  curve_12(new QwtPlotCurve()),
  grid_1(new QwtPlotGrid()),
  grid_2(new QwtPlotGrid()),
  grid_3(new QwtPlotGrid()),
  grid_4(new QwtPlotGrid()),
  grid_5(new QwtPlotGrid()),
  grid_6(new QwtPlotGrid()),
  grid_7(new QwtPlotGrid()),
  grid_8(new QwtPlotGrid()),
  grid_9(new QwtPlotGrid()),
  grid_10(new QwtPlotGrid()),
  grid_11(new QwtPlotGrid()),
  grid_12(new QwtPlotGrid()),
  intern_counter(counter),
  intern_log_ref(log_ref),
  intern_log_bunch(log_bunch),
  intern_control(control),
  init(false)
{
  ui->setupUi(this);
  this->setWindowTitle("Referenz-Plots");
  //string which has all options, what can be chosen in the monitoring window
  qstring << "t_R" << "T_R" << "u_R" << "u_1R" << "u_2R" << "phi_1R" << "phi_2R"
          << "f_R" <<"omega_R" << "f_syn" << "omega_syn" << "w_R" << "BDot" << "B"
          << "beta_R" << "eta_R"<< "bucket" << "UFP"
          << "Delta_phi_gap" << "fd_xI" << "u"
          << "fft_phase_0" << "fft_amp_0" << "fft_phase_dis_0" << "fft_amp_dis_0" <<"emittance_multi"
          << "emittance_multi_com" << "moment_phi_1_multi" << "moment_phi_2_multi" << "moment_DEo_1_multi"
          << "moment_DEo_2_multi" << "moment_phi_1_DEo_1_multi" << "DH_phi_gap" << "DH_phi_gap_10" << "DH_energy" << "DH_energy_10" << "DH_pos" << "DH_vel"<< "DH_integrator" << "DH_integrator_energy"
          << "DH2_integrator" << "DH2_phi_gap" << "DH2_phi_gap_10" << "DH2_local_div" << "DH2_base" << "DH2_u";

  plot_1->setFixedSize(700,160);
  plot_2->setFixedSize(700,160);
  plot_3->setFixedSize(700,160);
  plot_4->setFixedSize(700,160);
  plot_5->setFixedSize(700,160);
  plot_6->setFixedSize(700,160);
  plot_7->setFixedSize(700,160);
  plot_8->setFixedSize(700,160);
  plot_9->setFixedSize(700,160);
  plot_10->setFixedSize(700,160);
  plot_11->setFixedSize(700,160);
  plot_12->setFixedSize(700,160);

  plot_1->move(10,10);
  plot_2->move(10,180);
  plot_3->move(10,350);
  plot_4->move(10,520);
  plot_5->move(10,690);
  plot_6->move(10,860);
  plot_7->move(910,10);
  plot_8->move(910,180);
  plot_9->move(910,350);
  plot_10->move(910,520);
  plot_11->move(910,690);
  plot_12->move(910,860);

  plot_1->show();
  plot_2->show();
  plot_3->show();
  plot_4->show();
  plot_5->show();
  plot_6->show();
  plot_7->show();
  plot_8->show();
  plot_9->show();
  plot_10->show();
  plot_11->show();
  plot_12->show();

  ui->spin_begin->setMinimum(0);
  ui->spin_begin->setValue(0);
  ui->spin_end->setMaximum(intern_counter.size()-1);
  ui->spin_end->setValue(intern_counter.size()-1);
  ui->spin_begin->setMaximum(ui->spin_end->value()-1);
  ui->spin_end->setMinimum(ui->spin_begin->value()+1);
  ui->h_spin_box->setMaximum(h);
}

Monitoring::~Monitoring()
{
  delete ui;
}

void Monitoring::local_initialize(QComboBox & combo){
  for(int i = 0; i < qstring.size(); i++){
    combo.addItem(qstring.at(i));
  }
}

QVector<double> Monitoring::get_plot_data (QString string){
  if (string == "B") return intern_log_ref.get_B();
  else if (string == "BDot") return intern_log_ref.get_BDot();
  else if (string == "beta_R") return intern_log_ref.get_beta_R();
  else if (string == "bucket") return intern_log_ref.get_bucket();
  else if (string == "eta_R") return intern_log_ref.get_eta_R();
  else if (string == "f_R") return intern_log_ref.get_f_R();
  else if (string == "omega_R") return intern_log_ref.get_omega_R();
  else if (string == "f_syn") return intern_log_ref.get_f_syn();
  else if (string == "omega_syn") return intern_log_ref.get_omega_syn();
  else if (string == "phi_1R") return intern_log_ref.get_phi_1R();
  else if (string == "phi_2R") return intern_log_ref.get_phi_2R();
  else if (string == "T_R") return intern_log_ref.get_T_R();
  else if (string == "t_R") return intern_log_ref.get_t_R();
  else if (string == "u_1R") return intern_log_ref.get_u_1R();
  else if (string == "u_2R") return intern_log_ref.get_u_2R();
  else if (string == "u_R") return intern_log_ref.get_u_R();
  else if (string == "w_R") return intern_log_ref.get_w_R();
  else if (string == "UFP") return intern_log_ref.get_UFP();
  else if (string == "u") return intern_control.get_u_print();
  else if (string == "fd_xI") return intern_control.get_fd_xI((ui->h_spin_box->value()-1));
  else if (string == "Delta_phi_gap") return intern_control.get_Delta_phi_gap(ui->h_spin_box->value()-1);
  else if (string =="fft_phase_0") return intern_log_bunch.get_log_fft_phase_0(ui->h_spin_box->value()-1);
  else if (string =="fft_amp_0") return intern_log_bunch.get_log_fft_amp_0(ui->h_spin_box->value()-1);
  else if (string =="fft_phase_dis_0") return intern_log_bunch.get_log_fft_phase_dis_0(ui->h_spin_box->value()-1);
  else if (string =="fft_amp_dis_0") return intern_log_bunch.get_log_fft_amp_dis_0(ui->h_spin_box->value()-1);
  else if (string =="emittance_multi") return intern_log_bunch.get_emittance(ui->h_spin_box->value()-1);
  else if (string =="emittance_multi_com") return intern_log_bunch.get_emittance_sum();
  else if (string =="moment_phi_1_multi") return intern_log_bunch.get_moment_phi_1(ui->h_spin_box->value()-1);
  else if (string =="moment_phi_2_multi") return intern_log_bunch.get_moment_phi_2(ui->h_spin_box->value()-1);
  else if (string =="moment_DEo_1_multi") return intern_log_bunch.get_moment_DEo_1(ui->h_spin_box->value()-1);
  else if (string =="moment_DEo_2_multi") return intern_log_bunch.get_moment_DEo_2(ui->h_spin_box->value()-1);
  else if (string =="moment_phi_1_DEo_1_multi") return intern_log_bunch.get_moment_phi_1_DEo_1(ui->h_spin_box->value()-1);
  else if (string =="DH_phi_gap") return intern_control.get_DH_phi_gap((ui->h_spin_box->value()-1));
  else if (string =="DH_phi_gap_10") return intern_control.get_DH_phi_gap_10((ui->h_spin_box->value()-1));
  else if (string == "DH_energy") return intern_control.get_DH_energy(ui->h_spin_box->value()-1);
  else if (string == "DH_energy_10") return intern_control.get_DH_energy_10(ui->h_spin_box->value()-1);
  else if (string == "DH_integrator") return intern_control.get_DH_integrator(ui->h_spin_box->value()-1);
  else if (string == "DH_integrator_energy") return intern_control.get_DH_integrator_energy(ui->h_spin_box->value()-1);
  else if (string == "DH_pos") return intern_control.get_DH_pos(ui->h_spin_box->value()-1);
  else if (string == "DH_vel") return intern_control.get_DH_vel(ui->h_spin_box->value()-1);
  else if (string == "DH2_integrator") return intern_control.get_DH2_integrator(ui->h_spin_box->value()-1);
  else if (string == "DH2_phi_gap") return intern_control.get_DH2_phi_gap(ui->h_spin_box->value()-1);
  else if (string == "DH2_phi_gap_10") return intern_control.get_DH2_phi_gap_10(ui->h_spin_box->value()-1);
  else if (string == "DH2_local_div") return intern_control.get_DH2_local_div(ui->h_spin_box->value()-1);
  else if (string == "DH2_base") return intern_control.get_DH2_base(ui->h_spin_box->value()-1);
  else if (string == "DH2_u") return intern_control.get_DH2_u(ui->h_spin_box->value()-1);
  else return intern_log_ref.get_B();
}

//plot initial plot
void Monitoring::plot_Curve(QComboBox & combo,QwtPlot * plot, QwtPlotCurve * curve, QwtPlotGrid * grid){
  plot->detachItems(QwtPlotItem::Rtti_PlotItem,true);

  if (!ui->Time_check->isChecked()) { plot->setAxisScale(QwtPlot::xBottom,0,intern_counter.size()); }
  else { plot->setAxisScale(QwtPlot::xBottom,0,intern_log_ref.get_t_R().last());}

  QVector<double> temp = get_plot_data(combo.currentText());

  if(!ui->Time_check->isChecked()){curve->setSamples(intern_counter,temp);}
  else {curve->setSamples(intern_log_ref.get_t_R(),temp);}
  curve->attach(plot);
  grid->setPen(QPen(QColor(196,196,196)));
  grid->attach(plot);
  plot->replot();
}

//if plots are changed, the curves have to be deleted carefully, because the otherwise bizarre errors arise
void Monitoring::plot_Curve_change(QString string, QwtPlot * plot, QwtPlotCurve * curve,QwtPlotGrid * grid){

  curve = new QwtPlotCurve();
  grid = new QwtPlotGrid();
  plot->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  if (!ui->Time_check->isChecked()) {plot->setAxisScale(QwtPlot::xBottom,intern_counter[0],intern_counter[intern_counter.size()-1]);}
  else {plot->setAxisScale(QwtPlot::xBottom,intern_log_ref.get_t_R()[intern_counter[0]],intern_log_ref.get_t_R()[intern_counter[intern_counter.size()-1]]);}
  QVector<double> temp = get_plot_data(string);
  if (!ui->Time_check->isChecked()) {curve->setSamples(intern_counter,temp.mid(intern_counter[0],intern_counter.size()));}
  else {curve->setSamples(intern_log_ref.get_t_R().mid(intern_counter[0],intern_counter.size()),temp.mid(intern_counter[0],intern_counter.size()));}
  curve->attach(plot);
  grid->setPen(QPen(QColor(196,196,196)));
  grid->attach(plot);

  plot->replot();
}

void Monitoring::init_combo_box(){
  local_initialize(*ui->comboBox_1);
  ui->comboBox_1->setCurrentIndex(1);
  plot_Curve(*ui->comboBox_1,plot_1,curve_1,grid_1);
  local_initialize(*ui->comboBox_2);
  ui->comboBox_2->setCurrentIndex(2);
  plot_Curve(*ui->comboBox_2,plot_2,curve_2,grid_2);
  local_initialize(*ui->comboBox_3);
  ui->comboBox_3->setCurrentIndex(3);
  plot_Curve(*ui->comboBox_3,plot_3,curve_3,grid_3);
  local_initialize(*ui->comboBox_4);
  ui->comboBox_4->setCurrentIndex(4);
  plot_Curve(*ui->comboBox_4,plot_4,curve_4,grid_4);
  local_initialize(*ui->comboBox_5);
  ui->comboBox_5->setCurrentIndex(5);
  plot_Curve(*ui->comboBox_5,plot_5,curve_5,grid_5);
  local_initialize(*ui->comboBox_6);
  ui->comboBox_6->setCurrentIndex(6);
  plot_Curve(*ui->comboBox_6,plot_6,curve_6,grid_6);
  local_initialize(*ui->comboBox_7);
  ui->comboBox_7->setCurrentIndex(7);
  plot_Curve(*ui->comboBox_7,plot_7,curve_7,grid_7);
  local_initialize(*ui->comboBox_8);
  ui->comboBox_8->setCurrentIndex(8);
  plot_Curve(*ui->comboBox_8,plot_8,curve_8,grid_8);
  local_initialize(*ui->comboBox_9);
  ui->comboBox_9->setCurrentIndex(9);
  plot_Curve(*ui->comboBox_9,plot_9,curve_9,grid_9);
  local_initialize(*ui->comboBox_10);
  ui->comboBox_10->setCurrentIndex(10);
  plot_Curve(*ui->comboBox_10,plot_10,curve_10,grid_10);
  local_initialize(*ui->comboBox_11);
  ui->comboBox_11->setCurrentIndex(11);
  plot_Curve(*ui->comboBox_11,plot_11,curve_11,grid_11);
  local_initialize(*ui->comboBox_12);
  ui->comboBox_12->setCurrentIndex(12);
  plot_Curve(*ui->comboBox_12,plot_12,curve_12,grid_12);
  init = true;
}

void Monitoring::on_comboBox_1_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_1,curve_1,grid_1);
}

void Monitoring::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_2,curve_2,grid_2);
}

void Monitoring::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_3,curve_3,grid_3);
}

void Monitoring::on_comboBox_4_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_4,curve_4,grid_4);
}

void Monitoring::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_5,curve_5,grid_5);
}

void Monitoring::on_comboBox_6_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_6,curve_6,grid_6);
}

void Monitoring::on_comboBox_10_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_10,curve_10,grid_10);
}

void Monitoring::on_comboBox_7_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_7,curve_7,grid_7);
}

void Monitoring::on_comboBox_8_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_8,curve_8,grid_8);
}

void Monitoring::on_comboBox_11_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_11,curve_11,grid_11);
}

void Monitoring::on_comboBox_9_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_9,curve_9,grid_9);
}

void Monitoring::on_comboBox_12_currentIndexChanged(const QString &arg1)
{
  QString temp = arg1;
  if(init) plot_Curve_change(temp,plot_12,curve_12,grid_12);
}

//changes, if intervall is changed, alls windows are rearranged, or if its about bunch statistic in buckets, the right bucket is chosen
void Monitoring::on_spin_end_editingFinished()
{
    ui->spin_begin->setMaximum(ui->spin_end->value()-1);
    int newlength = ui->spin_end->value()-ui->spin_begin->value()+1;
    intern_counter.clear();
    intern_counter.resize(newlength);
    for(int i = 0; i < newlength; i++){
      intern_counter[i] = ui->spin_begin->value()+i;
    }
    plot_Curve_change(ui->comboBox_1->currentText(),plot_1,curve_1,grid_1);
    plot_Curve_change(ui->comboBox_2->currentText(),plot_2,curve_2,grid_2);
    plot_Curve_change(ui->comboBox_3->currentText(),plot_3,curve_3,grid_3);
    plot_Curve_change(ui->comboBox_4->currentText(),plot_4,curve_4,grid_4);
    plot_Curve_change(ui->comboBox_5->currentText(),plot_5,curve_5,grid_5);
    plot_Curve_change(ui->comboBox_6->currentText(),plot_6,curve_6,grid_6);
    plot_Curve_change(ui->comboBox_7->currentText(),plot_7,curve_7,grid_7);
    plot_Curve_change(ui->comboBox_8->currentText(),plot_8,curve_8,grid_8);
    plot_Curve_change(ui->comboBox_9->currentText(),plot_9,curve_9,grid_9);
    plot_Curve_change(ui->comboBox_10->currentText(),plot_10,curve_10,grid_10);
    plot_Curve_change(ui->comboBox_11->currentText(),plot_11,curve_11,grid_11);
    plot_Curve_change(ui->comboBox_12->currentText(),plot_12,curve_12,grid_12);
}

void Monitoring::on_spin_begin_editingFinished()
{
    ui->spin_end->setMinimum(ui->spin_begin->value()+1);
    int newlength = ui->spin_end->value()-ui->spin_begin->value()+1;
    intern_counter.clear();
    intern_counter.resize(newlength);
    for(int i = 0; i < newlength; i++){
      intern_counter[i] = ui->spin_begin->value()+i;
    }
    plot_Curve_change(ui->comboBox_1->currentText(),plot_1,curve_1,grid_1);
    plot_Curve_change(ui->comboBox_2->currentText(),plot_2,curve_2,grid_2);
    plot_Curve_change(ui->comboBox_3->currentText(),plot_3,curve_3,grid_3);
    plot_Curve_change(ui->comboBox_4->currentText(),plot_4,curve_4,grid_4);
    plot_Curve_change(ui->comboBox_5->currentText(),plot_5,curve_5,grid_5);
    plot_Curve_change(ui->comboBox_6->currentText(),plot_6,curve_6,grid_6);
    plot_Curve_change(ui->comboBox_7->currentText(),plot_7,curve_7,grid_7);
    plot_Curve_change(ui->comboBox_8->currentText(),plot_8,curve_8,grid_8);
    plot_Curve_change(ui->comboBox_9->currentText(),plot_9,curve_9,grid_9);
    plot_Curve_change(ui->comboBox_10->currentText(),plot_10,curve_10,grid_10);
    plot_Curve_change(ui->comboBox_11->currentText(),plot_11,curve_11,grid_11);
    plot_Curve_change(ui->comboBox_12->currentText(),plot_12,curve_12,grid_12);
}

void Monitoring::on_h_spin_box_editingFinished()
{
  plot_Curve_change(ui->comboBox_1->currentText(),plot_1,curve_1,grid_1);
  plot_Curve_change(ui->comboBox_2->currentText(),plot_2,curve_2,grid_2);
  plot_Curve_change(ui->comboBox_3->currentText(),plot_3,curve_3,grid_3);
  plot_Curve_change(ui->comboBox_4->currentText(),plot_4,curve_4,grid_4);
  plot_Curve_change(ui->comboBox_5->currentText(),plot_5,curve_5,grid_5);
  plot_Curve_change(ui->comboBox_6->currentText(),plot_6,curve_6,grid_6);
  plot_Curve_change(ui->comboBox_7->currentText(),plot_7,curve_7,grid_7);
  plot_Curve_change(ui->comboBox_8->currentText(),plot_8,curve_8,grid_8);
  plot_Curve_change(ui->comboBox_9->currentText(),plot_9,curve_9,grid_9);
  plot_Curve_change(ui->comboBox_10->currentText(),plot_10,curve_10,grid_10);
  plot_Curve_change(ui->comboBox_11->currentText(),plot_11,curve_11,grid_11);
  plot_Curve_change(ui->comboBox_12->currentText(),plot_12,curve_12,grid_12);
}
