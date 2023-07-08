#include "playfair.h"

const QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя12345";

enum {
  nrows = 11, ncolumns = 11
} ;

int playfair(QString& text, QString key, bool isDecrypt) {
  if (!key.length())
    return EXIT_FAILURE;

  QChar matrix[nrows][ncolumns + 1];
  qsizetype i = 0, j = 0, k = 0;
  while (k < key.length()) {
    if (!alphabet.contains(key[k]))
      return EXIT_FAILURE;
    if (j == ncolumns) {
      matrix[i][j] = '\0';
      j = 0;
      ++i;
    }
    if (key.indexOf(key[k]) >= k)
      matrix[i][j++] = key[k];
    ++k;
  }
  for (k = 0; i < nrows; ++i, j %= ncolumns) {
    for (; j < ncolumns; ++k) {
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
    for (i1 = 0; i1 < nrows; ++i1) {
      for (j1 = 0; j1 < ncolumns; ++j1) {
        if (matrix[i1][j1] == text[i]) break;
      }
      if (matrix[i1][j1] == text[i]) break;
    }
    for (i2 = 0; i2 < nrows; ++i2) {
      for (j2 = 0; j2 < ncolumns; ++j2) {
        if (matrix[i2][j2] == text[j]) break;
      }
      if (matrix[i2][j2] == text[j]) break;
    }
    if (i1 == i2) {
      if (isDecrypt) {
        text[i] = matrix[i1][(j1 - 1 + ncolumns) % ncolumns];
        text[j] = matrix[i2][(j2 - 1 + ncolumns) % ncolumns];
      } else {
        text[i] = matrix[i1][(j1 + 1) % ncolumns];
        text[j] = matrix[i2][(j2 + 1) % ncolumns];
      }
    } else if (j1 == j2) {
      if (isDecrypt) {
        text[i] = matrix[(i1 - 1 + nrows) % nrows][j1];
        text[j] = matrix[(i2 - 1 + nrows) % nrows][j2];
      } else {
        text[i] = matrix[(i1 + 1) % nrows][j1];
        text[j] = matrix[(i2 + 1) % nrows][j2];
      }
    } else {
      text[i] = matrix[i1][j2];
      text[j] = matrix[i2][j1];
    }
  }

  QTextStream out(stdout);
  out << '\n';
  for (i = 0; i < nrows; ++i) {
    for (j = 0; j < ncolumns; ++j) {
      out << matrix[i][j] << ' ';
    }
    out << '\n';
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
  bool isDecrypt = cboxOperation->currentIndex();
  if (playfair(text, teditKey->toPlainText(), isDecrypt))
    teditOut->setText("Incorrect key!");
  else
    teditOut->setText(text);
}
