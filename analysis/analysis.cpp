#include "analysis.h"
#include "alphabet.h"

const QString alphabetEn = "ETAONISRHLDCUPFMWYBGVKQXJZ";
const QString alphabetRu = " ОЕАИНТСРВЛКМДПУЯЫЗЪБГЧЙХЖЮШЦЩЭФ";

enum {
  ruFlag = 1, enFlag = 2
};


void addSymbol(QList<QPair<QChar, qsizetype>>& symbolAmount, QChar symbol) {
  for (qsizetype i = 0; i < symbolAmount.length(); ++i) {
    if (symbolAmount[i].first == symbol) {
      ++symbolAmount[i].second;
      return;
    }
  }
  symbolAmount.push_back(QPair<QChar, qsizetype>(symbol, 1));
}

qsizetype indexOfSymbol(QList<QPair<QChar, qsizetype>>& symbolAmount, QChar symbol) {
  for (qsizetype i = 0; i < symbolAmount.length(); ++i) {
    if (symbolAmount[i].first == symbol)
      return i;
  }
  return symbolAmount.length();
}

bool compareSymbols(QPair<QChar, qsizetype>& s1, QPair<QChar, qsizetype>& s2) {
  return s1.second > s2.second;
}

Analysis::Analysis() {
  glayout = new QGridLayout(this);

  labelFile = new QLabel("File ");
  leditFile = new QLineEdit();
  pbuttonFile = new QPushButton("Open");
  glayout->addWidget(labelFile, 0, 0);
  glayout->addWidget(leditFile, 0, 1);
  glayout->addWidget(pbuttonFile, 0, 2);

  labelIn = new QLabel("Text ");
  teditIn = new QTextEdit();
  glayout->addWidget(labelIn, 1, 0);
  glayout->addWidget(teditIn, 1, 1, 1, 2);

  labelOut = new QLabel("Result ");
  teditOut = new QTextEdit();
  glayout->addWidget(labelOut, 2, 0);
  glayout->addWidget(teditOut, 2, 1, 1, 2);

  labelReplacements = new QLabel("Replacements");
  tableWidgetReplacements = new QTableWidget();
  tableWidgetReplacements->setRowCount(2);
  tableWidgetReplacements->setColumnCount(32);
  tableWidgetReplacements->setRowHeight(0, 50);
  tableWidgetReplacements->setRowHeight(1, 50);
  tableWidgetReplacements->setVerticalHeaderLabels({"Before", "After"});
  glayout->addWidget(labelReplacements, 3, 0);
  glayout->addWidget(tableWidgetReplacements, 3,  1, 1, 2);

  labelFrom = new QLabel("Read from ");
  cboxFrom = new QComboBox();
  cboxFrom->addItem("File");
  cboxFrom->addItem("Text box");
  glayout->addWidget(labelFrom, 4, 0);
  glayout->addWidget(cboxFrom, 4,  1, 1, 2);

  barcharts = new BarCharts();

  connect(pbuttonFile, &QPushButton::clicked, this,
          &Analysis::pbuttonFileClicked);
  connect(leditFile, &QLineEdit::textChanged, this,
          &Analysis::leditFileChanged);
  connect(teditIn, &QTextEdit::textChanged, this,
          &Analysis::teditTextChanged);
  connect(cboxFrom, &QComboBox::currentIndexChanged, this,
          &Analysis::teditTextChanged);
  connect(cboxFrom, &QComboBox::currentIndexChanged, this,
          &Analysis::leditFileChanged);
}

void Analysis::pbuttonFileClicked() {
  QString file_name = QFileDialog::getOpenFileName();
  leditFile->setText(file_name);
}

void Analysis::leditFileChanged() {
  if (cboxFrom->currentIndex() || readFile(leditFile->text()))
    return;
  QString text = teditIn->toPlainText();
  QList<QPair<QChar, qsizetype>> symbolAmount;
  if (analysis(text, symbolAmount)) {
    teditOut->setText("Incorrect input!");
    return;
  }
  teditOut->setText(text);
  fillTable();
  drawBarCharts(symbolAmount);
}

void Analysis::teditTextChanged() {
  QString text = teditIn->toPlainText();
  if (!cboxFrom->currentIndex() || text.isEmpty())
    return;
  QList<QPair<QChar, qsizetype>> symbolAmount;
  if (analysis(text, symbolAmount)) {
    teditOut->setText("Incorrect input!");
    return;
  }
  teditOut->setText(text);
  fillTable();
  drawBarCharts(symbolAmount);
}

