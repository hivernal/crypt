#ifndef MACROS_H
#define MACROS_H

#define IS_UP_EN(symbol) ((symbol) >= u'A' && (symbol) <= u'Z')
#define IS_LOW_EN(symbol) ((symbol) >= u'a' && (symbol) <= u'z')
#define IS_UP_RU(symbol) ((symbol) >= u'А' && (symbol) <= u'Я')
#define IS_LOW_RU(symbol) ((symbol) >= u'а' && (symbol) <= u'я')
#define IS_EN(symbol) (IS_LOW_EN(symbol) && IS_UP_EN(symbol))
#define IS_RU(symbol) (IS_LOW_RU(symbol) && IS_UP_RU(symbol))

#define EN 26
#define RU 32

#endif
