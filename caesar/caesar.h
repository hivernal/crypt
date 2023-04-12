#ifndef CAESAR_H
#define CAESAR_H

#include "atbash.h"

#include <QComboBox>
#include <QSpinBox>

class Caesar : public Atbash {
  QLabel* labelOperation;
  QComboBox* cboxOperation;
  QLabel* labelStep;
  QSpinBox* sboxStep;

public:
  Caesar();
  ~Caesar();

public slots:
  void teditChanged();
};

#endif