void Analysis::tableCellChanged(int row, int column) {
  disconnect(tableWidgetReplacements, &QTableWidget::cellChanged, this,
             &Analysis::tableCellChanged);
  QTableWidgetItem* item = tableWidgetReplacements->item(row, column);
  QString itemText = item->text();
  if (itemText.length() != 1 || !itemText[0].isLetter() && itemText[0] != '_') {
    item->setText(alphabetAfter[column]);
    connect(tableWidgetReplacements, &QTableWidget::cellChanged, this,
               &Analysis::tableCellChanged);
    return;
  }
  if (itemText[0].isLower())
    item->setText(itemText[0].toUpper());
  alphabetAfter[column] = item->text()[0];

  QString text = teditIn->toPlainText();
  bool wasLow = false;
  for (qsizetype i = 0; i < text.length(); ++i) {
    if (text[i].isLower()) {
      wasLow = true;
      text[i] = text[i].toUpper();
    }
    for (qsizetype j = 0; j < alphabetBefore.length(); ++j) {
      if (text[i] == alphabetBefore[j]) {
        text[i] = alphabetAfter[j];
        if (text[i] == '_')
          text[i] = ' ';
        break;
      }
    }
    if (wasLow)
      text[i] = text[i].toLower();
  }
  teditOut->setText(text);
  connect(tableWidgetReplacements, &QTableWidget::cellChanged, this,
             &Analysis::tableCellChanged);
}

int Analysis::readFile(QString fileName) {
  if (fileName.isEmpty())
    return EXIT_FAILURE;
  QFile file_in(fileName);
  if (!file_in.open(QFile::ReadOnly))
    return EXIT_FAILURE;
  QTextStream input(&file_in);
  fileName = input.readAll();
  teditIn->setText(fileName);
  file_in.close();
  return EXIT_SUCCESS;
}

int Analysis::analysis(QString& text, QList<QPair<QChar, qsizetype>>& symbolAmount) {
  qsizetype amount = 0;
  int alphabet = 0;
  for (qsizetype i = 0; i < text.length(); ++i) {
    bool wasLow = false;
    if (text[i] == '\n')
      continue;
    if (text[i].isLower()) {
      wasLow = true;
      text[i] = text[i].toUpper();
    }
    if (isUpEn(text[i])) {
      alphabet |= enFlag;
      addSymbol(symbolAmount, text[i]);
    }
    else if (isUpRu(text[i]) || text[i] == ' ' && alphabet & ruFlag) {
      alphabet |= ruFlag;
      addSymbol(symbolAmount, text[i]);
    }
    ++amount;
    if (wasLow)
      text[i] = text[i].toLower();
  }
  if (alphabet & ruFlag && alphabet & enFlag)
    return EXIT_FAILURE;

  if (alphabet & ruFlag) alphabetAfter = alphabetRu;
  else alphabetAfter = alphabetEn;
  std::sort(symbolAmount.begin(), symbolAmount.end(), compareSymbols);

  QTextStream out(stdout);
  out << Qt::endl;
  alphabetBefore.clear();
  for (QPair<QChar, qsizetype> it : symbolAmount) {
    out << it.first << " -> " << (float) it.second / amount << Qt::endl;
    alphabetBefore.push_back(it.first);
  }

  for (qsizetype i = 0; i < text.length(); ++i) {
    bool wasLow = false;
    if (text[i].isLower()) {
      wasLow = true;
      text[i] = text[i].toUpper();
    }
    if (isUpEn(text[i])) {
      text[i] = alphabetEn[indexOfSymbol(symbolAmount, text[i])];
    } else if (isUpRu(text[i]) || text[i] == ' ' && alphabet & ruFlag) {
      text[i] = alphabetRu[indexOfSymbol(symbolAmount, text[i])];
    }
    if (wasLow)
      text[i] = text[i].toLower();
  }
  return EXIT_SUCCESS;
}

void Analysis::fillTable() {
  disconnect(tableWidgetReplacements, &QTableWidget::cellChanged, this,
             &Analysis::tableCellChanged);
  tableWidgetReplacements->setColumnCount(alphabetBefore.length());
  for (qsizetype i = 0; i < alphabetBefore.length(); ++i) {
    QTableWidgetItem* item;
    if (alphabetBefore[i] == ' ') {
        item = new QTableWidgetItem("_");
    } else
        item = new QTableWidgetItem(alphabetBefore[i]);
    tableWidgetReplacements->setItem(0, i, item);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);

    if (alphabetAfter[i] == ' ')
      item = new QTableWidgetItem("_");
    else
      item = new QTableWidgetItem(alphabetAfter[i]);
    tableWidgetReplacements->setItem(1, i, item);
    item->setTextAlignment(Qt::AlignCenter);
    tableWidgetReplacements->setColumnWidth(i, 45);
  }
  connect(tableWidgetReplacements, &QTableWidget::cellChanged, this,
          &Analysis::tableCellChanged);
}

void Analysis::drawBarCharts(QList<QPair<QChar, qsizetype>>& symbolAmount) {
  if (!barcharts->isVisible()) {
    barcharts->show();
  }
  barcharts->drawBarCharts(symbolAmount);
}

QWidget* Analysis::getWidget() {
  return this;
}

Analysis::~Analysis() {
  delete barcharts;
  delete labelFile;
  delete leditFile;
  delete pbuttonFile;
  delete labelIn;
  delete teditIn;
  delete labelOut;
  delete teditOut;
  delete labelFrom;
  delete cboxFrom;
  delete labelReplacements;
  delete tableWidgetReplacements;
  delete glayout;
}
