#ifndef WIDGET_H
#define WIDGET_H

#include <QGridLayout>
#include <QTabWidget>

#include "atbash.h"
#include "caesar.h"
#include "richelieu.h"
#include "gronsfeld.h"
#include "visionary.h"
#include "playfair.h"
#include "analysis.h"
#include "scaling.h"

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

  QTabWidget* tab_widget;
  QGridLayout* glayout_main;

public:
  Crypt(QWidget* parent = nullptr);
  ~Crypt();
};
#endif // WIDGET_H
