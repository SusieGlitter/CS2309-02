#ifndef TIMER_H
#define TIMER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <functional>
class Timer
{
public:
    Timer();
    Timer(int maxTime,QLabel *display,std::function<void()> callback);
    QLabel *display;
    int maxTime;
    int currentTime;
    void reset();
    void start();
    void stop();
    void countDown();
    bool isStop;
    std::function<void()> callback;
    void displaying();
};

#endif // TIMER_H
