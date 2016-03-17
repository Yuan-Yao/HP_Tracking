#include "maingui.h"
#include "ui_maingui.h"

//CONSTUCTOR and DESTRUCTOR
MainGUI::MainGUI(QWidget *parent):QMainWindow(parent),//MainGUI ruft ConstructorFunk "MainGUI(QWidget *parent)" und "QMainWindow(parent)" aus seiner ParentKlasse auf.

  ui(new Ui::MainGUI),//Constructor von Klasse Ui::MainGUI aufrufen
  paraGUI(new ParaGUI()),
  monit(nullptr),//nullptr is null pointer
  para(new Parameter(*paraGUI)),//Parameter *para in header of maingui, Parameter is no class, but a struct, whose constructor is Parameter(PareGUI &)
  thread_count(para->threads),
  ref(new Referenz()),
  sep(new Seperatrix(*para,600,100)), //seperatrix save and dynamic resolution is set here
  log_ref(nullptr),
  control(nullptr),
  dist(nullptr),
  log_bunch(nullptr),
  lri(new log_ref_i()),
  hist(nullptr),
  fda(nullptr),
  save(nullptr),
  p_save(new para_save()),

  myplot1(new QwtPlot(this)),
  myplot2(new QwtPlot(this)),
  myplot3(new QwtPlot(this)),
  myplot4(new QwtPlot(this)),
  myplot5(new QwtPlot(this))
{
  ui->setupUi(this);
  ui->Monitoring->setEnabled(false);
  if(!p_save->al_exist())
      p_save->save(*paraGUI);
  p_save->load(*paraGUI);
}

MainGUI::~MainGUI()
{
  delete ui;
}
//end CONSTRUCTOR and DESTRUCTOR



//first some time debug functions

void MainGUI::time_start(){
  start = std::chrono::system_clock::now();
}

void MainGUI::time_end(){
  end = std::chrono::system_clock::now();
  auto elap = std::chrono::duration_cast<std::chrono::microseconds> (end-start);
  qDebug() << "MainGUI time: " <<elap.count();
}

void MainGUI::time_intervall(){
  end = std::chrono::system_clock::now();
  auto elap = std::chrono::duration_cast<std::chrono::microseconds> (end-start);
  qDebug() << "maingui time: " <<elap.count();
  start = std::chrono::system_clock::now();
}

void MainGUI::partial_time(int & time){
  partialend = std::chrono::system_clock::now();
  auto elap = std::chrono::duration_cast<std::chrono::microseconds> (partialend-partialstart);
  time = time+elap.count();
  partialstart = std::chrono::system_clock::now();
}

//helper structs; some functions are pulled together to be able to handle them in threads, which are to be runned parallel
struct helper_struct
{
    Bunch & bun;
    log_ref_i & lri;
    Parameter & par;
    int i;
    std::vector<double> u;
    helper_struct(Bunch & bun_, log_ref_i & lri_, Parameter & par_, int i_, std::vector<double> u_):bun(bun_),lri(lri_),par(par_),i(i_),u(u_){}
    void operator() (){
      bun.tracking(par,lri,u);
      bun.moment_actualisation(par);
    }
};

struct helper_struct_2
{
    Bunch & bun;
    Parameter & par;
    int i;
    helper_struct_2(Bunch & bun_, Parameter & par_, int i_):bun(bun_),par(par_),i(i_){}
    void operator() (){
      bun.moment_actualisation_2(par);
    }
};

struct do_nothing
{
    do_nothing();
    void operator() (){}
};

