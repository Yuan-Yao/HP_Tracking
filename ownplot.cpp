#include "ownplot.h"

ownPlot::ownPlot(QWidget * qw_,double xpos_, double ypos_, double width_, double height_):
  plot(new QwtPlot(qw_)),
  grid(nullptr),
  curve(nullptr),
  curve2(nullptr),
  functionCurve(nullptr)
{
  plot->setFixedHeight(height_);
  plot->setFixedWidth(width_);
  plot->move(xpos_,ypos_);
}

//delete Plot and reset curve, grid and plot
void ownPlot::emptyPlot(){
  QwtPlotCurve * tempc = nullptr;
  QwtPlotCurve * tempc2 = nullptr;
  QwtPlotCurve * tempfc = nullptr;
  QwtPlotGrid * tempg = nullptr;

  if (curve != nullptr) {
    QwtPlotCurve * tempc = curve;
  }
  if(curve2 != nullptr){
    QwtPlotCurve * tempc2 = curve2;
  }
  if (functionCurve != nullptr) {
    QwtPlotCurve * tempfc = functionCurve;
  }
  if (grid != nullptr) {
    QwtPlotGrid * tempg = grid;
  }

  curve = new QwtPlotCurve();
  curve2 = new QwtPlotCurve();
  functionCurve = new QwtPlotCurve();
  grid = new QwtPlotGrid();

  qDebug() << (curve == nullptr) << (curve2 == nullptr) << (functionCurve == nullptr) << (grid == nullptr);

  plot->detachItems(QwtPlotItem::Rtti_PlotItem,true);

  if (tempc != nullptr) delete tempc;
  if (tempc2 != nullptr) delete tempc2;
  if (tempg != nullptr) delete tempg;
  if (tempfc != nullptr) delete tempfc;
}

//for only one input vector, create a default x vector with spacing one
void ownPlot::plotY(QVector<double> yVector_){
  emptyPlot();
  if(!xForOnlyY.empty()) xForOnlyY.clear();
  xForOnlyY = QVector<double>(yVector_.size());
  for (int i = 0; i < yVector_.size(); i++){
    xForOnlyY[i] = i;
  }
  curve->setSamples(xForOnlyY,yVector_);
  curve->attach(plot);
  grid->setPen(QPen(QColor(196,196,196)));
  grid->attach(plot);
  plot->replot();
}

//for two input vector, set x-range to the accordingly
void ownPlot::plotXY(QVector<double> xVector_, QVector<double> yVector_){
  emptyPlot();
  curve->setSamples(xVector_,yVector_);
  curve->attach(plot);
  grid->setPen(QPen(QColor(196,196,196)));
  grid->attach(plot);
  plot->replot();
}
//for second two input vector, set x-range to the accordingly
void ownPlot::plotXY2(QVector<double> xVector_, QVector<double> yVector_){
  curve2->setSamples(xVector_,yVector_);
  curve2->attach(plot);
  plot->replot();
}


//for plotting an exponential equation of form A*exp(- lamba t)+b
void ownPlot::plotExp(double A_, double lambda_, double b_, double t_b, double t_e, int fitValues_){
  functionVectorX.push_back(QVector<double>(0));
  functionVectorY.push_back(QVector<double>(0));
  functionCurves.push_back(new QwtPlotCurve());

    if (fitValues_ > 1){
      int temp = (fitValues_-1)*10;
      for (int i = 0; i < temp + 1;i++){
        functionVectorX.back().push_back(t_b + (t_e-t_b)/(temp)*i);
        functionVectorY.back().push_back(A_* exp(-lambda_ * (1.0/temp*(fitValues_-1)*i))+b_);
      }
      qDebug() << functionVectorX.back().size() << functionVectorY.back().size();
      //for (int i = 0; i < functionVectorX.back().size(); i++) qDebug() << functionVectorX.back()[i] << functionVectorY.back()[i];
      functionCurves.back()->setSamples(functionVectorX.back(),functionVectorY.back());
      functionCurves.back()->attach(plot);
      plot->replot();
    }
}
