#include "scaling.h"
#include "macros.h"

#include <QList>

void gen(QList<int>& gamma, int m, int a, int c, int x0) {
  gamma.push_back(x0);
  for (int i = 1; i < m; ++i) {
    x0 = (x0 * a + c) % m;
    gamma.push_back(x0);
  }
}

int scaling(QString& text, QString key, bool is_decrypt) {
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

Scaling::Scaling() {
  labelKey->setText("Gamma");

  labelOptions = new QLabel("Options");

  labelX0 = new QLabel("X0:");
  labelX0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxX0 = new QSpinBox();
  sboxX0->setAlignment(Qt::AlignCenter);
  sboxX0->setMaximum(INT_MAX);

  labelM = new QLabel("M:");
  labelM->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxM = new QSpinBox();
  sboxM->setAlignment(Qt::AlignCenter);
  sboxM->setMaximum(INT_MAX);
  sboxM->setMinimum(INT_MIN);

  labelC = new QLabel("C:");
  labelC->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxC = new QSpinBox();
  sboxC->setAlignment(Qt::AlignCenter);
  sboxC->setMaximum(INT_MAX);
  sboxC->setMinimum(INT_MIN);

  labelA = new QLabel("A:");
  labelA->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  sboxA = new QSpinBox();
  sboxA->setAlignment(Qt::AlignCenter);
  sboxA->setMaximum(INT_MAX);
  sboxA->setMinimum(INT_MIN);

  pbuttonGen = new QPushButton("generate");

  hlayoutOptions = new QHBoxLayout();
  hlayoutOptions->addWidget(labelX0);
  hlayoutOptions->addWidget(sboxX0);
  hlayoutOptions->addWidget(labelM);
  hlayoutOptions->addWidget(sboxM);
  hlayoutOptions->addWidget(labelC);
  hlayoutOptions->addWidget(sboxC);
  hlayoutOptions->addWidget(labelA);
  hlayoutOptions->addWidget(sboxA);
  hlayoutOptions->addWidget(pbuttonGen);

  glayout->addWidget(labelOptions, 3, 0);
  glayout->addLayout(hlayoutOptions, 3, 1);
  glayout->addWidget(labelOperation, 4, 0);
  glayout->addWidget(cboxOperation, 4, 1);

  connect(teditIn, &QTextEdit::textChanged, this,
          &Scaling::teditInChanged);
  connect(teditKey, &QTextEdit::textChanged, this,
          &Scaling::teditInChanged);
  connect(cboxOperation, &QComboBox::currentIndexChanged, this,
          &Scaling::teditInChanged);
  connect(pbuttonGen, &QPushButton::clicked, this,
          &Scaling::pbuttonGenClicked);
}

void Scaling::teditInChanged() {
  QString text = teditIn->toPlainText();
  bool is_decrypt = false;
  if (cboxOperation->currentIndex())
    is_decrypt = true;
  if (scaling(text, teditKey->toPlainText(), is_decrypt))
    teditOut->setText("Incorrect key!");
  else
    teditOut->setText(text);
}

void Scaling::pbuttonGenClicked() {
  QList<int> gamma;
  gen(gamma, sboxM->value(), sboxA->value(), sboxC->value(), sboxX0->value());
  QString key;
  for (int i = 0; i < gamma.length(); ++i) {
    key.push_back(QString::number(gamma[i]));
    if (i < gamma.length() - 1)
      key.push_back(',');
  }
  teditKey->setText(key);
}

Scaling::~Scaling() {
  delete labelOptions;
  delete labelX0;
  delete sboxX0;
  delete labelM;
  delete sboxM;
  delete labelC;
  delete sboxC;
  delete labelA;
  delete sboxA;
  delete pbuttonGen;
  delete hlayoutOptions;
}
