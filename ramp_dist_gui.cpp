#include "ramp_dist_gui.h"
#include "ui_ramp_dist_gui.h"

Ramp_Dist_GUI::Ramp_Dist_GUI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Ramp_Dist_GUI)
{
  ui->setupUi(this);
}

Ramp_Dist_GUI::~Ramp_Dist_GUI()
{
  delete ui;
}
