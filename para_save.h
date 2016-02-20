#ifndef PARA_SAVE_H
#define PARA_SAVE_H

#include <paragui.h>
#include <qfile.h>
#include <QDir>
#include <QDataStream>
#include <QTextStream>
#include <qdebug.h>

class para_save
{
  private:
    QString filename;
    QDir dir;
    QFile *file;
    QTextStream *textStream;

  public:
    para_save();
    void load(ParaGUI & paragui);
    void save(ParaGUI & paragui);
    bool al_exist();
};

#endif // PARA_SAVE_H
