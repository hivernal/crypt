#include "gronsfeld.h"
#include "alphabet.h"

int gronsfeld(QString& text, QString key, bool isDecrypt) {
  QStringList steps = key.split(',', Qt::SkipEmptyParts);
  if (!steps.length())
    return EXIT_FAILURE;

  int step;
  bool toIntIsOk;
  for (qsizetype i = 0, j = 0; i < text.length(); ++i) {
    if (j == steps.length())
      j = 0;

    step = steps[j].toInt(&toIntIsOk);
    if (!toIntIsOk)
      return EXIT_FAILURE;
    if (isDecrypt)
      step = ~step + 1;

    if (isUpEn(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % sizeEn);
      if (text[i] > u'Z')
        text[i] = (QChar)(text[i].unicode() - sizeEn);
      else if (text[i] < u'A')
        text[i] = (QChar)(text[i].unicode() + sizeEn);
    } else if (isLowEn(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % sizeEn);
      if (text[i] > u'z')
        text[i] = (QChar)(text[i].unicode() - sizeEn);
      else if (text[i] < u'a')
        text[i] = (QChar)(text[i].unicode() + sizeEn);
    }

    else if (isUpRu(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % sizeRu);
      if (text[i] > u'Я')
        text[i] = (QChar)(text[i].unicode() - sizeRu);
      else if (text[i] < u'А')
        text[i] = (QChar)(text[i].unicode() + sizeRu);
    } else if (isLowRu(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % sizeRu);
      if (text[i] > u'я')
        text[i] = (QChar)(text[i].unicode() - sizeRu);
      else if (text[i] < u'а')
        text[i] = (QChar)(text[i].unicode() + sizeRu);
    }
  }
  return EXIT_SUCCESS;
}

Gronsfeld::Gronsfeld() {
  connect(teditIn, &QTextEdit::textChanged, this,
          &Gronsfeld::teditInChanged);
  connect(teditKey, &QTextEdit::textChanged, this,
          &Gronsfeld::teditInChanged);
  connect(cboxOperation, &QComboBox::currentIndexChanged, this,
          &Gronsfeld::teditInChanged);
}

void Gronsfeld::teditInChanged() {
  QString text = teditIn->toPlainText();
  bool isDecrypt = cboxOperation->currentIndex();
  if (gronsfeld(text, teditKey->toPlainText(), isDecrypt))
    teditOut->setText("Incorrect key!");
  else
    teditOut->setText(text);
}
