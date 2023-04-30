#ifndef RSA_H
#define RSA_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>

class Rsa : public QWidget {
  QWidget* widget;
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
  QPushButton* pbuttonGenKey;
  QPushButton* pbuttonEncrypt;
  QPushButton* pbuttonDecrypt;

  int openFile(QFile& file, QString fileName, QIODeviceBase::OpenMode mode);

public:
  Rsa();
  ~Rsa();
  QWidget* getWidget();

private slots:
  void pbuttonFileInClicked();
  void pbuttonFileOutClicked();
  void pbuttonGenKeyClicked();
};

#endif
