#include "richelieu.h"

int richelieu(QString& text, QString key, bool isDecrypt) {
  qsizetype i = 0, j = 0, keyLength = 0;
  QStringList blocks = key.split(' ', Qt::SkipEmptyParts);
  QStringList* numbers = new QStringList[blocks.length()];
  for (i = 0; i < blocks.length(); ++i) {
    numbers[i] = blocks[i].split(',', Qt::SkipEmptyParts);
    keyLength += numbers[i].length();
  }

  if (!keyLength)
    return EXIT_FAILURE;
  unsigned sum = 0;
  for (i = 0; i < blocks.length(); ++i) {
    for (j = 0; j < numbers[i].length(); ++j) {
      bool toIntIsOk;
      unsigned number = numbers[i][j].toUInt(&toIntIsOk);
      if (number > numbers[i].length() || !toIntIsOk ||
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
  int blockOffset;
  if (isDecrypt) {
    for (i = 0, j = 0; i < blocks.length(); ++i) {
      for (blockOffset = j; j < numbers[i].length() + blockOffset; ++j) {
        result[j] = text[numbers[i][j - blockOffset].toUInt()
                         - 1 + blockOffset];
      }
    }
  } else {
    for (i = 0, j = 0; i < blocks.length(); ++i) {
      for (blockOffset = j; j < numbers[i].length() + blockOffset; ++j) {
        result[numbers[i][j - blockOffset].toUInt() 
               - 1 + blockOffset] = text[j];
      }
    }
  }

  text = result;
  delete[] numbers;
  return EXIT_SUCCESS;
}

Richelieu::Richelieu() {
  labelKey = new QLabel("Key");
  teditKey = new QTextEdit();
  labelOperation = new QLabel("Operation");
  cboxOperation = new QComboBox();
  cboxOperation->addItem("Encrypt");
  cboxOperation->addItem("Decrypt");
  glayout->addWidget(labelKey, 2, 0);
  glayout->addWidget(teditKey, 2, 1);
  glayout->addWidget(labelOperation, 3, 0);
  glayout->addWidget(cboxOperation, 3, 1);
  connect(teditIn, &QTextEdit::textChanged, this, &Richelieu::teditInChanged);
  connect(teditKey, &QTextEdit::textChanged, this, &Richelieu::teditInChanged);
  connect(cboxOperation, &QComboBox::currentIndexChanged, this,
          &Richelieu::teditInChanged);
}


void Richelieu::teditInChanged() {
  QString text = teditIn->toPlainText();
  bool isDecrypt = cboxOperation->currentIndex();
  if (richelieu(text, teditKey->toPlainText(), isDecrypt))
    teditOut->setText("Incorrect key!");
  else
    teditOut->setText(text);
}

Richelieu::~Richelieu() {
  delete labelKey;
  delete labelOperation;
  delete teditKey;
  delete cboxOperation;
}
