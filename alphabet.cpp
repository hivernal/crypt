#include <QChar>
#include "alphabet.h"

bool isUpEn(QChar symbol) { return symbol >= u'A' && symbol <= u'Z'; }
bool isLowEn(QChar symbol) { return symbol >= u'a' && symbol <= u'z'; }
bool isUpRu(QChar symbol) { return symbol >= u'А' && symbol <= u'Я'; }
bool isLowRu(QChar symbol) { return symbol >= u'а' && symbol <= u'я'; }
bool isEn(QChar symbol) { return isLowEn(symbol) && isUpEn(symbol); }
bool isRu(QChar symbol) { return isLowRu(symbol) && isUpRu(symbol); }
