#ifndef RAMP_DIST_GUI_H
#define RAMP_DIST_GUI_H

#include <QWidget>

namespace Ui {
class Ramp_Dist_GUI;
}

class Ramp_Dist_GUI : public QWidget
{
    Q_OBJECT

  public:
    explicit Ramp_Dist_GUI(QWidget *parent = 0);
    ~Ramp_Dist_GUI();

  private:
    Ui::Ramp_Dist_GUI *ui;
};

#endif // RAMP_DIST_GUI_H
