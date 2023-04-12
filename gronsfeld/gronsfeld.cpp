#include "gronsfeld.h"
#include "macros.h"

int gronsfeld(QString& text, QString key, bool is_decrypt) {
  QStringList steps = key.split(',', Qt::SkipEmptyParts);
  if (!steps.length())
    return EXIT_FAILURE;

  int step;
  bool to_int_is_ok;
  for (size_t i = 0, j = 0; i < text.length(); ++i) {
    if (j == steps.length())
      j = 0;

    step = steps[j].toInt(&to_int_is_ok);
    if (!to_int_is_ok)
      return EXIT_FAILURE;
    if (is_decrypt)
      step = ~step + 1;

    if (IS_UP_EN(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % EN);
      if (text[i] > u'Z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'A')
        text[i] = (QChar)(text[i].unicode() + EN);
    } else if (IS_LOW_EN(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % EN);
      if (text[i] > u'z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'a')
        text[i] = (QChar)(text[i].unicode() + EN);
    }

    else if (IS_UP_RU(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % RU);
      if (text[i] > u'Я')
        text[i] = (QChar)(text[i].unicode() - RU);
      else if (text[i] < u'А')
        text[i] = (QChar)(text[i].unicode() + RU);
    } else if (IS_LOW_RU(text[i])) {
      ++j;
      text[i] = (QChar)(text[i].unicode() + step % RU);
      if (text[i] > u'я')
        text[i] = (QChar)(text[i].unicode() - RU);
      else if (text[i] < u'а')
        text[i] = (QChar)(text[i].unicode() + RU);
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
  bool is_decrypt = false;
  if (cboxOperation->currentIndex())
    is_decrypt = true;
  if (gronsfeld(text, teditKey->toPlainText(), is_decrypt))
    teditOut->setText("Incorrect key!");
  else
    teditOut->setText(text);
}
