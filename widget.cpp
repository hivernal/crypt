#include "widget.h"

#include <QString>

QString atbash(QString text);
QString caesar(QString text, int step, const int is_decrypt);
int richelieu(QString& text, const QString key, const int is_decrypt);
int gronsfeld(QString& text, const QString key, const int is_decrypt);
int visionary(QString& text, const QString key, const int is_decrypt);

Widget::Widget(QWidget *parent) : QWidget(parent) {
  this->setWindowTitle("Encryption");
  this->setMaximumSize(700, 500);

  widget_atbash = new QWidget();
  glayout_atbash = new QGridLayout(widget_atbash);
  label_atbash_in = new QLabel("Original");
  tedit_atbash_in = new QTextEdit();
  label_atbash_out = new QLabel("Result");
  tedit_atbash_out = new QTextEdit();
  glayout_atbash->addWidget(label_atbash_in, 0, 0);
  glayout_atbash->addWidget(tedit_atbash_in, 0, 1);
  glayout_atbash->addWidget(label_atbash_out, 1, 0);
  glayout_atbash->addWidget(tedit_atbash_out, 1, 1);

  widget_caesar = new QWidget();
  glayout_caesar = new QGridLayout(widget_caesar);
  label_caesar_in = new QLabel("Original");
  tedit_caesar_in = new QTextEdit();
  label_caesar_out = new QLabel("Result");
  tedit_caesar_out = new QTextEdit();
  label_caesar_operation = new QLabel("Operation");
  cbox_caesar_operation = new QComboBox();
  cbox_caesar_operation->addItem("Encrypt");
  cbox_caesar_operation->addItem("Decrypt");
  label_caesar_step = new QLabel("Step");
  spin_caesar_step = new QSpinBox();
  spin_caesar_step->setRange(-1024, 1024);
  spin_caesar_step->setMaximumWidth(100);
  spin_caesar_step->setAlignment(Qt::AlignCenter);
  glayout_caesar->addWidget(label_caesar_in, 0, 0);
  glayout_caesar->addWidget(tedit_caesar_in, 0, 1);
  glayout_caesar->addWidget(label_caesar_out, 1, 0);
  glayout_caesar->addWidget(tedit_caesar_out, 1, 1);
  glayout_caesar->addWidget(label_caesar_operation, 2, 0);
  glayout_caesar->addWidget(cbox_caesar_operation, 2, 1);
  glayout_caesar->addWidget(label_caesar_step, 3, 0);
  glayout_caesar->addWidget(spin_caesar_step, 3, 1);

  widget_richelieu = new QWidget();
  glayout_richelieu = new QGridLayout(widget_richelieu);
  label_richelieu_in = new QLabel("Original");
  tedit_richelieu_in = new QTextEdit();
  label_richelieu_out = new QLabel("Result");
  tedit_richelieu_out = new QTextEdit();
  label_richelieu_key = new QLabel("Key");
  tedit_richelieu_key = new QTextEdit();
  tedit_richelieu_key->setMaximumHeight(30);
  tedit_richelieu_key->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  label_richelieu_operation = new QLabel("Operation");
  cbox_richelieu_operation = new QComboBox();
  cbox_richelieu_operation->addItem("Encrypt");
  cbox_richelieu_operation->addItem("Decrypt");
  glayout_richelieu->addWidget(label_richelieu_in, 0, 0);
  glayout_richelieu->addWidget(tedit_richelieu_in, 0, 1);
  glayout_richelieu->addWidget(label_richelieu_out, 1, 0);
  glayout_richelieu->addWidget(tedit_richelieu_out, 1, 1);
  glayout_richelieu->addWidget(label_richelieu_key, 2, 0);
  glayout_richelieu->addWidget(tedit_richelieu_key, 2, 1);
  glayout_richelieu->addWidget(label_richelieu_operation, 3, 0);
  glayout_richelieu->addWidget(cbox_richelieu_operation, 3, 1);

  widget_gronsfeld = new QWidget();
  glayout_gronsfeld = new QGridLayout(widget_gronsfeld);
  label_gronsfeld_in = new QLabel("Original");
  tedit_gronsfeld_in = new QTextEdit();
  label_gronsfeld_out = new QLabel("Result");
  tedit_gronsfeld_out = new QTextEdit();
  label_gronsfeld_key = new QLabel("Key");
  tedit_gronsfeld_key = new QTextEdit();
  tedit_gronsfeld_key->setMaximumHeight(30);
  tedit_gronsfeld_key->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  label_gronsfeld_operation = new QLabel("Operation");
  cbox_gronsfeld_operation = new QComboBox();
  cbox_gronsfeld_operation->addItem("Encrypt");
  cbox_gronsfeld_operation->addItem("Decrypt");
  glayout_gronsfeld->addWidget(label_gronsfeld_in, 0, 0);
  glayout_gronsfeld->addWidget(tedit_gronsfeld_in, 0, 1);
  glayout_gronsfeld->addWidget(label_gronsfeld_out, 1, 0);
  glayout_gronsfeld->addWidget(tedit_gronsfeld_out, 1, 1);
  glayout_gronsfeld->addWidget(label_gronsfeld_key, 2, 0);
  glayout_gronsfeld->addWidget(tedit_gronsfeld_key, 2, 1);
  glayout_gronsfeld->addWidget(label_gronsfeld_operation, 3, 0);
  glayout_gronsfeld->addWidget(cbox_gronsfeld_operation, 3, 1);

  widget_visionary = new QWidget();
  glayout_visionary = new QGridLayout(widget_visionary);
  label_visionary_in = new QLabel("Original");
  tedit_visionary_in = new QTextEdit();
  label_visionary_out = new QLabel("Result");
  tedit_visionary_out = new QTextEdit();
  label_visionary_key = new QLabel("Key");
  tedit_visionary_key = new QTextEdit();
  tedit_visionary_key->setMaximumHeight(30);
  tedit_visionary_key->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  label_visionary_operation = new QLabel("Operation");
  cbox_visionary_operation = new QComboBox();
  cbox_visionary_operation->addItem("Encrypt");
  cbox_visionary_operation->addItem("Decrypt");
  glayout_visionary->addWidget(label_visionary_in, 0, 0);
  glayout_visionary->addWidget(tedit_visionary_in, 0, 1);
  glayout_visionary->addWidget(label_visionary_out, 1, 0);
  glayout_visionary->addWidget(tedit_visionary_out, 1, 1);
  glayout_visionary->addWidget(label_visionary_key, 2, 0);
  glayout_visionary->addWidget(tedit_visionary_key, 2, 1);
  glayout_visionary->addWidget(label_visionary_operation, 3, 0);
  glayout_visionary->addWidget(cbox_visionary_operation, 3, 1);

  widget_playfair = new QWidget();
  glayout_playfair = new QGridLayout(widget_playfair);
  label_playfair_in = new QLabel("Original");
  tedit_playfair_in = new QTextEdit();
  label_playfair_out = new QLabel("Result");
  tedit_playfair_out = new QTextEdit();
  label_playfair_key = new QLabel("Key");
  tedit_playfair_key = new QTextEdit();
  tedit_playfair_key->setMaximumHeight(30);
  tedit_playfair_key->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  label_playfair_operation = new QLabel("Operation");
  cbox_playfair_operation = new QComboBox();
  cbox_playfair_operation->addItem("Encrypt");
  cbox_playfair_operation->addItem("Decrypt");
  glayout_playfair->addWidget(label_playfair_in, 0, 0);
  glayout_playfair->addWidget(tedit_playfair_in, 0, 1);
  glayout_playfair->addWidget(label_playfair_out, 1, 0);
  glayout_playfair->addWidget(tedit_playfair_out, 1, 1);
  glayout_playfair->addWidget(label_playfair_key, 2, 0);
  glayout_playfair->addWidget(tedit_playfair_key, 2, 1);
  glayout_playfair->addWidget(label_playfair_operation, 3, 0);
  glayout_playfair->addWidget(cbox_playfair_operation, 3, 1);

  tab_widget = new QTabWidget();
  tab_widget->addTab(widget_atbash, "Atbash");
  tab_widget->addTab(widget_caesar, "Caesar");
  tab_widget->addTab(widget_richelieu, "Richelieu");
  tab_widget->addTab(widget_gronsfeld, "Gronsfeld");
  tab_widget->addTab(widget_visionary, "Visionary");
  tab_widget->addTab(widget_playfair, "Playfair");
  glayout_main = new QGridLayout(this);
  glayout_main->addWidget(tab_widget);

  connect(tedit_atbash_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_atbash_in_text_changed);

  connect(tedit_caesar_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_caesar_in_text_changed);
  connect(spin_caesar_step, &QSpinBox::valueChanged, this,
          &Widget::on_text_edit_caesar_in_text_changed);
  connect(cbox_caesar_operation, &QComboBox::currentIndexChanged, this,
          &Widget::on_text_edit_caesar_in_text_changed);

  connect(tedit_richelieu_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_richelieu_in_text_changed);
  connect(tedit_richelieu_key, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_richelieu_in_text_changed);
  connect(cbox_richelieu_operation, &QComboBox::currentIndexChanged, this,
          &Widget::on_text_edit_richelieu_in_text_changed);

  connect(tedit_gronsfeld_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_gronsfeld_in_text_changed);
  connect(tedit_gronsfeld_key, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_gronsfeld_in_text_changed);
  connect(cbox_gronsfeld_operation, &QComboBox::currentIndexChanged, this,
          &Widget::on_text_edit_gronsfeld_in_text_changed);

  connect(tedit_visionary_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_visionary_in_text_changed);
  connect(tedit_visionary_key, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_visionary_in_text_changed);
  connect(cbox_visionary_operation, &QComboBox::currentIndexChanged, this,
          &Widget::on_text_edit_visionary_in_text_changed);

  connect(tedit_playfair_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_playfair_in_text_changed);
  connect(tedit_playfair_key, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_playfair_in_text_changed);
  connect(cbox_playfair_operation, &QComboBox::currentIndexChanged, this,
          &Widget::on_text_edit_playfair_in_text_changed);
}