void MainGUI::tracking_simulation()
{  
  p_save->save(*paraGUI);
  //Reinitialisation Process

  //hide monitoring, and delete, if exists
  if(monit != nullptr){//if monit not a void pointer,then...
    monit->hide_GUI();
    Monitoring * temp = monit;
    monit = nullptr;
    delete temp;
  }

  //delete parameters and read them a new from paragui
  if(para != nullptr){
    Parameter * temp = para;
    para = nullptr;
    delete temp;
  }
  para = new Parameter(*paraGUI);
  thread_count = para->threads;
  para->ausgabe_debug();
  //save_results
  if (save != nullptr){
    save_Results * temp = save;
    save = nullptr;
    delete temp;
  }
  save = new save_Results(*para);

  //reference parameters have to be actualised
  ref->init_ref(*para);
  //delete and create seperatrix object
  if(sep != nullptr){
    Seperatrix * temp = sep;
    sep = nullptr;
    delete temp;
  }
  sep = new Seperatrix(*para,600,100);
  sep->get_from_ref(ref->ref_to_seperatrix());
  sep->set_static_seperatrix();
  //delete and initialise controller
  if (control != nullptr){
    Controller * tempc = control;
    control = nullptr;
    delete tempc;
  }
  control = new Controller(*para,*lri);

  //delete and initialise Reference-Log
  if (log_ref != nullptr){
    Log_Reference * temp = log_ref;
    log_ref = nullptr;
    delete temp;
  }
  log_ref = new Log_Reference(para->anz_Umdrehungen);

  log_ref->save_stat(0,*ref,*sep);

  //Bunch is emptied as a vector, not as a pointer. Bunch consists of as much bunches, as threads will be used
  if (bunch.empty() == false) bunch.clear();
  for (int i = 0; i < thread_count; i++) bunch.push_back(Bunch(*para));
  for (unsigned int i = 0; i < bunch.size(); i++) bunch[i].init_bunch(*para,*sep,*ref);
  for (unsigned int i = 0; i < bunch.size(); i++) bunch[i].moment_actualisation(*para);

  //delete and initialise distortion and histogramm
  if (dist != nullptr){
    Distortion * temp = dist;
    dist = nullptr;
    delete temp;
  }
  if (hist != nullptr){
    Histogramm * temp = hist;
    hist = nullptr;
    delete temp;
  }
  hist = new Histogramm(bunch,*para);

  //delete and initialise Bunch-Log
  if (log_bunch != nullptr){
    Log_Bunch * temp = log_bunch;
    log_bunch = nullptr;
    delete temp;
  }
  //save initial stats
  log_bunch = new Log_Bunch(*para,*sep,para->save_Slots,thread_count);
  log_bunch->save_moments_1(bunch,0);
  hist->process_bunch(bunch,*para);
  log_bunch->save_histogramm(*hist,0);
  for (unsigned int i = 0; i < bunch.size(); i++) bunch[i].moment_actualisation_2(*para);
  log_bunch->save_moments_2(bunch,*hist,para->coasting,0);
  log_bunch->save_bunch(bunch,*hist,*sep,0);
  //
  //end (re-)initialising
  //
  //
  //tracking-simulation
  //
for (int i = 1; i < para->anz_Umdrehungen+1; i++){
    //change machine parameters
    ref->tracking(*para,*sep);
    log_ref->save_stat(i,*ref,*sep);
  }
 time_start();
  log_ref->set_log_ref_i(*lri,0);
  dist = new Distortion(*para,*lri,true,false);
  for (int i = 1; i < para->anz_Umdrehungen+1;i++){

    // if activated, a side branch is started, which extracts the damping and synchrotron frequency of the nonlinear simulation
    // without a controller;;; maybe even searches for the best gain !, Afterwards everything is back to normal


    if (i == 1000000){
      //delete fda if it exists;
      if (fda != nullptr){
        freq_Damp_Analyser *temp = fda;
        fda = nullptr;
        delete temp;
      }
      fda = new freq_Damp_Analyser(*para,*log_ref,bunch,i,save);
      fda->local_tracking();
    }


    log_ref->set_log_ref_i(*lri,i);
    //actualise particle position and energy
    std::vector<helper_struct> h;
    std::vector<helper_struct_2> h2;
    std::vector<std::thread> t;
    std::vector<std::thread> t2;

    std::vector<double> dummy;
    dummy.assign(para->h,0);
    if (false) {for (unsigned int j = 0; j<bunch.size(); j++) h.push_back(helper_struct(bunch[j],*lri,*para,i,dummy));}
    else {for (unsigned int j = 0; j<bunch.size(); j++) h.push_back(helper_struct(bunch[j],*lri,*para,i,control->get_u()));}
    for (unsigned int j = 1; j<bunch.size(); j++) t.push_back(std::thread(h[j]));
    h[0]();
    for (unsigned int j = 0; j<t.size(); j++) t[j].join();

    //statistic operations on bunches and controller for next turn

    log_bunch->save_moments_1(bunch,i);
    hist->process_bunch(bunch,*para);

    for (unsigned int i = 0; i<bunch.size(); i++) h2.push_back(helper_struct_2(bunch[i],*para,i));
    for (unsigned int i = 1; i<bunch.size(); i++) t2.push_back(std::thread(h2[i]));
    h2[0]();
    for (unsigned int i = 0; i<t2.size(); i++) t2[i].join();
    log_bunch->save_moments_2(bunch,*hist,para->coasting,i);

    control->operate(i,*log_bunch,*lri,*para);
    //save Bunches only every save_Gap time to save memory and copying time
    if (i%para->save_Gap == 0){
      qDebug() << i << log_ref->get_u_1R()[i];
      time_end();
      time_start();
      sep->get_from_lri(*lri);
      sep->set_full_length_dynamic_sep(0);
      sep->set_voltage(control->get_u());
      log_bunch->save_bunch(bunch,*hist,*sep,i/para->save_Gap);
      log_bunch->save_histogramm(*hist,i/para->save_Gap);
    }

    //apply distortions on bunches. can be applied here, because one turn does not matter
    dist->process_distortion(*para,*lri,bunch);

  }
  sep->set_all_small_seperatrix();
}


