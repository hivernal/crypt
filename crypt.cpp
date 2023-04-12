#include "crypt.h"

Crypt::Crypt(QWidget* parent) : QWidget(parent) {
  this->setWindowTitle("Encryption");
  // this->setMaximumSize(700, 500);

  tab_widget = new QTabWidget();

  atbash = new Atbash();
  tab_widget->addTab(atbash->getWidget(), "Atbash");

  caesar = new Caesar();
  tab_widget->addTab(caesar->getWidget(), "Caesar");

  richelieu = new Richelieu();
  tab_widget->addTab(richelieu->getWidget(), "Richelieu");

  gronsfeld = new Gronsfeld();
  tab_widget->addTab(gronsfeld->getWidget(), "Gronsfeld");

  visionary = new Visionary();
  tab_widget->addTab(visionary->getWidget(), "Visionary");

  playfair = new Playfair();
  tab_widget->addTab(playfair->getWidget(), "Playfair");

  analysis = new Analysis();
  tab_widget->addTab(analysis->getWidget(), "Analysis");

  glayout_main = new QGridLayout(this);
  glayout_main->addWidget(tab_widget);
}

Crypt::~Crypt() {
  delete atbash;
  delete caesar;
  delete richelieu;
  delete gronsfeld;
  delete visionary;
  delete playfair;
  delete analysis;

  delete glayout_main;
  delete tab_widget;
}
