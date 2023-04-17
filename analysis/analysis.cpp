#include "analysis.h"

int analysis(const QString& file_name, QMap<QChar, qsizetype> &symbols) {
  QFile file_in(file_name);
  if (!file_in.open(QIODevice::ReadOnly))
    return EXIT_FAILURE;
  QTextStream input(&file_in);
  QString text = input.readAll();
  for (qsizetype i = 0; i < text.length() - 1; ++i) {
    if (text[i].isLetter())
      ++symbols[text[i]];
  }
  file_in.close();
  return EXIT_SUCCESS;
}

Analysis::Analysis() {
  widget = new QWidget();
  glayout = new QGridLayout(widget);

  labelFile = new QLabel("File: ");
  teditFile = new QTextEdit();
  teditFile->setMaximumHeight(30);
  teditFile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  pbuttonFile = new QPushButton("open");

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

  glayout->addWidget(labelFile, 0, 0);
  glayout->addWidget(teditFile, 0, 1);
  glayout->addWidget(pbuttonFile, 0, 2);
  glayout->addWidget(scroll, 1, 0, 1, 3);
  connect(pbuttonFile, &QPushButton::clicked, this,
          &Analysis::pbuttonFileClicked);
  connect(teditFile, &QTextEdit::textChanged, this,
          &Analysis::teditFileChanged);
}

void Analysis::pbuttonFileClicked() {
  QString file_name = QFileDialog::getOpenFileName();
  teditFile->setText(file_name);
}

void Analysis::teditFileChanged() {
  QMap<QChar, qsizetype> symbols;
  if (!analysis(teditFile->toPlainText(), symbols)) {
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
}

QWidget* Analysis::getWidget() {
  return widget;
}

Analysis::~Analysis() {
  delete labelFile;
  delete teditFile;
  delete pbuttonFile;
  delete bseries;
  delete axisY;
  delete axisX;
  delete chart;
  delete chartView;
  delete glayout;
  delete widget;
}
