#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

#include "barchart.h"

class Analysis : public QWidget {
  QWidget* widget;
  QGridLayout* glayout;

  QLabel* labelFile;
  QTextEdit* teditFile;
  QPushButton* pbuttonFile;

  QLabel* labelIn;
  QTextEdit* teditIn;

  QLabel* labelOut;
  QTextEdit* teditOut;

  QLabel* labelFrom;
  QComboBox* cboxFrom;

  BarChart* barchart;

public:
  Analysis();
  ~Analysis();
  QWidget* getWidget();

private slots:
  void pbuttonFileClicked();
  void teditFileChanged();
  void teditTextChanged();
};

#endif
