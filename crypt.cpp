#include <QString>

#define UP_EN 155
#define LOW_EN 219
#define UP_RU 2111
#define LOW_RU 2175

QString atbash(QString text) {
  for (size_t i = 0; i < text.size(); ++i) {
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

int richelieu(QString& text, const QString key, const int is_decrypt) {
  size_t key_size = 0;
  size_t i = 0, j = 0;
  for (i = 0; i < key.size(); ++i) {
    if (key[i] >= '1' && key[i] <= '9' && key[i].unicode() - '0' <= text.size())
      ++key_size;
    else if (key[i] != ' ') 
      return EXIT_FAILURE;
  }
  if (text.size() != key_size || key_size == 0)
    return EXIT_FAILURE;

  QString result = text;
  if (is_decrypt) {
    for (i = 0, j = 0; i < text.size(); ++j)
      for (size_t block_offset = i; i < text.size() && key[i + j] != ' '; ++i)
        result[i] = text[key[i + j].unicode() - '0' - 1 + block_offset];
  }
  else {
    for (i = 0, j = 0; i < text.size(); ++j)
      for (size_t block_offset = i; i < text.size() && key[i + j] != ' '; ++i)
        result[key[i + j].unicode() - '0' - 1 + block_offset] = text[i];
  }
  text = result;
  return EXIT_SUCCESS;
}
