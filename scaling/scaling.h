#ifndef SCALING_H
#define SCALING_H

#include "richelieu.h"
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFile>

class Scaling : public Richelieu {
  QLabel* labelOptions;
  QHBoxLayout* hlayoutOptions;
  QLabel* labelX0;
  QSpinBox* sboxX0;
  QLabel* labelM;
  QSpinBox* sboxM;
  QLabel* labelC;
  QSpinBox* sboxC;
  QLabel* labelA;
  QSpinBox* sboxA;
  QPushButton* pbuttonGen;

  QGridLayout* glayoutFiles;
  QLabel* labelFileIn;
  QLineEdit* leditFileIn;
  QPushButton* pbuttonFileIn;
  QLabel* labelFileOut;
  QLineEdit* leditFileOut;
  QPushButton* pbuttonFileOut;
  QPushButton* pbuttonRun;

  int openFile(QFile& file, QString fileName, QIODeviceBase::OpenMode mode);
  void scaling(QByteArray& text);
  int checkGamma();

public:
  Scaling();
  ~Scaling();
private slots:
  void teditInChanged();
  void pbuttonGenClicked();
  void pbuttonFileInClicked();
  void pbuttonFileOutClicked();
  void pbuttonRunClicked();
};

#endif
