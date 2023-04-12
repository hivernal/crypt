#include "caesar.h"
#include "macros.h"

QString caesar(QString text, int step, bool is_decrypt) {
  if (is_decrypt)
    step = ~step + 1;

  for (int i = 0; i < text.length(); i++) {
    if (IS_UP_EN(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % EN);
      if (text[i] > u'Z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'A')
        text[i] = (QChar)(text[i].unicode() + EN);
    } else if (IS_LOW_EN(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % EN);
      if (text[i] > u'z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'a')
        text[i] = (QChar)(text[i].unicode() + EN);
    }

    else if (IS_UP_RU(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % RU);
      if (text[i] > u'Я')
        text[i] = (QChar)(text[i].unicode() - RU);
      else if (text[i] < u'А')
        text[i] = (QChar)(text[i].unicode() + RU);
    } else if (IS_LOW_RU(text[i])) {
      text[i] = (QChar)(text[i].unicode() + step % RU);
      if (text[i] > u'я')
        text[i] = (QChar)(text[i].unicode() - RU);
      else if (text[i] < u'а')
        text[i] = (QChar)(text[i].unicode() + RU);
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
  bool is_decrypt = false;
  if (cboxOperation->currentIndex())
    is_decrypt = true;
  teditOut->setText(caesar(qstr, step, is_decrypt));
}

Caesar::~Caesar() {
  delete labelOperation;
  delete cboxOperation;
  delete labelStep;
  delete sboxStep;
}
