#include "atbash.h"
#include "alphabet.h"

enum {
  upEn = 155, lowEn = 219, upRu = 2111, lowRu = 2175,
};

QString atbash(QString text) {
  for (qsizetype i = 0; i < text.size(); ++i) {
    if (isUpEn(text[i]))
      text[i] = (QChar)(upEn - text[i].unicode());
    else if (isLowEn(text[i]))
      text[i] = (QChar)(lowEn - text[i].unicode());
    else if (isUpRu(text[i]))
      text[i] = (QChar)(upRu - text[i].unicode());
    else if (isLowRu(text[i]))
      text[i] = (QChar)(lowRu - text[i].unicode());
  }
  return text;
}

Atbash::Atbash() {
  glayout = new QGridLayout(this);
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
}

QWidget* Atbash::getWidget() {
  return this;
}
