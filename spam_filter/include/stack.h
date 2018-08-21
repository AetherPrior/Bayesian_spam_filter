#ifndef STACK_H
#define STACK_H
template <class T>
class stack
{
    public:
        stack();
        T *top;
        virtual ~stack();
        void push(T);
        T pop();
    protected:

    private:
};

#endif // STACK_H
