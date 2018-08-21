#include "WordData.h"
#include "stackimpl.h"
#include "WordGet.h"
#include <string.H>
#include <fstream>
#include <cstdio>
using std::fstream;
using std::ios;
int BayesianChecker(WordData w1, double TotSpamProb)
{
 //P(Spam/Word) = P(Word/Spam)*P(Spam)/(P(Word/Spam)*P(Spam) + P(Word/~Spam)*P(~Spam))
 //P(Word/Spam) = P(Word int Spam)/P(Spam) <SpamProb>
 //to reevaluate again, replace P(Spam) With the newly calculated value
 return w1.SpamProb*TotSpamProb/((w1.SpamProb*TotSpamProb)+((1-w1.SpamProb)*(1-TotSpamProb)));
}
stack<WordGet> GetWord(char* Sentence)
{
 stack<WordGet> s1;
 int count = 0;
 char* sptr = Sentence;
 for(int i = 0;Sentence[i]!='\0';i++)
  {
    if(Sentence[i] == ' ')
     {
       WordGet c1;
       strncpy(c1.word,sptr,count);
       s1.push(c1);
       sptr +=(count+1);
       count = 0;
     }
    else
     count++;
    }
 return s1;
}

int main(void)
{
   //create a file stream to store the data in a file
   //the first record of the file stream shall hold the total word and spam count
   //whenever a new word is added, the stream object shall go to the first record and modify whatever is necessary
   //whenever you train the model, the sentence specified must be broken down into words and the bayesian filter will be run on
   //each word
   //if the word already exists in the file mentioned, the stream object shall modify its word and spam count

   //declare the file stream
   fstream DataFile("worddata.dat",ios::binary|ios::in|ios::out|ios::trunc);
   WordData w1;
   if(!DataFile)
   {
       cout<<"File not found, Creating a new one\n";
       DataFile.close();
       DataFile.open("worddata.dat",ios::out);
       WordData w1;
       DataFile.write((char*)&w1,sizeof(WordData));
       DataFile.seekg(0);
       DataFile.seekp(0);
   }
   DataFile.read((char*)&w1,sizeof(WordData));
   int TotSpamProb = w1.SpamProb;
   char Sentence[140];
   cout<<"Enter a sentence: ";
   gets(Sentence);
   Sentence[strlen(Sentence)] = ' ';
   Sentence[strlen(Sentence)+1] = '\0';
   stack<WordGet> ListWords;
   ListWords = GetWord(Sentence);
   bool SpamFlag;
   cout<<"Is the sentence you've entered, a spam message?(Y or N) ";
   char SpamFlagChar;
   cin>>SpamFlagChar;
   SpamFlag = (SpamFlagChar == 'y'||SpamFlagChar == 'Y') ? true:false;
   while(ListWords.top->next != nullptr)
   {
       //update initializer after every word :/
       char word[45];
       strcpy(word,ListWords.pop().word);
       bool WordFlag = false;
       while(!DataFile.eof())
       {
           DataFile.read((char*)&w1,sizeof(w1));
           if(strcmp(word,w1.word)==0)
           {
               WordFlag = true;
               DataFile.seekp(DataFile.tellg()-sizeof(w1));
               w1.drword++;
               if(SpamFlag)
               {
                   w1.nrspam++;
               }
               w1.ProbCalc();
               DataFile.write((char*)&w1,sizeof(w1));
               break;
           }
       }
       if(WordFlag == false)
       {
           strcpy(w1.word,word);
           w1.ClearData(); //define this
           DataFile.seekp(ios::end);
           DataFile.write((char*)&w1,sizeof(w1));
       }
       TotSpamProb = BayesianChecker(w1,TotSpamProb);
   }
   cout<<"Total SpamProb = "<<TotSpamProb<<endl;
}
//to do
//define the WordData.ClearData() and WordData.ProbCalc() functions
