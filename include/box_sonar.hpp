#ifndef BOX_SONAR_HPP_
#define BOX_SONAR_HPP_

#define CM true
#define INCH false
#define NUMBER_OF_AVERAGE_VALUES 100

namespace box {

class Sonar {

  private:
    int pin_trigger;
    int pin_echo;
    int average_array[NUMBER_OF_AVERAGE_VALUES];
    bool average_not_the_first_iteration;
    int average_iter;
    unsigned int average_summary;
    unsigned int average_distance_cm;
    void calculate_average(int distance_in_cm);


  public:
    Sonar(int trigger, int echo);
    virtual ~Sonar();
    unsigned int  get_distance_cm();
    unsigned int  get_average_distance_cm();
};

} // namespace box

#endif // BOX_SONAR_HPP_