//actual simulation
void MainGUI::on_Starte_Simulation_clicked()
{

  tracking_simulation();
  //
  //plot result
  //
  QwtSymbol * symbol = new QwtSymbol(QwtSymbol::XCross,QBrush(Qt::green),QPen(Qt::blue),QSize(3,3));

  QwtPlotCurve *curve1_2 = new QwtPlotCurve("Curve1_2");
  QwtPlotCurve *curve2 = new QwtPlotCurve("Curve2");
  QwtPlotCurve *curve3 = new QwtPlotCurve("Curve3");
  QwtPlotCurve *curve4 = new QwtPlotCurve("Curve4");
  QwtPlotCurve *bunchout = new QwtPlotCurve("Bunch");
  QwtPlotCurve *seppos = new QwtPlotCurve("Seperatrix pos");
  QwtPlotCurve *sepneg = new QwtPlotCurve("Seperatrix neg");
  QwtPlotCurve *voltage = new QwtPlotCurve("Voltage");
  QwtPlotCurve *seppos_stat = new QwtPlotCurve("Seperatrix pos");
  QwtPlotCurve *sepneg_stat = new QwtPlotCurve("Seperatrix neg");

  QVector<double> lauf(para->anz_Umdrehungen+1);
  for(int i=0; i< para->anz_Umdrehungen+1;i++){
    lauf[i] = i;
  }
  QVector<double> histo_lauf(para->histo_anz);
  for(int i=0; i<para->histo_anz; i++){
    histo_lauf[i] = -para->pi+2*para->pi*(i+0.5)/para->histo_anz*para->h;
  }
  QVector<double> histo_lauf_multi(para->histo_anz*para->h);
  for(int i=0; i<para->histo_anz*para->h; i++){
    histo_lauf_multi[i] = -para->pi+2*para->pi*(i+0.5)/para->histo_anz;
  }

  myplot1->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  myplot1->setAxisScale(QwtPlot::xBottom,histo_lauf[0],histo_lauf.last());
  //myplot2->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  //myplot2->setAxisScale(QwtPlot::xBottom,1,para->anz_Umdrehungen);
  myplot2->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  myplot2->setAxisScale(QwtPlot::xBottom,0,para->anz_Umdrehungen);
  myplot3->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  myplot3->setAxisScale(QwtPlot::xBottom,1,para->anz_Umdrehungen);
  myplot4->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  myplot4->setAxisScale(QwtPlot::xBottom,1,para->anz_Umdrehungen);
  myplot5->detachItems(QwtPlotItem::Rtti_PlotItem,true);

  curve1_2->setSamples(histo_lauf_multi,log_bunch->get_log_histogramm(para->save_Slots));
  curve1_2->attach(myplot1);
  curve2->setSamples(lauf,log_bunch->get_moment_phi_1(0));
  curve2->attach(myplot2);
  curve3->setSamples(lauf,log_bunch->get_lost_particles());
  curve3->attach(myplot3);

  //int laufparameter = log_bunch->get_log_fft_phase_0(0).size();
  //QVector<double> *temp= new QVector<double>(laufparameter);
  //for(int i=0; i< laufparameter;i++){
  //  temp->replace(i,log_bunch->get_log_fft_phase_0(0)[i]-log_ref->get_phi_1R()[i]);
  //}
  //curve4->setSamples(lauf,*temp);

  curve4->setSamples(lauf,log_bunch->get_emittance_sum());
  curve4->attach(myplot4);
  bunchout->setStyle(QwtPlotCurve::NoCurve);
  bunchout->setSymbol(symbol);
  bunchout->setSamples(log_bunch->get_log_bunch_phi(para->save_Slots),log_bunch->get_log_bunch_DEo(para->save_Slots));
  bunchout->attach(myplot5);
  QwtPlotMarker* refpoint = new QwtPlotMarker();
  refpoint->setSymbol(new QwtSymbol(QwtSymbol::Rect,QBrush(Qt::green),Qt::NoPen,QSize(5,5) ) );
  refpoint->setValue(QPointF (log_ref->get_phi_1R()[para->save_Slots*para->save_Gap] , 0));
  refpoint->attach(myplot5);
  seppos->setSamples(sep->get_phi_ring_sequence(),log_bunch->get_log_sep_pos(para->save_Slots));
  sepneg->setSamples(sep->get_phi_ring_sequence(),log_bunch->get_log_sep_neg(para->save_Slots));
  voltage->setSamples(sep->get_phi_ring_sequence(),log_bunch->get_log_voltage(para->save_Slots));
  seppos_stat->setSamples(sep->get_phi_ring_sequence(),sep->get_sep_pos_static());
  sepneg_stat->setSamples(sep->get_phi_ring_sequence(),sep->get_sep_neg_static());
  seppos->attach(myplot5);
  sepneg->attach(myplot5);
  voltage->attach(myplot5);
  seppos_stat->attach(myplot5);
  sepneg_stat->attach(myplot5);

  myplot5->setAxisScale(myplot5->xBottom,-para->pi,para->pi*(para->h*2-1),1.0);
  myplot5->setAxisScale(myplot5->yLeft,-4*sep->get_stat_max_sep(),4*sep->get_stat_max_sep());

  myplot1->resize(400,400);
  myplot1->move(0,60);
  myplot1->replot();
  myplot1->show();

  myplot2->resize(400,400);
  myplot2->move(0,460);
  myplot2->replot();
  myplot2->show();

  myplot3->resize(400,400);
  myplot3->move(400,60);
  myplot3->replot();
  myplot3->show();

  myplot4->resize(400,400);
  myplot4->move(400,460);
  myplot4->replot();
  myplot4->show();

  myplot5->resize(600,600);
  myplot5->move(800,60);
  myplot5->replot();
  myplot5->show();
  //
  //Ergebnisse plotten Ende
  //

  ui->bunch_scroll->move(800,660);
  ui->bunch_scroll->resize(600,50);
  ui->bunch_scroll->setMinimum(0);
  ui->bunch_scroll->setMaximum(para->save_Slots-1);
  ui->bunch_scroll->setValue(para->save_Slots-1);
  ui->bunch_scroll->setSingleStep(1);
  ui->bunch_scroll->setPageStep(10);//set bunch scroll button

  ui->Turn->resize(170,30);
  ui->Turn->move(1015,720);
  ui->Turn->setText(QString::number(ui->bunch_scroll->value()*para->save_Gap));

  ui->log_ref_value->setFixedSize(200,30);
  ui->log_ref_value->move(20,870);
  ui->log_ref_value->setRange(0,para->anz_Umdrehungen);
  ui->log_ref_value->setValue(0);
  ui->log_ref_value->setEnabled(true);
  ui->log_ref_value->show();

  monit = new Monitoring(lauf,*log_ref,log_bunch[0],*control,para->h);
  monit->init_combo_box();
  ui->Monitoring->setEnabled(true);
  ui->emittance_out->setText("Emittanz-Zuwachs: ") ;
}




