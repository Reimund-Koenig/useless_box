#ifndef UB_ULTRASONIC_HPP_
#define UB_ULTRASONIC_HPP_

namespace ub {

class Ultrasonic {

  private:
    int m_lastResult = -1;


  public:

    void addition(int, int);
    int lastResult();
};

} // namespace ub

#endif // UB_ULTRASONIC_HPP_
