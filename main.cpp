#include <QApplication>

#include "crypt.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Crypt w;
  w.show();
  return a.exec();
}
