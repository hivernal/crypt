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
  QLabel* label_richelieu_key;
  QTextEdit* tedit_richelieu_key;
  QLabel* label_richelieu_operation;
  QComboBox* cbox_richelieu_operation;

  QWidget* widget_gronsfeld;
  QGridLayout* glayout_gronsfeld;
  QLabel* label_gronsfeld_in;
  QTextEdit* tedit_gronsfeld_in;
  QLabel* label_gronsfeld_out;
  QTextEdit* tedit_gronsfeld_out;
  QLabel* label_gronsfeld_key;
  QTextEdit* tedit_gronsfeld_key;
  QLabel* label_gronsfeld_operation;
  QComboBox* cbox_gronsfeld_operation;

  QWidget* widget_visionary;
  QGridLayout* glayout_visionary;
  QLabel* label_visionary_in;
  QTextEdit* tedit_visionary_in;
  QLabel* label_visionary_out;
  QTextEdit* tedit_visionary_out;
  QLabel* label_visionary_key;
  QTextEdit* tedit_visionary_key;
  QLabel* label_visionary_operation;
  QComboBox* cbox_visionary_operation;

public:
  Widget(QWidget* parent = nullptr);
  ~Widget();

private slots:
  void on_text_edit_atbash_in_text_changed();
  void on_text_edit_caesar_in_text_changed();
  void on_text_edit_richelieu_in_text_changed();
  void on_text_edit_gronsfeld_in_text_changed();
  void on_text_edit_visionary_in_text_changed();
};
#endif // WIDGET_H
