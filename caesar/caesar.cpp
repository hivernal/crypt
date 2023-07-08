#include "caesar.h"
#include "alphabet.h"

QString caesar(QString text, int step, bool isDecrypt) {
  if (isDecrypt)
    step = ~step + 1;

  for (qsizetype i = 0; i < text.length(); i++) {
    if (isUpEn(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % sizeEn);
      if (text[i] > u'Z')
        text[i] = (QChar)(text[i].unicode() - sizeEn);
      else if (text[i] < u'A')
        text[i] = (QChar)(text[i].unicode() + sizeEn);
    } else if (isLowEn(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % sizeEn);
      if (text[i] > u'z')
        text[i] = (QChar)(text[i].unicode() - sizeEn);
      else if (text[i] < u'a')
        text[i] = (QChar)(text[i].unicode() + sizeEn);
    }

    else if (isUpRu(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % sizeRu);
      if (text[i] > u'Я')
        text[i] = (QChar)(text[i].unicode() - sizeRu);
      else if (text[i] < u'А')
        text[i] = (QChar)(text[i].unicode() + sizeRu);
    } else if (isLowRu(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % sizeRu);
      if (text[i] > u'я')
        text[i] = (QChar)(text[i].unicode() - sizeRu);
      else if (text[i] < u'а')
        text[i] = (QChar)(text[i].unicode() + sizeRu);
    }
  }
  return text;
}

Caesar::Caesar() {
  labelOperation = new QLabel("Operation");
  cboxOperation = new QComboBox();
  cboxOperation->addItem("Encrypt");
  cboxOperation->addItem("Decrypt");
  labelStep = new QLabel("Step");
  sboxStep = new QSpinBox();
  sboxStep->setRange(-1024, 1024);
  sboxStep->setMaximumWidth(100);
  sboxStep->setAlignment(Qt::AlignCenter);
  glayout->addWidget(labelOperation, 2, 0);
  glayout->addWidget(cboxOperation, 2, 1);
  glayout->addWidget(labelStep, 3, 0);
  glayout->addWidget(sboxStep, 3, 1);
  connect(teditIn, &QTextEdit::textChanged, this, &Caesar::teditChanged);
  connect(sboxStep, &QSpinBox::valueChanged, this, &Caesar::teditChanged);
  connect(cboxOperation, &QComboBox::currentIndexChanged, this,
          &Caesar::teditChanged);
}

void Caesar::teditChanged() {
  int step = sboxStep->value();
  QString qstr = teditIn->toPlainText();
  bool isDecrypt = cboxOperation->currentIndex();
  teditOut->setText(caesar(qstr, step, isDecrypt));
}

Caesar::~Caesar() {
  delete labelOperation;
  delete cboxOperation;
  delete labelStep;
  delete sboxStep;
}
