#ifndef MAINGUI_H
#define MAINGUI_H
#include "parameter.h"
#include <QMainWindow>
#include <QString>
#include "referenz.h"
#include "qdebug.h"
#include "log_reference.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "time.h"
#include "qdebug.h"
#include "bunch.h"
#include "qwt_symbol.h"
#include "log_bunch.h"
#include "qscrollbar.h"
#include "monitoring.h"
#include "qwt_plot_marker.h"
#include <thread>
#include <chrono>
#include <ctime>
#include <paragui.h>
#include <distortion.h>
#include <histogramm.h>
#include <controller.h>
#include <seperatrix.h>
#include <freq_damp_analyser.h>
#include <log_ref_i.h>
#include <save_results.h>
#include <para_save.h>

namespace Ui {
class MainGUI;
}//namespace Ui::MainGUI erstellen

class MainGUI : public QMainWindow//MainGUI, Tochterklasse von QMianWindow
{
    Q_OBJECT// its for the signals and slots

  public:
    explicit MainGUI(QWidget *parent = 0);//explicit:Initialisierung muss explizit durch Konstruktor durchgeführt werden! Z.B. Formen wie "intA = 10" sind dann nicht erlaubt!
                                          //MainGUI(QWidget *parent = 0): MainGUI's Parent Ob will also be QWidget's
    ~MainGUI();//Object von MainGUI zerstören und entlassen

  private slots://basiclly private
    void on_Starte_Simulation_clicked();

    void on_bunch_scroll_valueChanged(int value);

    void on_Monitoring_clicked();

    void on_Parameter_clicked();

    void on_Regler_clicked();

    void on_Rampe_u_Stoer_clicked();

    void on_log_ref_value_editingFinished();

  private:
    Ui::MainGUI *ui;//under ui namespace a MainGUI pointer
    ParaGUI *paraGUI;
    Monitoring *monit;
    Parameter *para;
    int thread_count;
    Referenz *ref;
    Seperatrix *sep;
    Log_Reference *log_ref;
    Controller *control;
    std::vector<Bunch> bunch;//vector with type of "Bunch"
    Distortion *dist;
    Log_Bunch *log_bunch;
    log_ref_i *lri;
    Histogramm *hist;
    freq_Damp_Analyser *fda;
    save_Results * save;
    para_save * p_save;

    QwtPlot *myplot1;
    QwtPlot *myplot2;
    QwtPlot *myplot3;
    QwtPlot *myplot4;
    QwtPlot *myplot5;
    std::chrono::time_point<std::chrono::system_clock> start,end,gstart,gend,partialstart,partialend;


    void time_start();
    void time_end();
    void time_intervall();
    void partial_time(int & time);
    void tracking_simulation();
};

#endif // MAINGUI_H
