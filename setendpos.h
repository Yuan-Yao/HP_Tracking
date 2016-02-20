#ifndef SETENDPOS_H
#define SETENDPOS_H
#include <QVector>
#include "bunch.h"
#include "referenz.h"
#include "parameter.h"

class SetEndPos
{
  private:
    QVector<int> color;
    QVector<double> sep_1;
    QVector<double> sep_2;
    QVector<double> sep_3;
    QVector<double> sep_4;
    QVector<double> sep_5;
    QVector<double> lauf;
  public:
    SetEndPos(Bunch &, Referenz &,Parameter &);
    QVector<int> get_color(){return color;}
};

#endif // SETENDPOS_H
