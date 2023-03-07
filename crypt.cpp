#include <QString>
#include <QStringList>

#define UP_EN 155
#define LOW_EN 219
#define UP_RU 2111
#define LOW_RU 2175

QString atbash(QString text) {
  for (int i = 0; i < text.size(); ++i) {
    if (text[i] >= u'A' && text[i] <= u'Z')
      text[i] = (QChar)(UP_EN - text[i].unicode());
    else if (text[i] >= u'a' && text[i] <= u'z')
      text[i] = (QChar)(LOW_EN - text[i].unicode());
    else if (text[i] >= u'А' && text[i] <= u'Я')
      text[i] = (QChar)(UP_RU - text[i].unicode());
    else if (text[i] >= u'а' && text[i] <= u'я')
      text[i] = (QChar)(LOW_RU - text[i].unicode());
  }
  return text;
}

#define EN 26
#define RU 32

QString caesar(QString text, int step, const int is_decrypt) {
  if (is_decrypt)
    step = ~step + 1;

  for (int i = 0; i < text.length(); i++) {
    if (text[i] >= u'A' && text[i] <= u'Z') {
      text[i] = (QChar)(text[i].unicode() + step % EN);
      if (text[i] > u'Z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'A')
        text[i] = (QChar)(text[i].unicode() + EN);
    } else if (text[i] >= u'a' && text[i] <= u'z') {
      text[i] = (QChar)(text[i].unicode() + step % EN);
      if (text[i] > u'z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'a')
        text[i] = (QChar)(text[i].unicode() + EN);
    }

    else if (text[i] >= u'А' && text[i] <= u'Я') {
      text[i] = (QChar)(text[i].unicode() + step % RU);
      if (text[i] > u'Я')
        text[i] = (QChar)(text[i].unicode() - RU);
      else if (text[i] < u'А')
        text[i] = (QChar)(text[i].unicode() + RU);
    } else if (text[i] >= u'а' && text[i] <= u'я') {
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
  int i = 0, j = 0, key_size = 0;
  for (i = 0; i < key.size(); ++i) {
    if (key[i] != ' ' && key[i] != ',' && !key[i].isDigit())
      return EXIT_FAILURE;
  }
  QStringList blocks = key.split(' ', Qt::SkipEmptyParts);
  QStringList* numbers = new QStringList[blocks.length()];
  for (i = 0; i < blocks.length(); ++i) {
    numbers[i] = blocks[i].split(',', Qt::SkipEmptyParts);
    key_size += numbers[i].length();
  }

  if (!key_size)
    return EXIT_FAILURE;
  for (i = 0; i < blocks.length(); ++i) {
    for (j = 0; j < numbers[i].length(); ++j) {
      if (numbers[i][j].toInt() > numbers[i].length() || !numbers[i][j].toInt())
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
        result[j] =
            text[numbers[i][j - block_offset].toInt() - 1 + block_offset];
      }
    }
  } else {
    for (i = 0, j = 0; i < blocks.length(); ++i) {
      for (block_offset = j; j < numbers[i].length() + block_offset; ++j) {
        result[numbers[i][j - block_offset].toInt() - 1 + block_offset] =
            text[j];
      }
    }
  }

  text = result;
  delete[] numbers;
  return EXIT_SUCCESS;
}

int gronsfeld(QString& text, const QString key, const int is_decrypt) {
  int i, j;
  for (i = 0; i < key.length(); ++i) {
    if (key[i] != ',' && key[i] != '-' && !key[i].isDigit())
      return EXIT_FAILURE;
  }
  QStringList steps = key.split(',', Qt::SkipEmptyParts);
  if (!steps.length())
    return EXIT_FAILURE;
  if (is_decrypt)
    for (i = 0; i < steps.length(); ++i) {
      if (steps[i][0] == '-')
        steps[i].remove('-');
      else
        steps[i].insert(0, '-');
    }

  for (i = 0, j = 0; i < text.length(); ++i) {
    if (j == steps.length())
      j = 0;
    if (text[i] >= u'A' && text[i] <= u'Z') {
      text[i] = (QChar)(text[i].unicode() + steps[j++].toInt() % EN);
      if (text[i] > u'Z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'A')
        text[i] = (QChar)(text[i].unicode() + EN);
    } else if (text[i] >= u'a' && text[i] <= u'z') {
      text[i] = (QChar)(text[i].unicode() + steps[j++].toInt() % EN);
      if (text[i] > u'z')
        text[i] = (QChar)(text[i].unicode() - EN);
      else if (text[i] < u'a')
        text[i] = (QChar)(text[i].unicode() + EN);
    }

    else if (text[i] >= u'А' && text[i] <= u'Я') {
      text[i] = (QChar)(text[i].unicode() + steps[j++].toInt() % RU);
      if (text[i] > u'Я')
        text[i] = (QChar)(text[i].unicode() - RU);
      else if (text[i] < u'А')
        text[i] = (QChar)(text[i].unicode() + RU);
    } else if (text[i] >= u'а' && text[i] <= u'я') {
      text[i] = (QChar)(text[i].unicode() + steps[j++].toInt() % RU);
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
  int i, j;
  for (i = 0; i < key.length(); ++i) {
    if (!key[i].isLetter())
      return EXIT_FAILURE;
  }

  QChar tmp;
  enum {ENCRYPT, DECRYPT};
  for (i = 0, j = 0; i < text.length(); ++i) {
    if (j == key.length())
      j = 0;
    switch (is_decrypt) {
    case ENCRYPT:
      if ((text[i] >= u'a' && text[i] <= u'z') ||
          (text[i] >= u'A' && text[i] <= u'Z')) {
        tmp = (QChar)(text[i].unicode() + key[j++].unicode() % EN);
        if ((text[i] >= u'A' && text[i] <= u'Z' && tmp > u'Z') ||
            (text[i] >= u'a' && text[i] <= u'z' && tmp > u'z'))
          tmp = (QChar)(tmp.unicode() - EN);
        text[i] = tmp;
      } else if ((text[i] >= u'а' && text[i] <= u'я') ||
                 (text[i] >= u'А' && text[i] <= u'Я')) {
        tmp = (QChar)(text[i].unicode() + key[j++].unicode() % RU);
        if ((text[i] >= u'А' && text[i] <= u'Я' && tmp > u'Я') ||
            (text[i] >= u'а' && text[i] <= u'я' && tmp > u'я'))
          tmp = (QChar)(tmp.unicode() - RU);
        text[i] = tmp;
      }
      break;
    case DECRYPT:
      if ((text[i] >= u'a' && text[i] <= u'z') ||
          (text[i] >= u'A' && text[i] <= u'Z')) {
        tmp = (QChar)(text[i].unicode() - key[j++].unicode() % EN);
        if ((text[i] >= u'A' && text[i] <= u'Z' && tmp < u'A') ||
            (text[i] >= u'a' && text[i] <= u'z' && tmp < u'a'))
          tmp = (QChar)(tmp.unicode() + EN);
        text[i] = tmp;
      } else if ((text[i] >= u'а' && text[i] <= u'я') ||
                 (text[i] >= u'А' && text[i] <= u'Я')) {
        tmp = (QChar)(text[i].unicode() - key[j++].unicode() % RU);
        if ((text[i] >= u'А' && text[i] <= u'Я' && tmp < u'А') ||
            (text[i] >= u'а' && text[i] <= u'я' && tmp < u'а'))
          tmp = (QChar)(tmp.unicode() + RU);
        text[i] = tmp;
      }
    }
  }
  return EXIT_SUCCESS;
}
