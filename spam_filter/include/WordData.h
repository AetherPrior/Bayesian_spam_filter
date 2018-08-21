#ifndef WORDDATA_H
#define WORDDATA_H


class WordData
{
    public:
        WordData();
        virtual ~WordData();
        int nrspam,nrword;
        double SpamProb,WordProb;
        char word[45];
        void ClearData();
        void ProbCalc();
    protected:

    private:
}; //to store the spam count to get the total number of words stored

#endif // WORDDATA_H
