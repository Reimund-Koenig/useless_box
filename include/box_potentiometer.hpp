#ifndef BOX_POTENTIOMETER_HPP_
#define BOX_POTENTIOMETER_HPP_

namespace box {

class Potentiometer {

  private:
    int pin_potentiometer;
    int min_peak;
    int max_peak;

  public:
    Potentiometer(int pin_potentiometer, int min_peak, int max_peak);
    virtual ~Potentiometer();
    int get_value();
};

} // namespace box

#endif // BOX_POTENTIOMETER_HPP_
