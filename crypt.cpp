#include <QString>
#include <QStringList>

#define IS_UP_EN(symbol) ((symbol) >= u'A' && (symbol) <= u'Z')
#define IS_LOW_EN(symbol) ((symbol) >= u'a' && (symbol) <= u'z')
#define IS_UP_RU(symbol) ((symbol) >= u'А' && (symbol) <= u'Я')
#define IS_LOW_RU(symbol) ((symbol) >= u'а' && (symbol) <= u'я')

#define UP_EN 155
#define LOW_EN 219
#define UP_RU 2111
#define LOW_RU 2175

#define EN 26
#define RU 32

enum {ENCRYPT, DECRYPT};

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

QString caesar(QString text, int step, const int is_decrypt) {
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

int richelieu(QString& text, const QString key, const int is_decrypt) {
  size_t i = 0, j = 0, key_size = 0;
  QStringList blocks = key.split(' ', Qt::SkipEmptyParts);
  QStringList* numbers = new QStringList[blocks.length()];
  for (i = 0; i < blocks.length(); ++i) {
    numbers[i] = blocks[i].split(',', Qt::SkipEmptyParts);
    key_size += numbers[i].length();
  }

  if (!key_size)
    return EXIT_FAILURE;
  bool to_int_is_ok;
  for (i = 0; i < blocks.length(); ++i) {
    for (j = 0; j < numbers[i].length(); ++j) {
      if (numbers[i][j].toUInt(&to_int_is_ok) > numbers[i].length() ||
          !to_int_is_ok || !numbers[i][j].toUInt())
        return EXIT_FAILURE;
      for (int k = j + 1; k < numbers[i].length(); ++k) {
        if (numbers[i][j] == numbers[i][k])
          return EXIT_FAILURE;
      }
    }
  }

  QString result = text;
  int block_offset;
  if (is_decrypt) {
    for (i = 0, j = 0; i < blocks.length(); ++i) {
      for (block_offset = j; j < numbers[i].length() + block_offset; ++j) {
        result[j] = text[numbers[i][j - block_offset].toUInt()
                    - 1 + block_offset];
      }
    }
  } else {
    for (i = 0, j = 0; i < blocks.length(); ++i) {
      for (block_offset = j; j < numbers[i].length() + block_offset; ++j) {
        result[numbers[i][j - block_offset].toUInt() 
               - 1 + block_offset] = text[j];
      }
    }
  }

  text = result;
  delete[] numbers;
  return EXIT_SUCCESS;
}

int gronsfeld(QString& text, const QString key, const int is_decrypt) {
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

int visionary(QString& text, QString key, const int is_decrypt) {
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

    switch (is_decrypt) {
    case ENCRYPT:
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
      break;
    case DECRYPT:
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
  }
  return EXIT_SUCCESS;
}
