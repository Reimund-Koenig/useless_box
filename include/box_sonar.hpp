#ifndef BOX_SONAR_HPP_
#define BOX_SONAR_HPP_

#include <Arduino.h>

#define CM true
#define INCH false

namespace box {


class Sonar {

  private:
    int pin_trigger;
    int pin_echo;

  public:
    Sonar(int trigger, int echo);
    virtual ~Sonar();
    unsigned int  get_distance_cm();
};

} // namespace box

#endif // BOX_SONAR_HPP_
