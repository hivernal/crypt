#ifndef ATBASH_H
#define ATBASH_H

#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QLabel>

class Atbash : public QWidget {
protected:
  QGridLayout* glayout;
  QTextEdit* teditIn;
  QTextEdit* teditOut;
  QLabel* labelIn;
  QLabel* labelOut;

public:
  Atbash();
  ~Atbash();
  QWidget* getWidget();

private slots:
  void teditChanged();
};

#endif
