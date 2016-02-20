#ifndef OWNPLOT_H
#define OWNPLOT_H

#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qvector.h>
#include <qwidget.h>

class ownPlot
{
  private:
    QwtPlot *plot;
    QwtPlotGrid *grid;
    QwtPlotCurve *curve;
    QwtPlotCurve *curve2;
    QwtPlotCurve *functionCurve;
    void emptyPlot();
    QVector<double> xForOnlyY;

    QVector< QVector<double> > functionVectorX;
    QVector< QVector<double> > functionVectorY;
    QVector< QwtPlotCurve* > functionCurves;

  public:
    ownPlot(QWidget * qw,double xpos, double ypos, double width, double height);
    void plotY(QVector<double> yVector);
    void plotXY(QVector<double> xVector, QVector<double> yVector);
    void plotXY2(QVector<double> xVector, QVector<double> yVector);
    void plotExp(double A,double lambda, double b, double t_b, double t_e, int fitValues);
};

#endif // OWNPLOT_H
