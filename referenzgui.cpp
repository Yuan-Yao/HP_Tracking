#include "referenzgui.h"

ReferenzGUI::ReferenzGUI(QVector<double> & counter, Log_Reference & log_ref, Log_Bunch & log_bunch):
  ui(new QWidget),
  plot_1(new QwtPlot(ui)),
  plot_2(new QwtPlot(ui)),
  plot_3(new QwtPlot(ui)),
  plot_4(new QwtPlot(ui)),
  plot_5(new QwtPlot(ui)),
  plot_6(new QwtPlot(ui)),
  plot_7(new QwtPlot(ui)),
  plot_8(new QwtPlot(ui)),
  plot_9(new QwtPlot(ui)),
  plot_10(new QwtPlot(ui)),
  plot_11(new QwtPlot(ui)),
  plot_12(new QwtPlot(ui)),
  combo_1(new QComboBox(ui)),
  combo_2(new QComboBox(ui)),
  combo_3(new QComboBox(ui)),
  combo_4(new QComboBox(ui)),
  combo_5(new QComboBox(ui)),
  combo_6(new QComboBox(ui)),
  combo_7(new QComboBox(ui)),
  combo_8(new QComboBox(ui)),
  combo_9(new QComboBox(ui)),
  combo_10(new QComboBox(ui)),
  combo_11(new QComboBox(ui)),
  combo_12(new QComboBox(ui)),
  curve_1(new QwtPlotCurve("Curve_1")),
  curve_2(new QwtPlotCurve("Curve_2")),
  curve_3(new QwtPlotCurve("Curve_3")),
  curve_4(new QwtPlotCurve("Curve_4")),
  curve_5(new QwtPlotCurve("Curve_5")),
  curve_6(new QwtPlotCurve("Curve_6")),
  curve_7(new QwtPlotCurve("Curve_7")),
  curve_8(new QwtPlotCurve("Curve_8")),
  curve_9(new QwtPlotCurve("Curve_9")),
  curve_10(new QwtPlotCurve("Curve_10")),
  curve_11(new QwtPlotCurve("Curve_11")),
  curve_12(new QwtPlotCurve("Curve_12")),
  intern_counter(counter),
  intern_log_ref(log_ref),
  intern_log_bunch(log_bunch)
{
  ui->setFixedSize(1800,1030);
  ui->setWindowTitle("Referenz-Plots");

  qstring << "t_R" << "T_R" << "u_R" << "u_1R" << "u_2R" << "phi_1R" << "phi_2R"
          << "omega_R" << "omega_syn" << "w_R" << "BDot" << "B"
          << "beta_R" << "eta_R"<< "bucket" << "UFP" << "emittance" << "moment_phi_1"
          << "moment_DEo_1"<< "moment_phi_2"<< "moment_phi_1_DEo_1" << "moment_DEo_2";

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
  combo_1->setFixedSize(170,30);
  combo_2->setFixedSize(170,30);
  combo_3->setFixedSize(170,30);
  combo_4->setFixedSize(170,30);
  combo_5->setFixedSize(170,30);
  combo_6->setFixedSize(170,30);
  combo_7->setFixedSize(170,30);
  combo_8->setFixedSize(170,30);
  combo_9->setFixedSize(170,30);
  combo_10->setFixedSize(170,30);
  combo_11->setFixedSize(170,30);
  combo_12->setFixedSize(170,30);
  combo_1->move(720,75);
  combo_2->move(720,245);
  combo_3->move(720,415);
  combo_4->move(720,585);
  combo_5->move(720,755);
  combo_6->move(720,925);
  combo_7->move(1620,75);
  combo_8->move(1620,245);
  combo_9->move(1620,415);
  combo_10->move(1620,585);
  combo_11->move(1620,755);
  combo_12->move(1620,925);
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
  combo_1->show();
  combo_2->show();
  combo_3->show();
  combo_4->show();
  combo_5->show();
  combo_6->show();
  combo_7->show();
  combo_8->show();
  combo_9->show();
  combo_10->show();
  combo_11->show();
  combo_12->show();
  ui->show();

 // QObject::connect(combo_1,SIGNAL(currentTextChanged(QString &)),this,SLOT(plot_single_curve_1()));
}

void ReferenzGUI::local_initialize(QComboBox & combo){
  for(int i = 0; i < qstring.size(); i++){
    combo.addItem(qstring.at(i));
  }
}

QVector<double> ReferenzGUI::get_plot_data (QString string){
  if (string == "B") return intern_log_ref.get_B();
  else if (string == "BDot") return intern_log_ref.get_BDot();
  else if (string == "beta_R") return intern_log_ref.get_beta_R();
  else if (string == "bucket") return intern_log_ref.get_bucket();
  else if (string == "eta_R") return intern_log_ref.get_eta_R();
  else if (string == "omega_R") return intern_log_ref.get_omega_R();
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
  else if (string == "emittance") return intern_log_bunch.get_emittance();
  else if (string == "moment_phi_1") return intern_log_bunch.get_moment_phi_1();
  else if (string == "moment_DEo_1") return intern_log_bunch.get_moment_DEo_1();
  else if (string == "moment_phi_2") return intern_log_bunch.get_moment_phi_2();
  else if (string == "moment_phi_1_DEo_1") return intern_log_bunch.get_moment_phi_1_DEo_1();
  else return intern_log_bunch.get_moment_DEo_2();
}

void ReferenzGUI::plot_Curve(QComboBox & combo,QwtPlot * plot, QwtPlotCurve * curve){
  plot->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  plot->setAxisScale(QwtPlot::xBottom,0,intern_counter.size());
  QVector<double> temp = get_plot_data(combo.currentText());
  curve->setSamples(intern_counter,temp);
  curve->attach(plot);
  plot->replot();
}

void ReferenzGUI::init_combo_box(){
  local_initialize(*combo_1);
  combo_1->setCurrentIndex(1);
  plot_Curve(*combo_1,plot_1,curve_1);
  local_initialize(*combo_2);
  combo_2->setCurrentIndex(2);
  plot_Curve(*combo_2,plot_2,curve_2);
  local_initialize(*combo_3);
  combo_3->setCurrentIndex(3);
  plot_Curve(*combo_3,plot_3,curve_3);
  local_initialize(*combo_4);
  combo_4->setCurrentIndex(4);
  plot_Curve(*combo_4,plot_4,curve_4);
  local_initialize(*combo_5);
  combo_5->setCurrentIndex(5);
  plot_Curve(*combo_5,plot_5,curve_5);
  local_initialize(*combo_6);
  combo_6->setCurrentIndex(6);
  plot_Curve(*combo_6,plot_6,curve_6);
  local_initialize(*combo_7);
  combo_7->setCurrentIndex(7);
  plot_Curve(*combo_7,plot_7,curve_7);
  local_initialize(*combo_8);
  combo_8->setCurrentIndex(8);
  plot_Curve(*combo_8,plot_8,curve_8);
  local_initialize(*combo_9);
  combo_9->setCurrentIndex(9);
  plot_Curve(*combo_9,plot_9,curve_9);
  local_initialize(*combo_10);
  combo_10->setCurrentIndex(10);
  plot_Curve(*combo_10,plot_10,curve_10);
  local_initialize(*combo_11);
  combo_11->setCurrentIndex(11);
  plot_Curve(*combo_11,plot_11,curve_11);
  local_initialize(*combo_12);
  combo_12->setCurrentIndex(12);
  plot_Curve(*combo_12,plot_12,curve_12);
}

void ReferenzGUI::plot_single_curve_1(){
  plot_Curve(*combo_1,plot_1,curve_1);
}
