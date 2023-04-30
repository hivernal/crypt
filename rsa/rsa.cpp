#include "rsa.h"
#include <QFileDialog>
#include <QMessageBox>
#include <gmp.h>
#include <time.h>

#define KEY_RSA_FILE "key.rsa"

Rsa::Rsa() {
  widget = new QWidget();
  glayout = new QGridLayout(widget);

  labelTextIn = new QLabel("Original");
  teditTextIn = new QTextEdit();
  glayout->addWidget(labelTextIn, 0, 0);
  glayout->addWidget(teditTextIn, 0, 1, 1, 2);

  labelTextOut = new QLabel("Result");
  teditTextOut = new QTextEdit();
  glayout->addWidget(labelTextOut, 1, 0);
  glayout->addWidget(teditTextOut, 1, 1, 1, 2);
  
  labelFileIn = new QLabel("Input file");
  leditFileIn = new QLineEdit();
  pbuttonFileIn = new QPushButton("Open");
  connect(pbuttonFileIn, &QPushButton::clicked, this, &Rsa::pbuttonFileInClicked);
  glayout->addWidget(labelFileIn, 2, 0);
  glayout->addWidget(leditFileIn, 2, 1);
  glayout->addWidget(pbuttonFileIn, 2, 2);

  labelFileOut = new QLabel("Output file");
  leditFileOut = new QLineEdit();
  pbuttonFileOut = new QPushButton("Open");
  connect(pbuttonFileOut, &QPushButton::clicked, this, &Rsa::pbuttonFileOutClicked);
  glayout->addWidget(labelFileOut, 3, 0);
  glayout->addWidget(leditFileOut, 3, 1);
  glayout->addWidget(pbuttonFileOut, 3, 2);

  pbuttonGenKey = new QPushButton("Generate");
  connect(pbuttonGenKey, &QPushButton::clicked, this, &Rsa::pbuttonGenKeyClicked);
  pbuttonEncrypt = new QPushButton("Encrypt");
  // connect(pbuttonEncrypt, &QPushButton::clicked, this, &Rsa::pbuttonEncryptClicked);
  pbuttonDecrypt = new QPushButton("Decrypt");
  // connect(pbuttonDecrypt, &QPushButton::clicked, this, &Rsa::pbuttonDecryptClicked);
  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
  hlayout->addWidget(pbuttonGenKey);
  hlayout->addWidget(pbuttonEncrypt);
  hlayout->addWidget(pbuttonDecrypt);
  glayout->addLayout(hlayout, 4, 1, 1, 2);
}

void Rsa::pbuttonFileInClicked() {
  leditFileIn->setText(QFileDialog::getOpenFileName());
}

void Rsa::pbuttonFileOutClicked() {
  leditFileOut->setText(QFileDialog::getOpenFileName());
}

int Rsa::openFile(QFile& file, QString fileName, QIODeviceBase::OpenMode mode) {
  if (fileName.isEmpty() || fileName[0] == ' ' || fileName.last(1) == ' ')
    return EXIT_FAILURE;
  file.setFileName(fileName);
  if (file.isOpen())
    return EXIT_FAILURE;
  if (!file.open(mode))
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

void Rsa::pbuttonGenKeyClicked() {
  mpz_t p, q;
  mpz_inits(p, q);
  gmp_randstate_t randstate;
  gmp_randinit_mt(randstate);
  gmp_randseed_ui(randstate, time(NULL));
  do
    mpz_rrandomb(p, randstate, 2048);
  while (!mpz_probab_prime_p(p, 25));
  do
    mpz_rrandomb(q, randstate, 2048);
  while (!mpz_probab_prime_p(q, 25) || !mpz_cmp(p, q));

  FILE* rsaFile;
  if ((rsaFile = fopen(KEY_RSA_FILE, "w")) == NULL) {
    QMessageBox::critical(this, "Generating key", "Error to generate key");
    return;
  }
  mpz_out_str(rsaFile, 10, p);
  fputc('\n', rsaFile);
  mpz_out_str(rsaFile, 10, q);
  fclose(rsaFile);

  mpz_clear(p);
  mpz_clear(q);
  gmp_randclear(randstate);
  QMessageBox::information(this, "Generating key", "Key was generated");
}

Rsa::~Rsa() {
  delete labelTextIn;
  delete teditTextIn;
  delete labelTextOut;
  delete teditTextOut;
  delete labelFileIn;
  delete leditFileIn;
  delete pbuttonFileIn;
  delete labelFileOut;
  delete leditFileOut;
  delete pbuttonFileOut;
  delete glayout;
  delete widget;
}

QWidget* Rsa::getWidget() {
  return widget;
}
