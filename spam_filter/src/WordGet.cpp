#include "WordGet.h"

WordGet::WordGet()
{
    //ctor
    WordGet *next = nullptr;
    for(int i =0 ;i<45;i++)
    {
        word[i] = '\0';
    }
}

WordGet::~WordGet()
{
    //dtor
}
