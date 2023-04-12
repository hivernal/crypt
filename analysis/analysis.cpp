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
  chart->addSeries(bseries);
  chart->setTitle("Cryptoanalysis");
  chart->setAnimationOptions(QChart::SeriesAnimations);
  axisY = new QValueAxis();
  chart->addAxis(axisY, Qt::AlignLeft);
  bseries->attachAxis(axisY); chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignRight);
  chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  glayout->addWidget(labelFile, 0, 0);
  glayout->addWidget(teditFile, 0, 1);
  glayout->addWidget(pbuttonFile, 0, 2);
  glayout->addWidget(chartView, 1, 0, 1, 3);
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
    chart->removeSeries(bseries);
    qsizetype size = 0;
    for (QMap<QChar, qsizetype>::iterator it = symbols.begin();
         it != symbols.end(); ++it) {
      size += it.value();
    }
    for (QMap<QChar, qsizetype>::iterator it = symbols.begin();
         it != symbols.end(); ++it) {
      QString str = it.key();
      str += '(' + QString::number((double)it.value()/size, 'g', 2)+ ')';
      QBarSet* symbol = new QBarSet(str);
      *symbol << it.value();
      bseries->append(symbol);
    }
    chart->addSeries(bseries);
    axisY->setRange(0, size);
    axisY->setLabelFormat("%d");
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
  delete chart;
  delete chartView;
  delete glayout;
  delete widget;
}
