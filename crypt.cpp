#include <QString>
#include <QRegularExpressionMatchIterator>

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
  size_t i = 0, j = 0, key_size = 0;
  for (i = 0; i < key.size(); ++i) {
    if (key[i] != ' ' && key[i] != ',' && !key[i].isDigit()) {
      text = "Incorrect key!";
      return EXIT_SUCCESS;
    }
  }
  QStringList blocks = key.split(' ', Qt::SkipEmptyParts);
  QStringList* numbers = new QStringList[blocks.length()];
  for (i = 0; i < blocks.length(); ++i) {
    numbers[i] = blocks[i].split(',', Qt::SkipEmptyParts);
    key_size += numbers[i].length();
  }

  if (key_size != text.size() || !key_size) {
    text = "Incorrect key!";
    return EXIT_SUCCESS;
  }
  for (i = 0; i < blocks.length(); ++i) {
    for (j = 0; j < numbers[i].length(); ++j) {
      if (numbers[i][j].toInt() > numbers[i].length() || !numbers[i][j].toInt()) {
        text = "Incorrect key!";
        return EXIT_SUCCESS;
      }
      for (size_t k = 0; k < numbers[i].length(); ++k) {
        if (numbers[i][j] == numbers[i][k] && k != j) {
          text = "Incorrect key!";
          return EXIT_SUCCESS;
        }
      }
    }
  }

  QString result = text;
  size_t block_offset;
  if (is_decrypt) {
    for (i = 0, j = 0; i < blocks.length();  ++i) {
      for (block_offset = j; j < numbers[i].length() + block_offset; ++j) {
        result[j] = text[numbers[i][j - block_offset].toInt() - 1 + block_offset];
      }
    }
  }
  else {
    for (i = 0, j = 0; i < blocks.length();  ++i) {
      for (block_offset = j; j < numbers[i].length() + block_offset; ++j) {
        result[numbers[i][j - block_offset].toInt() - 1 + block_offset] = text[j];
      }
    }
  }

  text = result;
  delete [] numbers;
  return EXIT_SUCCESS;
}
