#include "visionary.h"
#include "macros.h"

int visionary(QString& text, QString key, bool is_decrypt) {
  if (!key.length())
    return EXIT_FAILURE;
  size_t i, j;
  for (i = 0; i < key.length(); ++i) {
    if (!key[i].isLetter())
      return EXIT_FAILURE;
  }

  QChar tmp;
  for (i = 0, j = 0; i < text.length(); ++i) {
    if (j == key.length())
      j = 0;

    if (is_decrypt) {
      if (IS_UP_EN(text[i]) || IS_LOW_EN(text[i])) {
        tmp = (QChar)(text[i].unicode() - key[j++].unicode() % EN);
        if (IS_UP_EN(text[i]) && tmp < u'A' ||
            IS_LOW_EN(text[i]) && tmp < u'a')
          tmp = (QChar)(tmp.unicode() + EN);
        text[i] = tmp;
      } else if (IS_UP_RU(text[i]) || IS_LOW_RU(text[i])) {
        tmp = (QChar)(text[i].unicode() - key[j++].unicode() % RU);
        if (IS_UP_RU(text[i]) && tmp < u'А' ||
            IS_LOW_RU(text[i]) && tmp < u'а')
          tmp = (QChar)(tmp.unicode() + RU);
        text[i] = tmp;
      }
    }

    else {
      if (IS_UP_EN(text[i]) || IS_LOW_EN(text[i])) {
        tmp = (QChar)(text[i].unicode() + key[j++].unicode() % EN);
        if (IS_UP_EN(text[i]) && tmp > u'Z' ||
            IS_LOW_EN(text[i]) && tmp > u'z')
          tmp = (QChar)(tmp.unicode() - EN);
        text[i] = tmp;
      } else if (IS_UP_RU(text[i]) || IS_LOW_RU(text[i])) {
        tmp = (QChar)(text[i].unicode() + key[j++].unicode() % RU);
        if (IS_UP_RU(text[i]) && tmp > u'Я' ||
            IS_LOW_RU(text[i]) && tmp > u'я')
          tmp = (QChar)(tmp.unicode() - RU);
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
