#include "barchart.h"

BarChart::BarChart() {
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
  scroll = new QScrollArea();
  scroll->setWidget(chartView);
  scroll->setWidgetResizable(true);
  glayout = new QGridLayout(this);
  glayout->addWidget(scroll);
}

BarChart::~BarChart() {
  delete bseries;
  delete axisY;
  delete axisX;
  delete chart;
  delete chartView;
  delete scroll;
  delete glayout;
}

void BarChart::draw(QMap<QChar, qsizetype>& symbols) {
    bseries->clear();
    QBarSet* letters = new QBarSet("");
    QStringList categories;
    qsizetype max = 0;
    for (QMap<QChar, qsizetype>::iterator it = symbols.begin();
         it != symbols.end(); ++it) {
      if (it.value() > max)
        max = it.value();
      *letters << it.value();
      categories << it.key();
    }
    bseries->append(letters);
    axisX->setCategories(categories);
    axisY->setMax(max);
    chartView->setMinimumWidth(categories.count() * 35);
    chartView->setMinimumHeight(max * 13);
}
