#ifndef CRYPT_H
#define CRYPT_H

#define IS_UP_EN(symbol) ((symbol) >= u'A' && (symbol) <= u'Z')
#define IS_LOW_EN(symbol) ((symbol) >= u'a' && (symbol) <= u'z')
#define IS_UP_RU(symbol) ((symbol) >= u'А' && (symbol) <= u'Я')
#define IS_LOW_RU(symbol) ((symbol) >= u'а' && (symbol) <= u'я')
#define IS_EN(symbol) (IS_LOW_EN(symbol) && IS_UP_EN(symbol))
#define IS_RU(symbol) (IS_LOW_RU(symbol) && IS_UP_RU(symbol))

#define UP_EN 155
#define LOW_EN 219
#define UP_RU 2111
#define LOW_RU 2175

#define EN 26
#define RU 32

QString atbash(QString text);
QString caesar(QString text, int step, bool is_decrypt);
int richelieu(QString& text, QString key, bool is_decrypt);
int gronsfeld(QString& text, QString key, bool is_decrypt);
int visionary(QString& text, QString key, bool is_decrypt);
int playfair(QString& text, QString key, bool is_decrypt);


#endif
