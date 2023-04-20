#include "richelieu.h"

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
  bool is_decrypt = false;
  if (cboxOperation->currentIndex())
    is_decrypt = true;
  if (richelieu(text, teditKey->toPlainText(), is_decrypt))
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
