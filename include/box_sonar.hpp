#ifndef BOX_SONAR_HPP_
#define BOX_SONAR_HPP_

#include <Arduino.h>

#define CM true
#define INCH false

namespace box {


class Sonar {

  private:
    int m_lastResult = -1;
    int pin_trigger;
    int pin_echo;
    int pin_interrupt;
    int max_distance;
    volatile unsigned long starttime;
    volatile unsigned long endtime;
    volatile bool finished;
    static void echo_isr();

  protected:
        Sonar(int trigger, int echo, int interrupt, int max_distance);

  public:
    virtual ~Sonar();
    virtual void start();
    virtual bool isFinished();
    virtual unsigned int getRange(bool units=CM);
};

} // namespace box

#endif // BOX_SONAR_HPP_
