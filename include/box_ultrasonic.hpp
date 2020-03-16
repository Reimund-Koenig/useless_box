#ifndef BOX_ULTRASONIC_HPP_
#define BOX_ULTRASONIC_HPP_

namespace box {

class Ultrasonic {

  private:
    int m_lastResult = -1;


  public:

    void addition(int, int);
    int lastResult();
};

} // namespace box

#endif // BOX_ULTRASONIC_HPP_
