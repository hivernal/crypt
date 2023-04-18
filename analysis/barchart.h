#ifndef BARCHART_H
#define BARCHART_H

#include <QWidget>
#include <QtCharts>
#include <QScrollArea>
#include <QGridLayout>

class BarChart : public QWidget {
  QBarSeries* bseries;
  QChart* chart;
  QValueAxis* axisY;
  QChartView* chartView;
  QBarCategoryAxis* axisX;
  QScrollArea* scroll;
  QGridLayout* glayout;

public:
  BarChart();
  ~BarChart();
  void draw(QMap<QChar, qsizetype>& symbols);
};

#endif
