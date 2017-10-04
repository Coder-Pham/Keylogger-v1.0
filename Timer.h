#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>   //For very exact value of time

//Create a header where placed all external function
class Timer
{
    std::thread Thread;
    bool Alive = false; //check the prog is running
    long CallNumber = -1L;  //count how many times we call function
    long repeat_count = -1L;    //-1 is a default value & L is stand for long

    std::chrono::milliseconds interval = std::chrono::milliseconds(0);  //assign value in milisec =0 to interval
    std::function<void(void)> funct = nullptr;   //appear in C++11
    //<void(void)> is take nothing and return nothing, not point function to nothing

    void SleepAndRun()
    {
        std::this_thread::sleep_for(interval);  //pause a block for a certain time
        if (Alive)
                Function()();   //first (): take this function and then return to second () function
    }

    void ThreadFunc()   //function will be passed to thread
    {
        if (CallNumber == Infinite)
            while(Alive)
                SleepAndRun();
        else
            while(repeat_count--)
                SleepAndRun();
    }

public:
    static const long Infinite = -1L;

    //Constructer
    Timer(){}

    Timer(const std::function<void(void)> &f) : funct(f) {}

    Timer(const std::function<void(void)> &f,
          const unsigned long &i,
          const long repeat = Timer::Infinite): funct(f),
                                                interval(std::chrono::milliseconds(i)),
                                                CallNumber(repeat) {}

    void Start(bool Async = true)
    {
        if(IsAlive())
            return;
        Alive = true;
        repeat_count = CallNumber;
        if(Async)
            Thread = std::thread(ThreadFunc, this);
            //Assigned Thread to a pointer to the function ThreadFunc
        else
            this->ThreadFunc();
    }

    void SetFunction(const std::function <void(void)> &f)
    {
        funct = f;
    }


    void Stop()
    {
        Alive = false;
        Thread.join();
    }

    bool IsAlive() const {return Alive;}

    //Send the number of repeated
    void RepeatCount(const long r)
    {
        if (Alive)
            return;
        CallNumber = r;
    }

    //Get how many iteration are left
    long GetLeftCount() const {return repeat_count;}

    //Get the total of repeated we have set
    long RepeatCount() const {return CallNumber;}

    void SetInterval(const unsigned long &i)
    {
        //Check the prog is running ?
        if (Alive)
            return;
        interval = std::chrono::milliseconds(i);
    }

    //Get the actual Number from interval which can get to log file
    unsigned long Interval() const {return interval.count();}

    const std::function <void(void)> &Function() const
    {
        return funct;   //return a FUNCTION -> call and execute
    }
};
#endif // TIMER_H
