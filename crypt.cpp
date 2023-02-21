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
  QRegularExpression blocks_re("(,?[0-9])+");
  QRegularExpression numbers_re("[0-9]+");
  QRegularExpressionMatchIterator blocks_iterator, numbers_iterator;

  size_t i = 0, j = 0;
  for (i = 0; i < key.size(); ++i) {
    if (key[i] != ' ' && key[i] != ',' && !key[i].isDigit()) 
      return EXIT_FAILURE;
  }
  size_t blocks_count = 0;
  blocks_iterator = blocks_re.globalMatch(key);
  while (blocks_iterator.hasNext()) {
    blocks_iterator.next();
    ++blocks_count;
  }
  size_t* blocks_size = new size_t[blocks_count];
  size_t** numbers = new size_t* [blocks_count];
  size_t key_size = 0;
  blocks_iterator = blocks_re.globalMatch(key);
  for (i = 0; blocks_iterator.hasNext(); ++i) {
    numbers_iterator = numbers_re.globalMatch(blocks_iterator.next().captured());
    for (j = 0; numbers_iterator.hasNext(); ++j) {
      numbers_iterator.next();
    }
    blocks_size[i] = j;
    numbers[i] = new size_t[j];
    key_size += j;
  }

  blocks_iterator = blocks_re.globalMatch(key);
  for (i = 0; blocks_iterator.hasNext(); ++i) {
    numbers_iterator = numbers_re.globalMatch(blocks_iterator.next().captured());
    for (j = 0; numbers_iterator.hasNext(); ++j) {
      numbers[i][j] = numbers_iterator.next().captured().toInt();
      if (numbers[i][j] > blocks_size[i] || numbers[i][j] == 0) {
        for (i = 0; i < blocks_count; ++i)
          delete[] numbers[i];
        delete[] numbers;
        delete[] blocks_size;
        return EXIT_FAILURE;
      }
    }
  }
  if (text.size() != key_size || key_size == 0) {
    for (i = 0; i < blocks_count; ++i)
      delete[] numbers[i];
    delete[] numbers;
    delete[] blocks_size;
    return EXIT_FAILURE;
  }

  QString result = text;
  size_t block_offset;
  if (is_decrypt) {
    for (j = 0, i = 0; j < blocks_count;  ++j) {
      for (block_offset = i; i < blocks_size[j] + block_offset; ++i) {
        result[i] = text[numbers[j][i - block_offset] - 1 + block_offset];
      }
    }
  }
  else {
    for (i = 0, j = 0; i < blocks_count;  ++i) {
      for (block_offset = j; j < blocks_size[i] + block_offset; ++j) {
        result[numbers[i][j - block_offset] - 1 + block_offset] = text[j];
      }
    }
  }

  text = result;
  for (i = 0; i < blocks_count; ++i)
    delete[] numbers[i];
  delete[] numbers;
  delete [] blocks_size;
  return EXIT_SUCCESS;
}
