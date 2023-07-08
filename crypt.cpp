#include "crypt.h"

Crypt::Crypt() {
  this->setWindowTitle("Encryption");
  tabWidget = new QTabWidget();
  atbash = new Atbash();
  tabWidget->addTab(atbash->getWidget(), "Atbash");
  caesar = new Caesar();
  tabWidget->addTab(caesar->getWidget(), "Caesar");
  richelieu = new Richelieu();
  tabWidget->addTab(richelieu->getWidget(), "Richelieu");
  gronsfeld = new Gronsfeld();
  tabWidget->addTab(gronsfeld->getWidget(), "Gronsfeld");
  visionary = new Visionary();
  tabWidget->addTab(visionary->getWidget(), "Visionary");
  playfair = new Playfair();
  tabWidget->addTab(playfair->getWidget(), "Playfair");
  analysis = new Analysis();
  tabWidget->addTab(analysis->getWidget(), "Analysis");
  scaling = new Scaling();
  tabWidget->addTab(scaling->getWidget(), "Scaling");
  rsa = new Rsa();
  tabWidget->addTab(rsa->getWidget(), "RSA");
  hellman = new Hellman();
  tabWidget->addTab(hellman->getWidget(), "Hellman");
  glayoutMain = new QGridLayout(this);
  glayoutMain->addWidget(tabWidget);
}

Crypt::~Crypt() {
  delete atbash;
  delete caesar;
  delete richelieu;
  delete gronsfeld;
  delete visionary;
  delete playfair;
  delete analysis;
  delete scaling;
  delete rsa;
  delete hellman;

  delete glayoutMain;
  delete tabWidget;
}
