#ifndef GRONSFELD_H
#define GRONSFELD_H

#include "richelieu.h"

class Gronsfeld : public Richelieu {
public:
  Gronsfeld();
private slots:
  void teditInChanged();
};

#endif
