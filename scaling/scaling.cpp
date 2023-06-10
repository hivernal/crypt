#include "scaling.h"

#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

int gronsfeld(QString& text, QString key, bool isDecrypt);

Scaling::Scaling() {
  labelKey->setText("Gamma");

  labelFileIn = new QLabel("Input file");
  leditFileIn = new QLineEdit();
  pbuttonFileIn = new QPushButton("Open");
  labelFileOut = new QLabel("Output file");
  leditFileOut = new QLineEdit();
  pbuttonFileOut = new QPushButton("Open");
  pbuttonRun = new QPushButton("Run");

  labelOptions = new QLabel("Options");
  labelX0 = new QLabel("X0:");
  labelX0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxX0 = new QSpinBox();
  sboxX0->setAlignment(Qt::AlignCenter);
  sboxX0->setMaximum(INT_MAX);
  sboxX0->setMinimum(INT_MIN);
  sboxX0->setMinimumWidth(50);

  labelM = new QLabel("M:");
  labelM->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxM = new QSpinBox();
  sboxM->setAlignment(Qt::AlignCenter);
  sboxM->setMaximum(INT_MAX);
  sboxM->setMinimum(1);
  sboxM->setMinimumWidth(50);

  labelC = new QLabel("C:");
  labelC->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxC = new QSpinBox();
  sboxC->setAlignment(Qt::AlignCenter);
  sboxC->setMaximum(INT_MAX);
  sboxC->setMinimum(INT_MIN);
  sboxC->setMinimumWidth(50);

  labelA = new QLabel("A:");
  labelA->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxA = new QSpinBox();
  sboxA->setAlignment(Qt::AlignCenter);
  sboxA->setMaximum(INT_MAX);
  sboxA->setMinimum(INT_MIN);
  sboxA->setMinimumWidth(50);
  pbuttonGen = new QPushButton("Generate");

  hlayoutOptions = new QHBoxLayout();
  hlayoutOptions->addWidget(labelX0);
  hlayoutOptions->addWidget(sboxX0);
  hlayoutOptions->addWidget(labelM);
  hlayoutOptions->addWidget(sboxM);
  hlayoutOptions->addWidget(labelC);
  hlayoutOptions->addWidget(sboxC);
  hlayoutOptions->addWidget(labelA);
  hlayoutOptions->addWidget(sboxA);
  hlayoutOptions->addWidget(pbuttonGen);

  glayout->addWidget(teditIn, 0, 1, 1, 2);
  glayout->addWidget(teditOut, 1, 1, 1, 2);
  glayout->addWidget(teditKey, 2, 1, 1, 2);
  glayout->addWidget(labelFileIn, 3, 0);
  glayout->addWidget(leditFileIn, 3, 1);
  glayout->addWidget(pbuttonFileIn, 3, 2);
  glayout->addWidget(labelFileOut, 4, 0);
  glayout->addWidget(leditFileOut, 4, 1);
  glayout->addWidget(pbuttonFileOut, 4, 2);
  glayout->addWidget(labelOperation, 5, 0);
  glayout->addWidget(cboxOperation, 5, 1);
  glayout->addWidget(pbuttonRun, 5, 2);
  glayout->addWidget(labelOptions, 6, 0);
  glayout->addLayout(hlayoutOptions, 6, 1, 1, 2);

  connect(teditIn, &QTextEdit::textChanged, this,
          &Scaling::teditInChanged);
  connect(teditKey, &QTextEdit::textChanged, this,
          &Scaling::teditInChanged);
  connect(cboxOperation, &QComboBox::currentIndexChanged, this,
          &Scaling::teditInChanged);
  connect(pbuttonGen, &QPushButton::clicked, this,
          &Scaling::pbuttonGenClicked);
  connect(pbuttonFileIn, &QPushButton::clicked, this,
          &Scaling::pbuttonFileInClicked);
  connect(pbuttonFileOut, &QPushButton::clicked, this,
          &Scaling::pbuttonFileOutClicked);
  connect(pbuttonRun, &QPushButton::clicked, this,
          &Scaling::pbuttonRunClicked);
}

void Scaling::teditInChanged() {
  QString text = teditIn->toPlainText();
  bool isDecrypt = cboxOperation->currentIndex();
  if (gronsfeld(text, teditKey->toPlainText(), isDecrypt))
    teditOut->setText("Incorrect gamma!");
  else
    teditOut->setText(text);
}

