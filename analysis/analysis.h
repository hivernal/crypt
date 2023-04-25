#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>

#include "barcharts.h"

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
  BarCharts* barcharts;
  QString alphabetBefore;
  QString alphabetAfter;
  enum {Ru = 1, En};

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
  int analysis(QString& text, QList<QPair<QChar, qsizetype>>& symbolAmount);
};

#endif
