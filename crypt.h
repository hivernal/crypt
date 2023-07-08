#ifndef CRYPT_H
#define CRYPT_H

#include <QTabWidget>

#include "atbash.h"
#include "caesar.h"
#include "richelieu.h"
#include "gronsfeld.h"
#include "visionary.h"
#include "playfair.h"
#include "analysis.h"
#include "scaling.h"
#include "rsa.h"
#include "hellman.h"

class Crypt : public QWidget {
  Q_OBJECT

  Atbash* atbash;
  Caesar* caesar;
  Richelieu* richelieu;
  Gronsfeld* gronsfeld;
  Visionary* visionary;
  Playfair* playfair;
  Analysis* analysis;
  Scaling* scaling;
  Rsa* rsa;
  Hellman* hellman;

  QTabWidget* tabWidget;
  QGridLayout* glayoutMain;

public:
  Crypt();
  ~Crypt();
};

#endif
