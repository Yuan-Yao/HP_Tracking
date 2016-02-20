#include "setendpos.h"

SetEndPos::SetEndPos(Bunch & bunch, Referenz & ref, Parameter & para):
  color(QVector<int>(para.anz_Teilchen)),
  sep_1(QVector<double>(200)),
  sep_2(QVector<double>(200)),
  sep_3(QVector<double>(200)),
  sep_4(QVector<double>(200)),
  sep_5(QVector<double>(200)),
  lauf(QVector<double>(200))
{
}
