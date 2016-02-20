#include "maingui.h"
#include <QApplication>
#include <dummytest.h>

int main(int argc, char *argv[])
{
  //dummytest *dummy = new dummytest();
  QApplication a(argc, argv);
  MainGUI w;
  w.show();

  return a.exec();
}
