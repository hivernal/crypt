#include "rsa.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QBuffer>

const QString rsaPrivateKey = "rsa.priv";
const QString rsaPublicKey = "rsa.pub";

Rsa::Rsa() {
  glayout = new QGridLayout(this);

  labelTextIn = new QLabel("Original");
  teditTextIn = new QTextEdit();
  connect(teditTextIn, &QTextEdit::textChanged, this, &Rsa::teditTextInChanged);
  labelTextOut = new QLabel("Result");
  teditTextOut = new QTextEdit();
  
  labelFileIn = new QLabel("Input file");
  leditFileIn = new QLineEdit();
  pbuttonFileIn = new QPushButton("Open");
  connect(pbuttonFileIn, &QPushButton::clicked, this, &Rsa::pbuttonFileInClicked);
  labelFileOut = new QLabel("Output file");
  leditFileOut = new QLineEdit();
  pbuttonFileOut = new QPushButton("Open");
  connect(pbuttonFileOut, &QPushButton::clicked, this, &Rsa::pbuttonFileOutClicked);

  labelOperation = new QLabel("Operation");
  cboxOperation = new QComboBox();
  cboxOperation->addItem("Encrypt");
  cboxOperation->addItem("Decrypt");
  connect(cboxOperation, &QComboBox::currentIndexChanged, this, &Rsa::teditTextInChanged);
  pbuttonRun = new QPushButton("Run");
  connect(pbuttonRun, &QPushButton::clicked, this, &Rsa::pbuttonRunClicked);

  labelKey = new QLabel("Key");
  sboxKey = new QSpinBox();
  sboxKey->setMinimum(4);
  sboxKey->setMaximum(2048);
  sboxKey->setValue(512);
  sboxKey->setMinimumWidth(100);
  sboxKey->setAlignment(Qt::AlignCenter);
  connect(sboxKey, &QSpinBox::valueChanged, this, &Rsa::teditTextInChanged);
  pbuttonGenKey = new QPushButton("Generate");
  connect(pbuttonGenKey, &QPushButton::clicked, this, &Rsa::pbuttonGenKeyClicked);
  QHBoxLayout* hlayoutKey = new QHBoxLayout();
  hlayoutKey->addWidget(sboxKey);
  hlayoutKey->addWidget(pbuttonGenKey);
  hlayoutKey->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding,
                                            QSizePolicy::Fixed));

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
  glayout->addWidget(labelOperation, 4, 0);
  glayout->addWidget(cboxOperation, 4, 1);
  glayout->addWidget(pbuttonRun, 4, 2);
  glayout->addWidget(labelKey, 5, 0);
  glayout->addLayout(hlayoutKey, 5, 1);
}

void Rsa::pbuttonFileInClicked() {
  leditFileIn->setText(QFileDialog::getOpenFileName());
}

void Rsa::pbuttonFileOutClicked() {
  leditFileOut->setText(QFileDialog::getOpenFileName());
}

void Rsa::pbuttonGenKeyClicked() {
  mpz_class p, q, n, f, e, d;
  gmp_randclass rand(gmp_randinit_mt);
  rand.seed(time(NULL));
  do
    p = rand.get_z_bits(sboxKey->value());
  while (!mpz_probab_prime_p(p.get_mpz_t(), 25));
  do
    q = rand.get_z_bits(sboxKey->value());
  while (!mpz_probab_prime_p(q.get_mpz_t(), 25) || p == q);
  n = p * q;
  f = (p - 1) * (q - 1);
  do
    e = rand.get_z_range(f);
  while (gcd(f, e) != 1 || e < 2);
  mpz_invert(d.get_mpz_t(), e.get_mpz_t(), f.get_mpz_t());

  QFile rsaPublicFile(rsaPublicKey);
  if (!rsaPublicFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    QMessageBox::critical(this, "Generating key", "Error to open public rsa file");
    return;
  }
  QFile rsaPrivateFile(rsaPrivateKey);
  if (!rsaPrivateFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    QMessageBox::critical(this, "Generating key", "Error to open private rsa file");
    rsaPublicFile.close();
    return;
  }

  rsaPublicFile.write(mpz_get_str(NULL, 10, e.get_mpz_t()));
  rsaPublicFile.putChar('\n');
  rsaPublicFile.write(mpz_get_str(NULL, 10, n.get_mpz_t()));
  rsaPublicFile.close();
  rsaPrivateFile.write(mpz_get_str(NULL, 10, d.get_mpz_t()));
  rsaPrivateFile.putChar('\n');
  rsaPrivateFile.write(mpz_get_str(NULL, 10, n.get_mpz_t()));
  rsaPrivateFile.close();
  QMessageBox::information(this, "Generating key", "Key was generated");
}

