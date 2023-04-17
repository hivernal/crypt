#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QtCharts>
#include <QScrollArea>

class Analysis : public QWidget {
  QWidget* widget;
  QGridLayout* glayout;
  QLabel* labelFile;
  QTextEdit* teditFile;
  QPushButton* pbuttonFile;
  QBarSeries* bseries;
  QChart* chart;
  QValueAxis* axisY;
  QChartView* chartView;
  QBarCategoryAxis* axisX;

  QScrollArea* scroll;

public:
  Analysis();
  ~Analysis();
  QWidget* getWidget();

private slots:
  void pbuttonFileClicked();
  void teditFileChanged();
};

#endif
