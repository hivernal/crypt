#include "atbash.h"
#include "macros.h"

QString atbash(QString text) {
  for (int i = 0; i < text.size(); ++i) {
    if (IS_UP_EN(text[i]))
      text[i] = (QChar)(UP_EN - text[i].unicode());
    else if (IS_LOW_EN(text[i]))
      text[i] = (QChar)(LOW_EN - text[i].unicode());
    else if (IS_UP_RU(text[i]))
      text[i] = (QChar)(UP_RU - text[i].unicode());
    else if (IS_LOW_RU(text[i]))
      text[i] = (QChar)(LOW_RU - text[i].unicode());
  }
  return text;
}

Atbash::Atbash() {
  widget = new QWidget();
  glayout = new QGridLayout(widget);
  labelIn = new QLabel("Original");
  teditIn = new QTextEdit();
  labelOut = new QLabel("Result");
  teditOut = new QTextEdit();
  glayout->addWidget(labelIn, 0, 0);
  glayout->addWidget(teditIn, 0, 1);
  glayout->addWidget(labelOut, 1, 0);
  glayout->addWidget(teditOut, 1, 1);
  connect(teditIn, &QTextEdit::textChanged, this, &Atbash::teditChanged);
}

void Atbash::teditChanged() {
  QString text = atbash(teditIn->toPlainText());
  teditOut->setText(text);
}

Atbash::~Atbash() {
  delete labelIn;
  delete teditIn;
  delete teditOut;
  delete labelOut;
  delete glayout;
  delete widget;
}

QWidget* Atbash::getWidget() {
  return widget;
}