void Scaling::pbuttonFileInClicked() {
  leditFileIn->setText(QFileDialog::getOpenFileName());
}

void Scaling::pbuttonFileOutClicked() {
  leditFileOut->setText(QFileDialog::getOpenFileName());
}

void Scaling::pbuttonRunClicked() {
  if (checkGamma()) {
    QMessageBox::critical(this, "Gamma", "Incorrect gamma");
    return;
  }

  QFile fileIn;
  if (openFile(fileIn, leditFileIn->text(), QIODeviceBase::ReadOnly |
               QIODeviceBase::ExistingOnly)) {
    QMessageBox::critical(this, "Input file", "Error to open '" +
                          leditFileIn->text() + "' file");
    return;
  }
  QByteArray text = fileIn.readAll();
  fileIn.close();

  QFile fileOut;
  if (openFile(fileOut, leditFileOut->text(), QIODeviceBase::WriteOnly |
               QIODeviceBase::Truncate)) {
    QMessageBox::critical(this, "Output file", "Error to open '" +
                          leditFileOut->text() + "' file");
    return;
  }

  scaling(text);
  fileOut.write(text);
  fileOut.close();
}

int Scaling::openFile(QFile& file, QString fileName,
                      QIODeviceBase::OpenMode mode) {
  if (fileName.isEmpty() || fileName[0] == ' ' || fileName.last(1) == ' ')
    return EXIT_FAILURE;
  file.setFileName(fileName);
  if (file.isOpen())
    return EXIT_FAILURE;
  if (!file.open(mode))
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int Scaling::checkGamma() {
  QString gamma = teditKey->toPlainText();
  if (gamma.isEmpty())
    return EXIT_FAILURE;
  QStringList steps = gamma.split(',', Qt::SkipEmptyParts);
  for (QString num : steps) {
    bool toIntIsOk;
    num.toInt(&toIntIsOk);
    if (!toIntIsOk)
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void Scaling::scaling(QByteArray& data) {
  bool isDecrypt = cboxOperation->currentIndex();
  QString gamma = teditKey->toPlainText();
  QStringList steps = gamma.split(',', Qt::SkipEmptyParts);
  QProgressDialog progress("Encryption...", "cancel", 0, 100, this);
  progress.setModal(true);
  progress.setMinimumDuration(250);
  for (qsizetype i = 0, j = 0; i < data.length(); ++i, ++j) {
    progress.setValue(i * 100 / data.length());
    if (progress.wasCanceled())
      break;
    j = j % steps.length();
    int step = steps[j].toInt();
    if (isDecrypt)
      step = ~step + 1;
    data[i] = data[i] + step % 256;
  }
  progress.setValue(100);
  QMessageBox::information(this, "Encryption", "Encryption completed");
}

void Scaling::pbuttonGenClicked() {
  qsizetype length;
  QFile fileIn;
  if (openFile(fileIn, leditFileIn->text(), QIODeviceBase::ReadOnly |
               QIODeviceBase::ExistingOnly)) {
    if (teditIn->toPlainText().isEmpty()) {
      QMessageBox::critical(this, "Message", "error to read file and text");
      return;
    }
    length = teditIn->toPlainText().length();
  } else  {
    length = fileIn.size();
    fileIn.close();
  }

  int m = sboxM->value();
  if (length < m)
    genGamma(length);
  else
    genGamma(m);
}

void Scaling::genGamma(qsizetype length) {
  int m = sboxM->value();
  int a = sboxA->value() % m;
  int c = sboxC->value() % m;
  int x0 = sboxX0->value() % m;
  if (length > 1000)
    length = 1000;
  QString key;
  key.push_back(QString::number(x0));
  key.push_back(',');
  for (qsizetype i = 1; i < length; ++i) {
    x0 = (x0 * a + c) % m;
    key.push_back(QString::number(x0));
    key.push_back(',');
  }
  key.removeLast();
  teditKey->setText(key);
}


Scaling::~Scaling() {
  delete labelOptions;
  delete labelX0;
  delete sboxX0;
  delete labelM;
  delete sboxM;
  delete labelC;
  delete sboxC;
  delete labelA;
  delete sboxA;
  delete pbuttonGen;
  delete hlayoutOptions;
  delete labelFileIn;
  delete leditFileIn;
  delete pbuttonFileIn;
  delete labelFileOut;
  delete leditFileOut;
  delete pbuttonFileOut;
  delete pbuttonRun;
}