//Slider-Action
void MainGUI::on_bunch_scroll_valueChanged(int value)
{
  ui->Turn->setText(QString::number(ui->bunch_scroll->value()*para->save_Gap));

  QwtSymbol * symbol_inner = new QwtSymbol(QwtSymbol::XCross,QBrush(Qt::green),QPen(Qt::blue),QSize(3,3));
  QwtPlotCurve *bunchout_inner = new QwtPlotCurve("Bunch");
  QwtPlotCurve *seppos_inner = new QwtPlotCurve("Seperatrix pos");
  QwtPlotCurve *sepneg_inner = new QwtPlotCurve("Seperatrix neg");
  QwtPlotCurve *voltage_inner = new QwtPlotCurve("voltage_innter");
  QwtPlotCurve *seppos_inner_stat = new QwtPlotCurve("Seperatrix pos");
  QwtPlotCurve *sepneg_inner_stat = new QwtPlotCurve("Seperatrix neg");
  QwtPlotCurve *curve1_2_inner = new QwtPlotCurve("Curve1_2");
  QwtPlotCurve *curve1_4_inner = new QwtPlotCurve("Curve1_4");

  QVector<double> histo_lauf_inner(para->histo_anz);
  for(int i=0; i<para->histo_anz; i++){
    histo_lauf_inner[i] = -para->pi+para->h*2*para->pi*(i+0.5)/para->histo_anz;
  }
  QVector<double> histo_lauf_inner_multi(para->histo_anz*para->h);
  for(int i=0; i<para->histo_anz*para->h; i++){
    histo_lauf_inner_multi[i] = -para->pi+2*para->pi*(i+0.5)/para->histo_anz;
  }

  curve1_2_inner->setSamples(histo_lauf_inner_multi,log_bunch->get_log_histogramm(value));
  curve1_2_inner->setPen(* new QPen(Qt::yellow));

  curve1_4_inner->setSamples(histo_lauf_inner_multi,log_bunch->get_log_histogramm_dis(value));
  curve1_4_inner->setPen(* new QPen(Qt::red));

  myplot1->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  curve1_2_inner->attach(myplot1);
  curve1_4_inner->attach(myplot1);
  myplot1->replot();

  myplot5->detachItems(QwtPlotItem::Rtti_PlotItem,true);
  bunchout_inner->setStyle(QwtPlotCurve::NoCurve);
  bunchout_inner->setSymbol(symbol_inner);
  bunchout_inner->setSamples(log_bunch->get_log_bunch_phi(value),log_bunch->get_log_bunch_DEo(value));
  bunchout_inner->attach(myplot5);

  //Referenzteilchenposition
  QwtPlotMarker* refpoint = new QwtPlotMarker();
  refpoint->setSymbol(new QwtSymbol(QwtSymbol::Rect,QBrush(Qt::green),Qt::NoPen,QSize(5,5) ) );
  refpoint->setValue(QPointF (log_ref->get_phi_1R()[value*para->save_Gap] , 0));
  refpoint->attach(myplot5);

  //Schwerpunkt der Verteilung
  QwtPlotMarker* centerom = new QwtPlotMarker();
  centerom->setSymbol(new QwtSymbol(QwtSymbol::Rect,QBrush(Qt::red),Qt::NoPen,QSize(5,5) ) );
  centerom->setValue(QPointF (log_bunch->get_moment_phi_1(0)[value*para->save_Gap],log_bunch->get_moment_DEo_1(0)[value*para->save_Gap] ));
  centerom->attach(myplot5);

  seppos_inner->setSamples(sep->get_phi_ring_sequence(),log_bunch->get_log_sep_pos(value));
  sepneg_inner->setSamples(sep->get_phi_ring_sequence(),log_bunch->get_log_sep_neg(value));
  voltage_inner->setSamples(sep->get_phi_ring_sequence(),log_bunch->get_log_voltage(value));
  seppos_inner_stat->setSamples(sep->get_phi_ring_sequence(),sep->get_sep_pos_static());
  sepneg_inner_stat->setSamples(sep->get_phi_ring_sequence(),sep->get_sep_neg_static());
  seppos_inner->attach(myplot5);
  sepneg_inner->attach(myplot5);
  voltage_inner->attach(myplot5);
  seppos_inner_stat->attach(myplot5);
  sepneg_inner_stat->attach(myplot5);
  myplot5->replot();
}

void MainGUI::on_Monitoring_clicked()
{
    (monit->isVisible()) ? monit->hide_GUI():monit->show_GUI();
}

void MainGUI::on_Parameter_clicked()
{
    (paraGUI->isVisible()) ? paraGUI->hide() : paraGUI->show();
}

void MainGUI::on_Regler_clicked()
{
}

void MainGUI::on_Rampe_u_Stoer_clicked()
{
}

void MainGUI::on_log_ref_value_editingFinished()
{
  int arg1 = ui->log_ref_value->value();
  log_ref->show_stat(arg1);
  qDebug()<<"show stat";
  ref->ausgabe_debug();
}
