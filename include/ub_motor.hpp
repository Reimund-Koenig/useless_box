#ifndef UB_MOTOR_HPP_
#define UB_MOTOR_HPP_

namespace ub {

class Motor {

  private:
    int m_lastResult = -1;


  public:

    void addition(int, int);
    int lastResult();
};

} // namespace ub

#endif // UB_MOTOR_HPP_
