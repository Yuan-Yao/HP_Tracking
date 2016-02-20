#ifndef MONITORING_H
#define MONITORING_H

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
#include "controller.h"
#include "qwt_plot_grid.h"

//possibility to get to see the saved timelines of several synchrotron and bunch variables
namespace Ui {
class Monitoring;
}

class Monitoring : public QWidget
{
    Q_OBJECT

  public:
    //explicit Monitoring(QWidget *parent = 0,QVector<double> & ,Log_Reference &, Log_Bunch &);
    Monitoring(QVector<double> & ,Log_Reference &, Log_Bunch &, Controller &, int h_);
    ~Monitoring();

  private:
    Ui::Monitoring *ui;

    const int h;

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

    QwtPlotGrid * grid_1;
    QwtPlotGrid * grid_2;
    QwtPlotGrid * grid_3;
    QwtPlotGrid * grid_4;
    QwtPlotGrid * grid_5;
    QwtPlotGrid * grid_6;
    QwtPlotGrid * grid_7;
    QwtPlotGrid * grid_8;
    QwtPlotGrid * grid_9;
    QwtPlotGrid * grid_10;
    QwtPlotGrid * grid_11;
    QwtPlotGrid * grid_12;

    QVector<double> intern_counter;
    Log_Reference intern_log_ref;
    Log_Bunch intern_log_bunch;
    Controller intern_control;
    bool init;

    void local_initialize(QComboBox &);

  public:
    void hide_GUI(){this->hide();}
    void show_GUI(){this->show();}
    QVector<double> get_plot_data(QString);
    void plot_Curve(QComboBox &, QwtPlot *, QwtPlotCurve *, QwtPlotGrid *);
    void plot_Curve_change(QString, QwtPlot *, QwtPlotCurve *,QwtPlotGrid *);
    void init_combo_box();

  private slots:
    void on_comboBox_1_currentIndexChanged(const QString &arg1);
    void on_comboBox_2_currentIndexChanged(const QString &arg1);
    void on_comboBox_3_currentIndexChanged(const QString &arg1);
    void on_comboBox_4_currentIndexChanged(const QString &arg1);
    void on_comboBox_5_currentIndexChanged(const QString &arg1);
    void on_comboBox_6_currentIndexChanged(const QString &arg1);
    void on_comboBox_10_currentIndexChanged(const QString &arg1);
    void on_comboBox_7_currentIndexChanged(const QString &arg1);
    void on_comboBox_8_currentIndexChanged(const QString &arg1);
    void on_comboBox_11_currentIndexChanged(const QString &arg1);
    void on_comboBox_9_currentIndexChanged(const QString &arg1);
    void on_comboBox_12_currentIndexChanged(const QString &arg1);
    void on_spin_end_editingFinished();
    void on_spin_begin_editingFinished();
    void on_h_spin_box_editingFinished();
};

#endif // MONITORING_H
