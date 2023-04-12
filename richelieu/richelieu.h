#ifndef RICHELIEU_H
#define RICHELIEU_H

#include "atbash.h"

#include <QComboBox>

class Richelieu : public Atbash {
protected:
  QLabel* labelKey;
  QTextEdit* teditKey;
  QLabel* labelOperation;
  QComboBox* cboxOperation;

public:
  Richelieu();
  ~Richelieu();

private slots:
  void teditInChanged();
};

#endif
