#include "playfair.h"

#include <iostream>

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

  std::wcout << std::endl;
  for (i = 0; i < NROWS; ++i) {
    QString qstr;
    qstr.clear();
    for (j = 0; j < NCOLUMNS; ++j) {
      qstr.push_back(matrix[i][j]);
      qstr.push_back(' ');
    }
    std::wcout << qstr.toStdWString();
    std::wcout << std::endl;
  }

  return EXIT_SUCCESS;
}

Playfair::Playfair() {
  connect(teditIn, &QTextEdit::textChanged, this,
          &Playfair::teditInChanged);
  connect(teditKey, &QTextEdit::textChanged, this,
          &Playfair::teditInChanged);
  connect(cboxOperation, &QComboBox::currentIndexChanged, this,
          &Playfair::teditInChanged);
}

void Playfair::teditInChanged() {
  QString text = teditIn->toPlainText();
  bool is_decrypt = false;
  if (cboxOperation->currentIndex())
    is_decrypt = true;
  if (playfair(text, teditKey->toPlainText(), is_decrypt))
    teditOut->setText("Incorrect key!");
  else
    teditOut->setText(text);
}
