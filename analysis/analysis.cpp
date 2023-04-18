#include "analysis.h"
#include "macros.h"

const QString en_symbols = "ETAONISRHLDCUPFMWYBGVKQXJZ";
const float en_probties[] = {0.123, 0.096, 0.081, 0.079, 0.072, 0.071, 0.066, 0.06,
                             0.051, 0.04, 0.036, 0.032, 0.031, 0.023, 0.023, 0.022, 
                             0.02, 0.019, 0.016, 0.016, 0.009, 0.005, 0.002, 0.002,
                             0.001, 0.001};
const QString ru_symbols = " ОЕАИНТСРВЛКМДПУЯЫЗЪБГЧЙХЖЮШЦЩЭФ";
const float ru_probties[] = {0.175, 0.09, 0.072, 0.062, 0.062, 0.053, 0.053, 0.045,
                             0.04, 0.038, 0.035, 0.028, 0.026, 0.025, 0.023, 0.021, 
                             0.018, 0.016, 0.016, 0.014, 0.014, 0.013, 0.012, 0.001,
                             0.009, 0.007, 0.006, 0.006, 0.004, 0.003, 0.003, 0.002};

int analysis(QString& text, QMap<QChar, qsizetype>& symbols, bool from_file) {
  if (from_file) {
    QFile file_in(text);
    if (!file_in.open(QIODevice::ReadOnly))
      return EXIT_FAILURE;
    QTextStream input(&file_in);
    text = input.readAll();
    file_in.close();
  }

  qsizetype amount = 0;
  for (qsizetype i = 0; i < text.length(); ++i) {
    bool is_low = false;
    if (text[i] == '\n' || text[i] == '\t')
      continue;
    if (text[i].isLower()) {
      is_low = true;
      text[i] = text[i].toUpper();
    }
    ++amount;
    if (text[i].isLetter())
      ++symbols[text[i]];
    if (is_low)
      text[i] = text[i].toLower();
  }

  QTextStream out(stdout);
  out << Qt::endl;
  for (QMap<QChar, qsizetype>::iterator it = symbols.begin();
       it != symbols.end(); ++it) {
    out << it.key() << " -> " << (float) it.value() / amount << Qt::endl;
  }

  for (qsizetype i = 0; i < text.length(); ++i) {
    bool is_low = false;
    if (text[i].isLower()) {
      is_low = true;
      text[i] = text[i].toUpper();
    }
    if (IS_UP_EN(text[i])) {
      float probty = (float) symbols[text[i]] / amount;
      qsizetype i1 = 0, i2 = 1;
      for (;i2 < EN && en_probties[i2] > probty; ++i1, ++i2);
      if (probty - en_probties[i2] < en_probties[i1] - probty)
        text[i] = en_symbols[i2];
      else
        text[i] = en_symbols[i1];
    } else if (IS_UP_RU(text[i])) {
      float probty = (float) symbols[text[i]] / amount;
      qsizetype i1 = 0, i2 = 1;
      for (;i2 < RU && ru_probties[i2] > probty; ++i1, ++i2);
      if (probty - ru_probties[i2] < ru_probties[i1] - probty)
        text[i] = ru_symbols[i2];
      else
        text[i] = ru_symbols[i1];
    }
    if (is_low)
      text[i] = text[i].toLower();
  }
  return EXIT_SUCCESS;
}

Analysis::Analysis() {
  widget = new QWidget();
  glayout = new QGridLayout(widget);

  labelFile = new QLabel("File: ");
  teditFile = new QTextEdit();
  teditFile->setMaximumHeight(30);
  teditFile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  pbuttonFile = new QPushButton("open");

  labelIn = new QLabel("Text: ");
  teditIn = new QTextEdit();

  labelOut = new QLabel("Result: ");
  teditOut = new QTextEdit();

  labelFrom = new QLabel("Read from: ");
  cboxFrom = new QComboBox();
  cboxFrom->addItem("File");
  cboxFrom->addItem("Text box");

  barchart = nullptr;

  glayout->addWidget(labelFile, 0, 0);
  glayout->addWidget(teditFile, 0, 1);
  glayout->addWidget(pbuttonFile, 0, 2);

  glayout->addWidget(labelIn, 1, 0);
  glayout->addWidget(teditIn, 1, 1, 1, 2);

  glayout->addWidget(labelOut, 2, 0);
  glayout->addWidget(teditOut, 2, 1, 1, 2);

  glayout->addWidget(labelFrom, 3, 0);
  glayout->addWidget(cboxFrom, 3,  1, 1, 2);

  connect(pbuttonFile, &QPushButton::clicked, this,
          &Analysis::pbuttonFileClicked);
  connect(teditFile, &QTextEdit::textChanged, this,
          &Analysis::teditFileChanged);
  connect(teditIn, &QTextEdit::textChanged, this,
          &Analysis::teditTextChanged);
}

void Analysis::pbuttonFileClicked() {
  QString file_name = QFileDialog::getOpenFileName();
  teditFile->setText(file_name);
}

void Analysis::teditFileChanged() {
  if (cboxFrom->currentIndex())
    return;
  QString text = teditFile->toPlainText();
  QMap<QChar, qsizetype> symbols;
  if (!analysis(text, symbols, true)) {
    teditOut->setText(text);
    if (barchart == nullptr)
      barchart = new BarChart();
    barchart->show();
    barchart->draw(symbols);
  }
}

void Analysis::teditTextChanged() {
  if (!cboxFrom->currentIndex())
    return;
  QString text = teditIn->toPlainText();
  QMap<QChar, qsizetype> symbols;
  if (!analysis(text, symbols, false)) {
    teditOut->setText(text);
    if (barchart == nullptr)
      barchart = new BarChart();
    barchart->show();
    barchart->draw(symbols);
  }
}

QWidget* Analysis::getWidget() {
  return widget;
}

Analysis::~Analysis() {
  delete barchart;
  delete labelFile;
  delete teditFile;
  delete pbuttonFile;
  delete labelIn;
  delete teditIn;
  delete labelOut;
  delete teditOut;
  delete labelFrom;
  delete cboxFrom;
  delete glayout;
  delete widget;
}
