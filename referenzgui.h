#ifndef REFERENZGUI_H
#define REFERENZGUI_H
#include <QWidget>
#include <QComboBox>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QStringList>
#include <QString>
#include <QVector>
#include "log_reference.h"
#include "log_bunch.h"
#include <QObject>

class ReferenzGUI
{

  private:
    QWidget * ui;

    QwtPlot * plot_1;
    QwtPlot * plot_2;
    QwtPlot * plot_3;
    QwtPlot * plot_4;
    QwtPlot * plot_5;
    QwtPlot * plot_6;
    QwtPlot * plot_7;
    QwtPlot * plot_8;
    QwtPlot * plot_9;
    QwtPlot * plot_10;
    QwtPlot * plot_11;
    QwtPlot * plot_12;

    QStringList qstring;

    QComboBox * combo_1;
    QComboBox * combo_2;
    QComboBox * combo_3;
    QComboBox * combo_4;
    QComboBox * combo_5;
    QComboBox * combo_6;
    QComboBox * combo_7;
    QComboBox * combo_8;
    QComboBox * combo_9;
    QComboBox * combo_10;
    QComboBox * combo_11;
    QComboBox * combo_12;

    QwtPlotCurve * curve_1;
    QwtPlotCurve * curve_2;
    QwtPlotCurve * curve_3;
    QwtPlotCurve * curve_4;
    QwtPlotCurve * curve_5;
    QwtPlotCurve * curve_6;
    QwtPlotCurve * curve_7;
    QwtPlotCurve * curve_8;
    QwtPlotCurve * curve_9;
    QwtPlotCurve * curve_10;
    QwtPlotCurve * curve_11;
    QwtPlotCurve * curve_12;

    QVector<double> intern_counter;
    Log_Reference intern_log_ref;
    Log_Bunch intern_log_bunch;

    void local_initialize(QComboBox &);

  private slots:
    void plot_single_curve_1();

  public:
    ReferenzGUI(QVector<double> & ,Log_Reference &, Log_Bunch &);
    void hide_GUI(){ui->hide();}
    QVector<double> get_plot_data(QString);
    void plot_Curve(QComboBox &, QwtPlot *, QwtPlotCurve *);
    void init_combo_box();

};

#endif // REFERENZGUI_H
