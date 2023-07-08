#ifndef ALPHABET_H
#define ALPHABET_H

bool isUpEn(QChar symbol);
bool isLowEn(QChar symbol);
bool isUpRu(QChar symbol);
bool isLowRu(QChar symbol);
bool isEn(QChar symbol);
bool isRu(QChar symbol);

enum { 
  sizeEn = 26, sizeRu = 32
};

#endif
