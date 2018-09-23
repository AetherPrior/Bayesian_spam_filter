#include "WordData.h"

WordData::WordData()
{
    //ctor
    SpamProb = WordProb = 0;
    word = "  ";
}

WordData::~WordData()
{
    //dtor
}
void WordData::ClearData()
{
    nrspam =0;
    nrword= 1;
}
void WordData::ProbCalc(int TotWordCount)
{
    SpamProb = nrspam/TotWordCount;
    WordProb = nrword/TotWordCount;
}
