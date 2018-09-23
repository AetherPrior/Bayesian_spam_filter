#include "WordData.h"
#include "stackimpl.h"
#include "WordGet.h"
#include <string.H>
#include <fstream>
#include <cstdio>

#define TotSpamProb SpamProb
#define TotWordCount nrword
using std::fstream;
using std::ios;

double BayesianChecker(WordData w1, double TotSpamProb)
{
 //P(Spam/Word) = P(Word/Spam)*P(Spam)/(P(Word/Spam)*P(Spam) + P(Word/~Spam)*P(~Spam))
 //P(Word/Spam) = P(Word int Spam)/P(Spam) <SpamProb>
 //to reevaluate again, replace P(Spam) With the newly calculated value
 //w1.SpamProb*TotSpamProb/((w1.SpamProb*TotSpamProb)+((1-w1.SpamProb)*(1-TotSpamProb)))
 double pwordspam = w1.SpamProb;
 double pspam = TotSpamProb;
 double pwordnotspam = 1-w1.SpamProb;
 double pnotspam = 1-TotSpamProb;
 double nrprob = pwordspam*pspam;
 double drprob = nrprob+(pwordnotspam*pnotspam);
 double prob= nrprob/drprob;
 return prob;
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
   //the first record of the file stream shall hold the total word and Spam count
   //whenever a new word is added, the stream object shall go to the first record and modify whatever is necessary
   //whenever you train the model, the sentence specified must be broken down into words and the Bayesian filter will be run on
   //each word
   //if the word already exists in the file mentioned, the stream object shall modify its word and Spam count

   //declare the file stream
   fstream DataFile("worddata.dat",ios::binary|ios::in|ios::out);
   WordData w1;



   //************************************First run*****************************88888
   if(!DataFile)
   {
       cout<<"File not found, Creating a new one\n";
       DataFile.close();
       DataFile.open("worddata.dat",ios::binary|ios::out);
       WordData w1;
       w1.SpamProb = 0.5;
       DataFile.write((char*)&w1,sizeof(w1));
       DataFile.close();
       DataFile.open("worddata.dat",ios::binary|ios::in|ios::out);
   }



   //*******************************read initial data*********************************************888
   DataFile.read((char*)&w1,sizeof(WordData));
   WordData TotStuffs;
   TotStuffs.TotSpamProb = w1.SpamProb;
   TotStuffs.TotWordCount = w1.nrword;




   //*******************************input**********************************8
   char Sentence[140];
   cout<<"Enter a sentence: ";
   gets(Sentence);
   Sentence[strlen(Sentence)] = ' ';
   Sentence[strlen(Sentence)+1] = '\0';
   stack<WordGet> ListWords;
   ListWords = GetWord(Sentence);




   //******************************ask if Spam***************************
   bool SpamFlag;
   cout<<"Is the sentence you've entered, a Spam message?(Y or N) ";
   char SpamFlagChar;
   cin>>SpamFlagChar;
   SpamFlag = (SpamFlagChar == 'y'||SpamFlagChar == 'Y') ? true:false;





   //******************************use stack****************************88
   double SessionSpamProb = TotStuffs.TotSpamProb; //Create a variable that calculates the Spam prob for the session
   while(ListWords.top != nullptr)
   {

       char MainWord[10];
       strcpy(MainWord,ListWords.pop().word);               //get the word from the stack
       bool WordFlag = false;                               //check if word is found
       while(!DataFile.eof())
       {
           DataFile.read((char*)&w1,sizeof(w1));            //read the data from the file
           if(strncmp(MainWord,w1.word,strlen(MainWord))==0)
           {
               WordFlag = true;                             //word is found, i.e. word exists
               int index = sizeof(w1);
               index = 0-index;
               DataFile.seekp(index,DataFile.cur); //overwrite data with new stuff
               w1.nrword++;                                 //found one new occurrence of the word
               TotStuffs.TotWordCount++;                        //increment the total word count*/
               if(SpamFlag)
               {
                   w1.nrspam++;                             //increment Spam count if word is Spam
                   TotStuffs.nrspam++;
               }
               TotStuffs.ProbCalc(TotStuffs.TotWordCount);
  /*             fstream InitData("worddata.dat",ios::binary|ios::in|ios::out);
               InitData.write((char*)&TotStuffs,sizeof(TotStuffs));
               InitData.close();*/
               w1.ProbCalc(TotStuffs.TotWordCount);
               DataFile.write((char*)&w1,sizeof(w1));       //now, rewrite the data
               break;                                       //get out of the loop
           }
       }
       if(WordFlag == false)                                //if word is not found
       {
           strcpy(w1.word,MainWord);                            //copy the word to the object
           w1.ClearData();                                  //clears all the preexisting probabilities
           DataFile.seekp(ios::end);                        //go to end of file
           if(SpamFlag)
           {
               w1.nrspam++;
               TotStuffs.nrspam++;
           }
           TotStuffs.TotWordCount++;                        //increment the total word count*/
           w1.ProbCalc(TotStuffs.TotWordCount);
           TotStuffs.ProbCalc(TotStuffs.TotWordCount);
           /*    fstream InitData("worddata.dat",ios::binary|ios::in|ios::out);
               InitData.write((char*)&TotStuffs,sizeof(TotStuffs));
               InitData.close();
*/
           DataFile.write((char*)&w1,sizeof(w1));           //write the data
           /*int index = DataFile.tellg();
           index = sizeof(w1);
           index = -index;
           DataFile.seekg(index,DataFile.cur);
           index = DataFile.tellg();
           WordData temp;
           DataFile.read((char*)&temp,sizeof(temp));
           cout<<"";*/
       }
       SessionSpamProb = BayesianChecker(w1,SessionSpamProb); //update the Spam probabilities
   }
   cout<<"Total SpamProb = "<<SessionSpamProb<<endl;
   //create a new object to store the total word count;
   WordData *t1 = new WordData();
   t1->nrword = TotStuffs.TotWordCount;
   t1->SpamProb = TotStuffs.TotSpamProb;
   DataFile.seekp(ios::beg);                         //modify the initial data
   DataFile.write((char*)t1,sizeof(WordData));       //overwrite it
   delete t1;
   DataFile.close();
}

