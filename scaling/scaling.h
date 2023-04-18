#ifndef SCALING_H
#define SCALING_H

#include "richelieu.h"
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>

class Scaling : public Richelieu {
  QLabel* labelOptions;
  QHBoxLayout* hlayoutOptions;

  QSpinBox* sboxX0;
  QLabel* labelX0;

  QSpinBox* sboxM;
  QLabel* labelM;

  QSpinBox* sboxC;
  QLabel* labelC;

  QSpinBox* sboxA;
  QLabel* labelA;

  QPushButton* pbuttonGen;

public:
  Scaling();
  ~Scaling();
private slots:
  void teditInChanged();
  void pbuttonGenClicked();
};

#endif
