#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QProgressDialog>

#include "hellman.h"

#define HELLMAN_PRIVATE_KEY "hellman.priv"
#define HELLMAN_PUBLIC_KEY "hellman.pub"

Hellman::Hellman() {
  glayout = new QGridLayout(this);
  
  labelTextIn = new QLabel("Original");
  teditTextIn = new QTextEdit();
  labelTextOut = new QLabel("Result");
  teditTextOut = new QTextEdit();

  labelFileIn = new QLabel("Input file");
  leditFileIn = new QLineEdit();
  pbuttonFileIn = new QPushButton("Open");
  labelFileOut = new QLabel("Output file");
  leditFileOut = new QLineEdit();
  pbuttonFileOut = new QPushButton("Open");

  pbuttonRun = new QPushButton("Run");

  labelKey = new QLabel("Key");
  hlayoutKey = new QHBoxLayout();
  labelKeySize = new QLabel("Size:");
  labelKeySize->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxKeySize = new QSpinBox();
  sboxKeySize->setRange(4, 2048);
  sboxKeySize->setAlignment(Qt::AlignCenter);
  sboxKeySize->setMinimumWidth(50);
  sboxKeySize->setValue(512);
  labelG = new QLabel("G:");
  labelG->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxG = new QSpinBox();
  sboxG->setRange(1, INT_MAX);
  sboxG->setAlignment(Qt::AlignCenter);
  sboxG->setMinimumWidth(50);
  labelP = new QLabel("P:");
  labelP->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxP = new QSpinBox();
  sboxP->setRange(1, INT_MAX);
  sboxP->setAlignment(Qt::AlignCenter);
  sboxP->setMinimumWidth(50);
  pbuttonGenerate = new QPushButton("Generate");
  hlayoutKey->addWidget(labelKeySize);
  hlayoutKey->addWidget(sboxKeySize);
  hlayoutKey->addWidget(labelG);
  hlayoutKey->addWidget(sboxG);
  hlayoutKey->addWidget(labelP);
  hlayoutKey->addWidget(sboxP);
  hlayoutKey->addWidget(pbuttonGenerate);

  glayout->addWidget(labelTextIn, 0, 0);
  glayout->addWidget(teditTextIn, 0, 1, 1, 2);
  glayout->addWidget(labelTextOut, 1, 0);
  glayout->addWidget(teditTextOut, 1, 1, 1, 2);
  glayout->addWidget(labelFileIn, 2, 0);
  glayout->addWidget(leditFileIn, 2, 1);
  glayout->addWidget(pbuttonFileIn, 2, 2);
  glayout->addWidget(labelFileOut, 3, 0);
  glayout->addWidget(leditFileOut, 3, 1);
  glayout->addWidget(pbuttonFileOut, 3, 2);
  glayout->addWidget(labelKey, 4, 0);
  glayout->addLayout(hlayoutKey, 4, 1, 1, 2);
  glayout->addWidget(pbuttonRun, 5, 2);

  connect(pbuttonFileIn, &QPushButton::clicked, this, &Hellman::pbuttonFileInClicked);
  connect(pbuttonFileOut, &QPushButton::clicked, this, &Hellman::pbuttonFileOutClicked);
  connect(pbuttonGenerate, &QPushButton::clicked, this, &Hellman::pbuttonGenerateClicked);
  connect(pbuttonRun, &QPushButton::clicked, this, &Hellman::pbuttonRunClicked);
  connect(teditTextIn, &QTextEdit::textChanged, this, &Hellman::teditTextInChanged);
}

void Hellman::pbuttonFileInClicked() {
  leditFileIn->setText(QFileDialog::getOpenFileName());
}

void Hellman::pbuttonFileOutClicked() {
  leditFileOut->setText(QFileDialog::getOpenFileName());
}