Widget::~Widget() {
  delete label_atbash_in;
  delete tedit_atbash_in;
  delete tedit_atbash_out;
  delete label_atbash_out;
  delete glayout_atbash;
  delete widget_atbash;

  delete label_caesar_in;
  delete label_caesar_out;
  delete label_caesar_operation;
  delete label_caesar_step;
  delete tedit_caesar_in;
  delete tedit_caesar_out;
  delete glayout_caesar;
  delete cbox_caesar_operation;
  delete spin_caesar_step;
  delete widget_caesar;

  delete label_richelieu_in;
  delete label_richelieu_out;
  delete label_richelieu_key;
  delete label_richelieu_operation;
  delete tedit_richelieu_in;
  delete tedit_richelieu_out;
  delete tedit_richelieu_key;
  delete glayout_richelieu;
  delete cbox_richelieu_operation;
  delete widget_richelieu;

  delete label_gronsfeld_in;
  delete label_gronsfeld_out;
  delete label_gronsfeld_key;
  delete label_gronsfeld_operation;
  delete tedit_gronsfeld_in;
  delete tedit_gronsfeld_out;
  delete tedit_gronsfeld_key;
  delete glayout_gronsfeld;
  delete cbox_gronsfeld_operation;
  delete widget_gronsfeld;

  delete label_visionary_in;
  delete label_visionary_out;
  delete label_visionary_key;
  delete label_visionary_operation;
  delete tedit_visionary_in;
  delete tedit_visionary_out;
  delete tedit_visionary_key;
  delete glayout_visionary;
  delete cbox_visionary_operation;
  delete widget_visionary;

  delete label_playfair_in;
  delete label_playfair_out;
  delete label_playfair_key;
  delete label_playfair_operation;
  delete tedit_playfair_in;
  delete tedit_playfair_out;
  delete tedit_playfair_key;
  delete glayout_playfair;
  delete cbox_playfair_operation;
  delete widget_playfair;

  delete glayout_main;
  delete tab_widget;
}

