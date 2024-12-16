#include "timer.h"

Timer::Timer() {}

Timer::Timer(int maxTime, QLabel *display,std::function<void()> callback)
{
    this->maxTime=maxTime;
    this->currentTime=maxTime;
    this->display=display;
    this->callback=callback;
    isStop=true;
    displaying();
}

void Timer::reset()
{
    currentTime=maxTime;
    isStop=true;
    displaying();
}

void Timer::start()
{
    isStop=false;
    countDown();
}

void Timer::stop()
{
    isStop=true;
}

void Timer::countDown()
{
    if(isStop)return;
    QTimer::singleShot(1000,[this](){
        if(isStop)return;
        currentTime--;
        this->displaying();
        if(currentTime==0)
        {
            isStop=true;
            callback();
            return;
        }
        this->countDown();
    });
}

void Timer::displaying()
{
    display->setText(QString("%1:%2").arg(QString::number(currentTime/60),2,QChar('0')).arg(QString::number(currentTime%60),2,QChar('0')));
}
