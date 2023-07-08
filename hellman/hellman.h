#ifndef HELLMAN_H
#define HELLMAN_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QFile>
#include <gmpxx.h>

class Hellman : public QWidget {
  Q_OBJECT

  QGridLayout* glayout;

  QLabel* labelTextIn;
  QTextEdit* teditTextIn;
  QLabel* labelTextOut;
  QTextEdit* teditTextOut;

  QLabel* labelFileIn;
  QLineEdit* leditFileIn;
  QPushButton* pbuttonFileIn;
  QLabel* labelFileOut;
  QLineEdit* leditFileOut;
  QPushButton* pbuttonFileOut;

  QPushButton* pbuttonRun;

  QLabel* labelKey;
  QHBoxLayout* hlayoutKey;
  QLabel* labelKeySize;
  QSpinBox* sboxKeySize;
  QLabel* labelG;
  QSpinBox* sboxG;
  QLabel* labelP;
  QSpinBox* sboxP;
  QPushButton* pbuttonGenerate;

  void hellmanFile(QByteArray& data, QFile& outputFile, int key);
  void hellmanText(QString text, QString& result, int key);
  int readKeyFile(mpz_class& aPrivate, mpz_class& k);

public:
  Hellman();
  ~Hellman();
  QWidget* getWidget();

private slots:
  void pbuttonFileInClicked();
  void pbuttonFileOutClicked();
  void pbuttonGenerateClicked();
  void pbuttonRunClicked();
  void teditTextInChanged();
};

#endif
