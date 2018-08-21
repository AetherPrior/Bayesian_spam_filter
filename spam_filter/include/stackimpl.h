#include<iostream>
#include "stack.h"
using std::cout;
using std::cin;
using std::endl;
template <typename T>
stack<T>::stack()
{
    //ctor
    top = nullptr;
}
template <typename T>
stack<T>::~stack()
{
    //dtor
  /* while(top->next != nullptr)
    {
        pop(); //Dissolve the stack
    }*/
}
template <typename T>
void stack<T>::push(T c1)
 {
   T *temp = new T(c1);
   temp->next = top;
   top = temp;
 }
template <typename T>
T stack<T>::pop()
 {
   if(top == nullptr)
   {
       cout<<"Underflow\n"<<endl;
       return *top;
   }
   T temp = *top;
   T *t2 = top;
   top = top->next;
   delete t2;
   return temp;
  }
