#include <QString>

#define UP_EN 155
#define LOW_EN 219
#define UP_RU 2111
#define LOW_RU 2175

/* const QString old_alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\
                          abcdefghijklmnopqrstuvwxyz\
                          АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ\
                          абвгдежзийклмнопрстуфхцчшщъыьэюя";
const QString new_alph = "ZYXWVUTSRQPONMLKJIHGFEDCBA\
                          zyxwvutsrqponmlkjihgfedcba\
                          ЯЮЭЬЫЪЩШЧЦХФУТСРПОНМЛКЙИЗЖЕДГВБА\
                          яюэьыъщшчцхфутсрпонмлкйизжедгвба"; */

QString atbash(QString text) {
  for (size_t i = 0; i < text.size(); ++i) {
    /* for (size_t j = 0; j < old_alph.length(); ++j)
      if (qstr[i] == old_alph[j]) {
        qstr[i] = new_alph[j];
        break;
      } */
    if (text[i] >= u'A' && text[i] <= u'Z')
      text[i] = (QChar) (UP_EN - text[i].unicode());
    else if (text[i] >= u'a' && text[i] <= u'z')
      text[i] = (QChar) (LOW_EN - text[i].unicode());
    else if (text[i] >= u'А' && text[i] <= u'Я')
      text[i] = (QChar) (UP_RU - text[i].unicode());
    else if (text[i] >= u'а' && text[i] <= u'я')
      text[i] = (QChar) (LOW_RU - text[i].unicode());
  }
  return text;
}

#define EN 26
#define RU 32

QString caesar(QString text, int step, const int is_decrypt) {
  if (is_decrypt)
    step = ~step + 1;

  for (size_t i = 0; i < text.length(); i++) {
    if (text[i] >= u'A' && text[i] <= u'Z') {
      text[i] = (QChar) (text[i].unicode() + step % EN);
      if (text[i] > u'Z')
        text[i] = (QChar) (text[i].unicode() - EN);
      else if (text[i] < u'A')
        text[i] = (QChar) (text[i].unicode() + EN);
    }
    else if (text[i] >= u'a' && text[i] <= u'z') {
      text[i] = (QChar) (text[i].unicode() + step % EN);
      if (text[i] > u'z')
        text[i] = (QChar) (text[i].unicode() - EN);
      else if (text[i] < u'a')
        text[i] = (QChar) (text[i].unicode() + EN);
    }

    else if (text[i] >= u'А' && text[i] <= u'Я') {
      text[i] = (QChar) (text[i].unicode() + step % RU);
      if (text[i] > u'Я')
        text[i] = (QChar) (text[i].unicode() - RU);
      else if (text[i] < u'А')
        text[i] = (QChar) (text[i].unicode() + RU);
    }
    else if (text[i] >= u'а' && text[i] <= u'я') {
      text[i] = (QChar) (text[i].unicode() + step % RU);
      if (text[i] > u'я')
        text[i] = (QChar) (text[i].unicode() - RU);
      else if (text[i] < u'а')
        text[i] = (QChar) (text[i].unicode() + RU);
    }
  }
  return text;
}

int richelieu(QString& text, const QString key) {
  if (text.size() != key.size() || key.size() == 0)
    return 1;
  for (size_t i = 0; i < text.size(); ++i)
    if (key[i].unicode() < '1' || key[i].unicode() > '9')
      return 1;

  QString result = text;
  for (size_t i = 0, block_end = 0; i < text.size();) {
    block_end += key[block_end].unicode() - '0';
    for (size_t block_offset = i; i < block_end; ++i)
      result[i] = text[key[i].unicode() - '0' - 1 + block_offset];
  }
  text = result;
  return 0;
}