void Hellman::pbuttonGenerateClicked() {
  mpz_class g, p;
  g = sboxG->value();
  p = sboxP->value();
  if (g >= p || mpz_probab_prime_p(g.get_mpz_t(), 25) != 2 || 
      mpz_probab_prime_p(p.get_mpz_t(), 25) != 2) {
    QMessageBox::critical(this, "G and P", "G and P must be simple. G lower than P");
    return;
  }

  mpz_class aPrivate;
  gmp_randclass rand(gmp_randinit_mt);
  int keySize = sboxKeySize->value();
  aPrivate = rand.get_z_bits(keySize);
  mpz_class bPrivate;
  do
    bPrivate = rand.get_z_bits(keySize);
  while (bPrivate == aPrivate);

  mpz_class aPublic;
  mpz_powm(aPublic.get_mpz_t(), g.get_mpz_t(), aPrivate.get_mpz_t(), p.get_mpz_t());
  mpz_class bPublic;
  mpz_powm(bPublic.get_mpz_t(), g.get_mpz_t(), bPrivate.get_mpz_t(), p.get_mpz_t());
  mpz_class k;
  mpz_powm(k.get_mpz_t(), bPublic.get_mpz_t(), aPrivate.get_mpz_t(), p.get_mpz_t());

  QFile publicKeyFile(HELLMAN_PUBLIC_KEY);
  if (!publicKeyFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    QMessageBox::critical(this, "Key", "Error to generate key");
    return;
  }
  QFile privateKeyFile(HELLMAN_PRIVATE_KEY);
  if (!privateKeyFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    publicKeyFile.close();
    QMessageBox::critical(this, "Key", "Error to generate key");
    return;
  }
  publicKeyFile.write(mpz_get_str(NULL, 10, g.get_mpz_t()));
  publicKeyFile.putChar('\n');
  publicKeyFile.write(mpz_get_str(NULL, 10, p.get_mpz_t()));
  publicKeyFile.putChar('\n');
  publicKeyFile.write(mpz_get_str(NULL, 10, aPublic.get_mpz_t()));
  publicKeyFile.putChar('\n');
  publicKeyFile.write(mpz_get_str(NULL, 10, bPublic.get_mpz_t()));
  publicKeyFile.close();
  privateKeyFile.write(mpz_get_str(NULL, 10, aPrivate.get_mpz_t()));
  privateKeyFile.putChar('\n');
  privateKeyFile.write(mpz_get_str(NULL, 10, k.get_mpz_t()));
  privateKeyFile.close();
  QMessageBox::information(this, "Key", "Key was generated");
}

int Hellman::readKeyFile(mpz_class& aPrivate, mpz_class& k) {
  QFile privateKeyFile(HELLMAN_PRIVATE_KEY);
  if (!privateKeyFile.open(QIODeviceBase::ReadOnly)) {
    QMessageBox::critical(this, "Key", "Error to read key file");
    return EXIT_FAILURE;
  }
  aPrivate.set_str(privateKeyFile.readLine().toStdString(), 10);
  k.set_str(privateKeyFile.readLine().toStdString(), 10);
  privateKeyFile.close();
  if (aPrivate == 0 || k == 0) {
    QMessageBox::critical(this, "Key", "Incorrect key");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void Hellman::pbuttonRunClicked() {
  mpz_class aPrivate, k;
  if (readKeyFile(aPrivate, k))
    return;

  QFile inputFile(leditFileIn->text());
  if (!inputFile.open(QIODeviceBase::ReadOnly)) {
    QMessageBox::critical(this, "Input file", "Error to open input file");
    return;
  }
  QByteArray data = inputFile.readAll();
  inputFile.close();
  QFile outputFile(leditFileOut->text());
  if (!outputFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    QMessageBox::critical(this, "Output file", "Error to open output file");
    return;
  }
  hellmanFile(data, outputFile, k.get_si());
  outputFile.close();
}

void Hellman::hellmanFile(QByteArray& data, QFile& outputFile, int key) {
  QBuffer buffer(&data);
  buffer.open(QIODeviceBase::ReadOnly);
  char ch;
  QProgressDialog progress("Encryption...", "cancel", 0, 100, this);
  progress.setModal(true);
  progress.setMinimumDuration(250);
  while (!buffer.atEnd()) {
    progress.setValue(buffer.pos() * 100 / buffer.size());
    if (progress.wasCanceled())
      break;
    buffer.getChar(&ch);
    ch = ch ^ key;
    outputFile.putChar(ch);
  }
  progress.setValue(100);
  QMessageBox::information(this, "Encryption", "Encryption completed");
}

void Hellman::teditTextInChanged() {
  mpz_class aPrivate, k;
  if (readKeyFile(aPrivate, k))
    return;

  QString text = teditTextIn->toPlainText();
  if (text.isEmpty())
    return;
  QString result;
  hellmanText(text, result, k.get_si());
  teditTextOut->setText(result);
}

void Hellman::hellmanText(QString text, QString& result, int key) {
  for (qsizetype i = 0; i < text.length(); ++i)
    text[i] = (QChar) (text[i].unicode() ^ key);
  result = text;
}

QWidget* Hellman::getWidget() {
  return this;
}

Hellman::~Hellman() {
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

  delete pbuttonRun;

  delete labelKey;
  delete labelKeySize;
  delete sboxKeySize;
  delete labelG;
  delete sboxG;
  delete labelP;
  delete sboxP;
  delete pbuttonGenerate;
  delete hlayoutKey;

  delete glayout;
}
