#include <QString>
#include <QStringList>

#define IS_UP_EN(symbol) ((symbol) >= u'A' && (symbol) <= u'Z')
#define IS_LOW_EN(symbol) ((symbol) >= u'a' && (symbol) <= u'z')
#define IS_UP_RU(symbol) ((symbol) >= u'А' && (symbol) <= u'Я')
#define IS_LOW_RU(symbol) ((symbol) >= u'а' && (symbol) <= u'я')
#define IS_EN(symbol) (IS_LOW_EN(symbol) && IS_UP_EN(symbol))
#define IS_RU(symbol) (IS_LOW_RU(symbol) && IS_UP_RU(symbol))

#define UP_EN 155
#define LOW_EN 219
#define UP_RU 2111
#define LOW_RU 2175

#define EN 26
#define RU 32

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

int richelieu(QString& text, QString key, bool is_decrypt) {
  qsizetype i = 0, j = 0, key_size = 0;
  QStringList blocks = key.split(' ', Qt::SkipEmptyParts);
  QStringList* numbers = new QStringList[blocks.length()];
  for (i = 0; i < blocks.length(); ++i) {
    numbers[i] = blocks[i].split(',', Qt::SkipEmptyParts);
    key_size += numbers[i].length();
  }

  if (!key_size)
    return EXIT_FAILURE;
  unsigned sum = 0;
  for (i = 0; i < blocks.length(); ++i) {
    for (j = 0; j < numbers[i].length(); ++j) {
      bool to_int_is_ok;
      unsigned number = numbers[i][j].toUInt(&to_int_is_ok);
      if (number > numbers[i].length() || !to_int_is_ok ||
          !number || number > text.length())
        return EXIT_FAILURE;
      for (qsizetype k = j + 1; k < numbers[i].length(); ++k) {
        if (numbers[i][j] == numbers[i][k])
          return EXIT_FAILURE;
      }
    }
    sum += numbers[i].length();
  }
  if (sum > text.length()) 
    return EXIT_FAILURE;

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

    else {
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

#define NROWS 11
#define NCOLUMNS 11

QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя12345";

int playfair(QString& text, QString key, bool is_decrypt) {
  if (!key.length())
    return EXIT_FAILURE;

  QChar matrix[NROWS][NCOLUMNS + 1];
  qsizetype i = 0, j = 0, k = 0;
  while (k < key.length()) {
    if (!alphabet.contains(key[k]))
      return EXIT_FAILURE;
    if (j == NCOLUMNS) {
      matrix[i][j] = '\0';
      j = 0;
      ++i;
    }
    if (key.indexOf(key[k]) >= k)
      matrix[i][j++] = key[k];
    ++k;
  }
  for (k = 0; i < NROWS; ++i, j %= NCOLUMNS) {
    for (; j < NCOLUMNS; ++k) {
      if (!key.contains(alphabet[k]))
        matrix[i][j++] = alphabet[k];
    }
  }

  for (i = 0; i < text.length(); i = j + 1) {
    for (; i < text.length() && !alphabet.contains(text[i]); ++i);
    if (i == text.length())
      break;
    for (j = i + 1; j < text.length() && !alphabet.contains(text[j]); ++j);
    if (j == text.length())
      text.push_back('x');
    else if (text[i] == text[j])
      text.insert(j, 'x');

    qsizetype i1 = 0, j1 = 0, i2 = 0, j2 = 0;
    for (i1 = 0; i1 < NROWS; ++i1) {
      for (j1 = 0; j1 < NCOLUMNS; ++j1) {
        if (matrix[i1][j1] == text[i]) break;
      }
      if (matrix[i1][j1] == text[i]) break;
    }
    for (i2 = 0; i2 < NROWS; ++i2) {
      for (j2 = 0; j2 < NCOLUMNS; ++j2) {
        if (matrix[i2][j2] == text[j]) break;
      }
      if (matrix[i2][j2] == text[j]) break;
    }
    if (i1 == i2) {
      if (is_decrypt) {
        text[i] = matrix[i1][(j1 - 1 + NCOLUMNS) % NCOLUMNS];
        text[j] = matrix[i2][(j2 - 1 + NCOLUMNS) % NCOLUMNS];
      } else {
        text[i] = matrix[i1][(j1 + 1) % NCOLUMNS];
        text[j] = matrix[i2][(j2 + 1) % NCOLUMNS];
      }
    } else if (j1 == j2) {
      if (is_decrypt) {
        text[i] = matrix[(i1 - 1 + NROWS) % NROWS][j1];
        text[j] = matrix[(i2 - 1 + NROWS) % NROWS][j2];
      } else {
        text[i] = matrix[(i1 + 1) % NROWS][j1];
        text[j] = matrix[(i2 + 1) % NROWS][j2];
      }
    } else {
      text[i] = matrix[i1][j2];
      text[j] = matrix[i2][j1];
    }
  }

  return EXIT_SUCCESS;
}
