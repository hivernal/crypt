#ifndef RSA_H
#define RSA_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QFile>
#include <gmpxx.h>

class Rsa : public QWidget {
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
  QLabel* labelKey;
  QSpinBox* sboxKey;
  QPushButton* pbuttonGenKey;
  QLabel* labelOperation;
  QComboBox* cboxOperation;
  QPushButton* pbuttonRun;

  int readKey(QString rsaFileName, mpz_class& eord, mpz_class& n);
  void rsaFile(QByteArray& data, QFile& ouputFile, const mpz_class& eord, 
               const mpz_class& n, bool isDecrypt);
  void rsaText(QString text, QString& result, const mpz_class& eord, 
               const mpz_class& n, bool isDecrypt);

public:
  Rsa();
  ~Rsa();
  QWidget* getWidget();

private slots:
  void pbuttonFileInClicked();
  void pbuttonFileOutClicked();
  void pbuttonGenKeyClicked();
  void pbuttonRunClicked();
  void teditTextInChanged();
};

#endif
