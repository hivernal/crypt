#include "visionary.h"
#include "alphabet.h"

int visionary(QString& text, QString key, bool isDecrypt) {
  if (!key.length())
    return EXIT_FAILURE;
  qsizetype i, j;
  for (i = 0; i < key.length(); ++i) {
    if (!key[i].isLetter())
      return EXIT_FAILURE;
  }

  QChar tmp;
  for (i = 0, j = 0; i < text.length(); ++i) {
    if (j == key.length())
      j = 0;

    if (isDecrypt) {
      if (isEn(text[i])) {
        tmp = (QChar)(text[i].unicode() - key[j++].unicode() % sizeEn);
        if (isUpEn(text[i]) && tmp < u'A' ||
            isLowEn(text[i]) && tmp < u'a')
          tmp = (QChar)(tmp.unicode() + sizeEn);
        text[i] = tmp;
      } else if (isRu(text[i])) {
        tmp = (QChar)(text[i].unicode() - key[j++].unicode() % sizeRu);
        if (isUpRu(text[i]) && tmp < u'А' ||
            isLowRu(text[i]) && tmp < u'а')
          tmp = (QChar)(tmp.unicode() + sizeRu);
        text[i] = tmp;
      }
    }

    else {
      if (isEn(text[i])) {
        tmp = (QChar)(text[i].unicode() + key[j++].unicode() % sizeEn);
        if (isUpEn(text[i]) && tmp > u'Z' ||
            isLowEn(text[i]) && tmp > u'z')
          tmp = (QChar)(tmp.unicode() - sizeEn);
        text[i] = tmp;
      } else if (isRu(text[i])) {
        tmp = (QChar)(text[i].unicode() + key[j++].unicode() % sizeRu);
        if (isUpRu(text[i]) && tmp > u'Я' ||
            isLowRu(text[i]) && tmp > u'я')
          tmp = (QChar)(tmp.unicode() - sizeRu);
        text[i] = tmp;
      }
    }
  }

  return EXIT_SUCCESS;
}

Visionary::Visionary() {
  connect(teditIn, &QTextEdit::textChanged, this,
          &Visionary::teditInChanged);
  connect(teditKey, &QTextEdit::textChanged, this,
          &Visionary::teditInChanged);
  connect(cboxOperation, &QComboBox::currentIndexChanged, this,
          &Visionary::teditInChanged);
}

void Visionary::teditInChanged() {
  QString text = teditIn->toPlainText();
  bool is_decrypt = false;
  if (cboxOperation->currentIndex())
    is_decrypt = true;
  if (visionary(text, teditKey->toPlainText(), is_decrypt))
    teditOut->setText("Incorrect key!");
  else
    teditOut->setText(text);
}
