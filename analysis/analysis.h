#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>

#include "barchart.h"

class Analysis : public QWidget {
  QWidget* widget;
  QGridLayout* glayout;
  QLabel* labelFile;
  QLineEdit* leditFile;
  QPushButton* pbuttonFile;
  QLabel* labelIn;
  QTextEdit* teditIn;
  QLabel* labelOut;
  QTextEdit* teditOut;
  QLabel* labelFrom;
  QComboBox* cboxFrom;
  QLabel* labelReplacements;
  QTableWidget* tableWidgetReplacements;
  BarChart* barchart;
  QString alphabetBefore;
  QString alphabetAfter;

  int readFile(QString fileName);
  void fillTable();
  void drawBarCharts(QList<QPair<QChar, qsizetype>>& symbols);

public:
  Analysis();
  ~Analysis();
  QWidget* getWidget();

private slots:
  void pbuttonFileClicked();
  void leditFileChanged();
  void teditTextChanged();
  void tableCellChanged(int row, int column);
};

#endif
