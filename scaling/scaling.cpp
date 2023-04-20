#include "scaling.h"

#include <QList>
#include <QFileDialog>
#include <QMessageBox>

int gronsfeld(QString& text, QString key, bool is_decrypt);

void genGamma(QList<int>& gamma, int m, int a, int c, int x0) {
  gamma.push_back(x0);
  for (int i = 1; i < m; ++i) {
    x0 = (x0 * a + c) % m;
    gamma.push_back(x0);
  }
}

Scaling::Scaling() {
  labelKey->setText("Gamma");

  labelFileIn = new QLabel("Input file");
  leditFileIn = new QLineEdit();
  glayoutFiles = new QGridLayout();
  pbuttonFileIn = new QPushButton("open");
  labelFileOut = new QLabel("Output file");
  leditFileOut = new QLineEdit();
  pbuttonFileOut = new QPushButton("open");
  progBarFiles = new QProgressBar();
  progBarFiles->hide();
  pbuttonRun = new QPushButton("run");

  glayoutFiles->addWidget(labelFileIn, 0, 0);
  glayoutFiles->addWidget(leditFileIn, 0, 1);
  glayoutFiles->addWidget(pbuttonFileIn, 0, 2);
  glayoutFiles->addWidget(labelFileOut, 1, 0);
  glayoutFiles->addWidget(leditFileOut, 1, 1);
  glayoutFiles->addWidget(pbuttonFileOut, 1, 2);
  glayoutFiles->addWidget(progBarFiles, 2, 1, 1, 2);
  glayoutFiles->addWidget(pbuttonRun, 3, 2);

  labelOptions = new QLabel("Options");
  labelX0 = new QLabel("X0:");
  labelX0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxX0 = new QSpinBox();
  sboxX0->setAlignment(Qt::AlignCenter);
  sboxX0->setMaximum(INT_MAX);
  sboxX0->setMinimumWidth(50);

  labelM = new QLabel("M:");
  labelM->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxM = new QSpinBox();
  sboxM->setAlignment(Qt::AlignCenter);
  sboxM->setMaximum(INT_MAX);
  sboxM->setMinimum(INT_MIN);
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
  pbuttonGen = new QPushButton("generate");

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

  glayout->addLayout(glayoutFiles, 3, 0, 1, 2);
  glayout->addWidget(labelOptions, 4, 0);
  glayout->addLayout(hlayoutOptions, 4, 1);
  glayout->addWidget(labelOperation, 5, 0);
  glayout->addWidget(cboxOperation, 5, 1);

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
    teditOut->setText("Incorrect key!");
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
  if (openFile(fileIn, leditFileIn->text(), QIODevice::ReadOnly |
               QIODeviceBase::ExistingOnly)) {
    QMessageBox::critical(this, "Input file", "Error to open '" +
                          leditFileIn->text() + "' file");
    return;
  }
  QByteArray text = fileIn.readAll();
  fileIn.close();

  QFile fileOut;
  if (openFile(fileOut, leditFileOut->text(), QIODevice::WriteOnly |
               QIODeviceBase::Truncate)) {
    QMessageBox::critical(this, "Output file", "Error to open '" +
                          leditFileOut->text() + "' file");
    return;
  }

  progBarFiles->show();
  scaling(text);
  progBarFiles->hide();
  fileOut.write(text);
  fileOut.close();
}

int Scaling::openFile(QFile& file, QString fileName,
                      QIODeviceBase::OpenMode mode) {
  if (fileName.isEmpty() || fileName[0] == ' ' || fileName.last(1) == ' ')
    return EXIT_FAILURE;
  file.setFileName(fileName);
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

void Scaling::scaling(QByteArray& text) {
  bool isDecrypt = cboxOperation->currentIndex();
  QString gamma = teditKey->toPlainText();
  QStringList steps = gamma.split(',', Qt::SkipEmptyParts);
  for (qsizetype i = 0, j = 0; i < text.length(); ++i, ++j) {
    progBarFiles->setValue(i * 100 / text.length());
    j = j % steps.length();
    int step = steps[j].toInt();
    if (isDecrypt)
      step = ~step + 1;
    text[i] = text[i] + step % 256;
  }
}


void Scaling::pbuttonGenClicked() {
  QList<int> gamma;
  int m = sboxM->value();
  if (!m) {
    QMessageBox::critical(this, "M", "M parametr is 0");
    return;
  }
  int a = sboxA->value() % m;
  int c = sboxC->value() % m;
  int x0 = sboxX0->value() % m;
  genGamma(gamma, m, a, c, x0);
  if (gamma.isEmpty())
    return;
  QString key;
  for (int i = 0; i < gamma.length(); ++i) {
    key.push_back(QString::number(gamma[i]));
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
  delete progBarFiles;
  delete pbuttonRun;
  delete glayoutFiles;
}
