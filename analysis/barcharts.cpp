#include "barcharts.h"

BarCharts::BarCharts() {
  bseries = new QBarSeries();
  chart = new QChart();
  chart->setTitle("Cryptoanalysis");
  chart->addSeries(bseries);
  chart->legend()->setVisible(false);

  axisY = new QValueAxis();
  axisY->setTickType(QValueAxis::TicksDynamic);
  axisY->setTickInterval(4);
  axisY->setMinorTickCount(3);
  axisY->setLabelFormat("%d");
  axisX = new QBarCategoryAxis();
  chart->addAxis(axisY, Qt::AlignLeft);
  chart->addAxis(axisX, Qt::AlignBottom);
  bseries->attachAxis(axisY); 
  bseries->attachAxis(axisX); 

  chartView = new QChartView(chart);
  chartView->setBaseSize(1000, 700);
  scroll = new QScrollArea();
  scroll->setWidget(chartView);
  scroll->setWidgetResizable(true);
  glayout = new QGridLayout(this);
  glayout->addWidget(scroll);
}

BarCharts::~BarCharts() {
  delete bseries;
  delete axisY;
  delete axisX;
  delete chart;
  delete chartView;
  delete scroll;
  delete glayout;
}

void BarCharts::drawBarCharts(QList<QPair<QChar, qsizetype>>& symbolAmount) {
    bseries->clear();
    QBarSet* letters = new QBarSet("");
    QStringList categories;
    qsizetype max = 0;
    for (QPair<QChar, qsizetype> it : symbolAmount) {
      if (it.second > max)
        max = it.second;
      *letters << it.second;
      if (it.first == ' ')
        categories << "_";
      else
        categories << it.first;
    }
    bseries->append(letters);
    axisX->setCategories(categories);
    axisY->setMax(max);
    chartView->setMinimumWidth(categories.count() * 35);
    chartView->setMinimumHeight(max * 13);
}
