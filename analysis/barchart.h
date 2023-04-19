#ifndef BARCHART_H
#define BARCHART_H

#include <QWidget>
#include <QtCharts>
#include <QScrollArea>
#include <QGridLayout>

class BarChart : public QWidget {
  QBarSeries* bseries;
  QValueAxis* axisY;
  QBarCategoryAxis* axisX;
  QChart* chart;
  QChartView* chartView;
  QScrollArea* scroll;
  QGridLayout* glayout;

public:
  BarChart();
  ~BarChart();
  void drawBarCharts(QList<QPair<QChar, qsizetype>>& symbolAmount);
};

#endif
