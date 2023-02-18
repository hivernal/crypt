#ifndef WIDGET_H
#define WIDGET_H

#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

class Widget : public QWidget {
  Q_OBJECT

  QTabWidget* tab_widget;
  QGridLayout* glayout_main;

  QWidget* widget_atbash;
  QGridLayout* glayout_atbash;
  QTextEdit* tedit_atbash_in;
  QTextEdit* tedit_atbash_out;
  QLabel* label_atbash_in;
  QLabel* label_atbash_out;

  QWidget* widget_caesar;
  QGridLayout* glayout_caesar;
  QLabel* label_caesar_in;
  QTextEdit* tedit_caesar_in;
  QLabel* label_caesar_out;
  QTextEdit* tedit_caesar_out;
  QLabel* label_caesar_operation;
  QComboBox* cbox_caesar_operation;
  QLabel* label_step;
  QSpinBox* spin_step;

  QWidget* widget_richelieu;
  QGridLayout* glayout_richelieu;
  QLabel* label_richelieu_in;
  QTextEdit* tedit_richelieu_in;
  QLabel* label_richelieu_out;
  QTextEdit* tedit_richelieu_out;
  QLabel* label_key;
  QTextEdit* tedit_key;
  QLabel* label_richelieu_operation;
  QComboBox* cbox_richelieu_operation;

public:
  Widget(QWidget* parent = nullptr);
  ~Widget();

private slots:
  void on_text_edit_atbash_in_text_changed();
  void on_text_edit_caesar_in_text_changed();
  void on_text_edit_richelieu_in_text_changed();

private:
};
#endif // WIDGET_H
