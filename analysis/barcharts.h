#ifndef BARCHART_H
#define BARCHART_H

#include <QWidget>
#include <QtCharts>
#include <QScrollArea>
#include <QGridLayout>

class BarCharts : public QWidget {
  QBarSeries* bseries;
  QValueAxis* axisY;
  QBarCategoryAxis* axisX;
  QChart* chart;
  QChartView* chartView;
  QScrollArea* scroll;
  QGridLayout* glayout;

public:
  BarCharts();
  ~BarCharts();
  void drawBarCharts(QList<QPair<QChar, qsizetype>>& symbolAmount);
};

#endif
