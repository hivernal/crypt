#include "widget.h"

#include <QString>

QString atbash(QString text);
QString caesar(QString text, int step, const int is_decrypt);
int richelieu(QString& text, const QString key, const int is_decrypt);

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
  label_step = new QLabel("Step");
  spin_step = new QSpinBox();
  spin_step->setRange(-1024, 1024);
  spin_step->setMaximumWidth(100);
  spin_step->setAlignment(Qt::AlignCenter);
  glayout_caesar->addWidget(label_caesar_in, 0, 0);
  glayout_caesar->addWidget(tedit_caesar_in, 0, 1);
  glayout_caesar->addWidget(label_caesar_out, 1, 0);
  glayout_caesar->addWidget(tedit_caesar_out, 1, 1);
  glayout_caesar->addWidget(label_caesar_operation, 2, 0);
  glayout_caesar->addWidget(cbox_caesar_operation, 2, 1);
  glayout_caesar->addWidget(label_step, 3, 0);
  glayout_caesar->addWidget(spin_step, 3, 1);

  widget_richelieu = new QWidget();
  glayout_richelieu = new QGridLayout(widget_richelieu);
  label_richelieu_in = new QLabel("Original");
  tedit_richelieu_in = new QTextEdit();
  label_richelieu_out = new QLabel("Result");
  tedit_richelieu_out = new QTextEdit();
  label_key = new QLabel("Key");
  tedit_key = new QTextEdit();
  tedit_key->setMaximumHeight(27);
  tedit_key->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  label_richelieu_operation = new QLabel("Operation");
  cbox_richelieu_operation = new QComboBox();
  cbox_richelieu_operation->addItem("Encrypt");
  cbox_richelieu_operation->addItem("Decrypt");
  glayout_richelieu->addWidget(label_richelieu_in, 0, 0);
  glayout_richelieu->addWidget(tedit_richelieu_in, 0, 1);
  glayout_richelieu->addWidget(label_richelieu_out, 1, 0);
  glayout_richelieu->addWidget(tedit_richelieu_out, 1, 1);
  glayout_richelieu->addWidget(label_key, 2, 0);
  glayout_richelieu->addWidget(tedit_key, 2, 1);
  glayout_richelieu->addWidget(label_richelieu_operation, 3, 0);
  glayout_richelieu->addWidget(cbox_richelieu_operation, 3, 1);

  tab_widget = new QTabWidget();
  tab_widget->addTab(widget_atbash, "Atbash");
  tab_widget->addTab(widget_caesar, "Caesar");
  tab_widget->addTab(widget_richelieu, "Richelieu");
  glayout_main = new QGridLayout(this);
  glayout_main->addWidget(tab_widget);

  connect(tedit_atbash_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_atbash_in_text_changed);
  connect(tedit_caesar_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_caesar_in_text_changed);
  connect(spin_step, &QSpinBox::valueChanged, this,
          &Widget::on_text_edit_caesar_in_text_changed);
  connect(cbox_caesar_operation, &QComboBox::currentIndexChanged, this,
          &Widget::on_text_edit_caesar_in_text_changed);
  connect(tedit_richelieu_in, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_richelieu_in_text_changed);
  connect(tedit_key, &QTextEdit::textChanged, this,
          &Widget::on_text_edit_richelieu_in_text_changed);
  connect(cbox_richelieu_operation, &QComboBox::currentIndexChanged, this,
          &Widget::on_text_edit_richelieu_in_text_changed);
}

Widget::~Widget() {
  delete label_atbash_in;
  delete label_atbash_out;
  delete label_caesar_in;
  delete label_caesar_out;
  delete label_caesar_operation;
  delete label_step;
  delete label_richelieu_in;
  delete label_richelieu_out;
  delete label_key;
  delete label_richelieu_operation;

  delete tedit_atbash_in;
  delete tedit_atbash_out;
  delete tedit_caesar_in;
  delete tedit_caesar_out;
  delete tedit_richelieu_in;
  delete tedit_richelieu_out;
  delete tedit_key;

  delete glayout_caesar;
  delete glayout_atbash;
  delete glayout_richelieu;
  delete glayout_main;
  
  delete spin_step;
  delete cbox_caesar_operation;
  delete cbox_richelieu_operation;
  delete widget_caesar;
  delete widget_atbash;
  delete widget_richelieu;
  delete tab_widget;
}

void Widget::on_text_edit_atbash_in_text_changed() {
  QString text = atbash(tedit_atbash_in->toPlainText());
  tedit_atbash_out->setText(text);
}

void Widget::on_text_edit_caesar_in_text_changed() {
  int step = spin_step->value();
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
  if (!richelieu(text, tedit_key->toPlainText(), is_decrypt))
    tedit_richelieu_out->setText(text);
}
