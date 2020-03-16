#ifndef BOX_SONAR_HPP_
#define BOX_SONAR_HPP_

namespace box {

class Sonar {

  private:
    int m_lastResult = -1;


  public:

    void addition(int, int);
    int lastResult();
};

} // namespace box

#endif // BOX_SONAR_HPP_