void Rsa::pbuttonRunClicked() {
  mpz_class eord;
  mpz_class n;
  QString rsaFileName = cboxOperation->currentIndex() ? rsaPrivateKey : 
                                                        rsaPublicKey;
  if (readKey(rsaFileName, eord, n)) {
    QMessageBox::critical(this, "Key", "Key was not generated");
    return;
  }

  QFile inputFile(leditFileIn->text());
  if (!inputFile.open(QIODeviceBase::ReadOnly)) {
    QMessageBox::critical(this, "Input file", "Error to open input file");
    return;
  }
  QByteArray data = inputFile.readAll();
  inputFile.close();
  if (mpz_sizeinbase(n.get_mpz_t(), 10) * data.size() > 6400000 &&
      !cboxOperation->currentIndex()) {
    QMessageBox::warning(this, "Input file", "Input file is too big for this key");
    return;
  }
  QFile outputFile(leditFileOut->text());
  if (!outputFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    QMessageBox::critical(this, "Output file", "Error to open Output file");
    return;
  }

  if (cboxOperation->currentIndex()) {
    for (char ch : data) {
      if (ch != '\n' && (ch < '0' || ch > '9')) {
        teditTextOut->setText("Incorrect input!");
        outputFile.close();
        return;
      }
    }
    rsaFile(data, outputFile, eord, n, true);
  } else
    rsaFile(data, outputFile, eord, n, false);
  outputFile.close();
}

void Rsa::teditTextInChanged() {
  QString text = teditTextIn->toPlainText();
  if (text.isEmpty())
    return;

  mpz_class eord;
  mpz_class n;
  QString rsaFileName = cboxOperation->currentIndex() ? rsaPrivateKey :
                                                        rsaPublicKey;
  if (readKey(rsaFileName, eord, n)) {
    QMessageBox::critical(this, "Key", "Key was not generated");
    return;
  }
  if (mpz_sizeinbase(n.get_mpz_t(), 10) * text.size() > 2400000 &&
      !cboxOperation->currentIndex()) {
    QMessageBox::warning(this, "Input text", "Input text is too big for this key");
    return;
  }

  QString result;
  if (cboxOperation->currentIndex()) {
    for (QChar ch : text) {
      if (ch != '\n' && (ch < '0' || ch > '9')) {
        teditTextOut->setText("Incorrect input!");
        return;
      }
    }
    rsaText(text, result, eord, n, true);
  } else
    rsaText(text, result, eord, n, false);
  teditTextOut->setText(result);
}

void Rsa::rsaFile(QByteArray& data, QFile& outputFile, const mpz_class& eord, 
                  const mpz_class& n, bool isDecrypt) {
  QBuffer buffer(&data);
  buffer.open(QIODeviceBase::ReadOnly);
  QProgressDialog progress("Encryption...", "cancel", 0, 100, this);
  progress.setModal(true);
  progress.setMinimumDuration(250);
  if (!isDecrypt) {
    mpz_class num;
    while (!buffer.atEnd()) {
      progress.setValue(buffer.pos() * 100 / buffer.size());
      if (progress.wasCanceled())
        break;
      char ch;
      buffer.getChar(&ch);
      num = (unsigned char) ch;
      mpz_powm(num.get_mpz_t(), num.get_mpz_t(), eord.get_mpz_t(), n.get_mpz_t());
      outputFile.write(mpz_get_str(NULL, 10, num.get_mpz_t()));
      outputFile.putChar('\n');
    }
  } else {
    mpz_class num;
    while (!buffer.atEnd()) {
      progress.setValue(buffer.pos() * 100 / buffer.size());
      if (progress.wasCanceled())
        break;
      num = buffer.readLine().toStdString();
      mpz_powm(num.get_mpz_t(), num.get_mpz_t(), eord.get_mpz_t(), n.get_mpz_t());
      outputFile.putChar((char) num.get_ui());
    }
  }
  progress.setValue(100);
  QMessageBox::information(this, "Encryption", "Encryption completed");
}

void Rsa::rsaText(QString text, QString& result, const mpz_class& eord,
                  const mpz_class& n, bool isDecrypt) {

  QProgressDialog progress("Encryption...", "cancel", 0, 100, this);
  progress.setModal(true);
  progress.setMinimumDuration(250);
  QTextStream textStream(&text);
  if (!isDecrypt) {
    mpz_class num;
    while (!textStream.atEnd()) {
      progress.setValue(textStream.pos() * 100 / text.size());
      if (progress.wasCanceled())
        break;
      QChar ch;
      textStream >> ch;
      num = ch.unicode();
      mpz_powm(num.get_mpz_t(), num.get_mpz_t(), eord.get_mpz_t(), n.get_mpz_t());
      result += mpz_get_str(NULL, 10, num.get_mpz_t());
      result += '\n';
    }
  } else {
    mpz_class num;
    while (!textStream.atEnd()) {
      progress.setValue(textStream.pos() * 100 / text.size());
      if (progress.wasCanceled())
        break;
      num = textStream.readLine().toStdString();
      mpz_powm(num.get_mpz_t(), num.get_mpz_t(), eord.get_mpz_t(), n.get_mpz_t());
      result += (ushort) num.get_ui();
    }
  }
  progress.setValue(100);
}

int Rsa::readKey(QString rsaFileName, mpz_class& eord, mpz_class& n) {
  QFile rsaFile(rsaFileName);
  if (!rsaFile.open(QIODeviceBase::ReadOnly))
    return EXIT_FAILURE;
  eord.set_str(rsaFile.readLine().toStdString(), 10);
  n.set_str(rsaFile.readLine().toStdString(), 10);
  rsaFile.close();
  if (eord == 0 || n == 0)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
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
  delete labelOperation;
  delete cboxOperation;
  delete glayout;
}

QWidget* Rsa::getWidget() {
  return this;
}
