#ifndef FREQ_DAMP_ANALYSER_H
#define FREQ_DAMP_ANALYSER_H

#include <QWidget>
#include <bunch.h>
#include <log_reference.h>
#include <log_ref_i.h>
#include <parameter.h>
#include <qdebug.h>
#include <qvector.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <distortion.h>
#include <exponential_fitter.h>
#include <ownplot.h>
#include <save_results.h>

namespace Ui {
class freq_Damp_Analyser;
}

class freq_Damp_Analyser : public QWidget
{
    Q_OBJECT

  public:
    freq_Damp_Analyser(Parameter & para, Log_Reference & log_ref, const std::vector<Bunch> & bunch, int begin, save_Results * save);
    ~freq_Damp_Analyser();
    void local_tracking();

  private:
    Ui::freq_Damp_Analyser *ui;
    Log_Reference & log_ref;
    Parameter & para;
    log_ref_i lri;
    std::vector<Bunch> bunch_vec;
    int begin;
    int end_max;
    QVector<double> time;
    QVector<double> phi;
    Distortion *dist;
    Exponential_Fitter * expo;
    Exponential_Fitter * expu;
    save_Results * save;

    //data for curve fitting
    QVector<double> maxima;
    QVector<double> minima;
    QVector<double> maxima_time;
    QVector<double> minima_time;
    QVector<double> frequency;
    QVector<double> frequency_time;
    int maxima_evaluate;
    int minima_evaluate;


    void show_results();
    void save_fits();
    void calculate_frequency();

};

#endif // FREQ_DAMP_ANALYSER_H
