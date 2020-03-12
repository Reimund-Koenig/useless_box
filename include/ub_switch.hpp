#ifndef UB_SWITCH_HPP_
#define UB_SWITCH_HPP_

namespace ub {

class Switch {

  private:
    int m_lastResult = -1;


  public:

    void addition(int, int);
    int lastResult();
};

} // namespace ub

#endif // UB_SWITCH_HPP_
