#ifndef BOX_SONAR_HPP_
#define BOX_SONAR_HPP_

#define CM true
#define INCH false
#define NUMBER_OF_MEDIAN_VALUES 300
#define NUMBER_OF_AVERAGE_VALUES 8

namespace box {

class Sonar {

  private:
    int pin_trigger;
    int pin_echo;
    int average_array[NUMBER_OF_AVERAGE_VALUES];
    int median_array[NUMBER_OF_MEDIAN_VALUES];
    bool average_not_the_first_iteration;
    int average_iter;
    int median_iter;
    unsigned int average_summary;
    unsigned int average_distance_cm;
    unsigned int  calculate_and_get_median();
    void bubble_sort(int a[], int size);
    void calculate_average(int distance_in_cm);
    void add_to_median_array(int distance_in_cm);


  public:
    Sonar(int trigger, int echo);
    virtual ~Sonar();
    virtual unsigned int  get_distance_cm();
    virtual unsigned int  get_average_distance_cm();
    virtual unsigned int  get_median_distance_cm();
};

} // namespace box

#endif // BOX_SONAR_HPP_
