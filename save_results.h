#ifndef SAVE_RESULTS_H
#define SAVE_RESULTS_H

#include <parameter.h>
#include <qfile.h>
#include <QDir>
#include <QDataStream>
#include <log_ref_i.h>

class save_Results
{
  private:
    QString filename;
    QDir dir;
    QFile *file;
    QTextStream *textStream;
  public:
    save_Results(Parameter & para);
    void add_actual_state(QString string);
};

#endif // SAVE_RESULTS_H