void Widget::on_text_edit_atbash_in_text_changed() {
  QString text = atbash(tedit_atbash_in->toPlainText());
  tedit_atbash_out->setText(text);
}

void Widget::on_text_edit_caesar_in_text_changed() {
  int step = spin_caesar_step->value();
  QString qstr = tedit_caesar_in->toPlainText();
  int is_decrypt = 0;
  if (cbox_caesar_operation->currentIndex())
    is_decrypt = 1;
  tedit_caesar_out->setText(caesar(qstr, step, is_decrypt));
}

void Widget::on_text_edit_richelieu_in_text_changed() {
  QString text = tedit_richelieu_in->toPlainText();
  int is_decrypt = 0;
  if (cbox_richelieu_operation->currentIndex())
    is_decrypt = 1;
  if (richelieu(text, tedit_richelieu_key->toPlainText(), is_decrypt))
    tedit_richelieu_out->setText("Incorrect key!");
  else
    tedit_richelieu_out->setText(text);
}

void Widget::on_text_edit_gronsfeld_in_text_changed() {
  QString text = tedit_gronsfeld_in->toPlainText();
  int is_decrypt = 0;
  if (cbox_gronsfeld_operation->currentIndex())
    is_decrypt = 1;
  if (gronsfeld(text, tedit_gronsfeld_key->toPlainText(), is_decrypt))
    tedit_gronsfeld_out->setText("Incorrect key!");
  else
    tedit_gronsfeld_out->setText(text);
}

void Widget::on_text_edit_visionary_in_text_changed() {
  QString text = tedit_visionary_in->toPlainText();
  int is_decrypt = 0;
  if (cbox_visionary_operation->currentIndex())
    is_decrypt = 1;
  if (visionary(text, tedit_visionary_key->toPlainText(), is_decrypt))
    tedit_visionary_out->setText("Incorrect key!");
  else
    tedit_visionary_out->setText(text);
}

void Widget::on_text_edit_playfair_in_text_changed() {
  QString text = tedit_playfair_in->toPlainText();
}
