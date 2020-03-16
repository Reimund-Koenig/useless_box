#ifndef BOX_SWITCH_HPP_
#define BOX_SWITCH_HPP_

namespace box {

class Switch {

  private:
    int m_lastResult = -1;


  public:

    void addition(int, int);
    int lastResult();
};

} // namespace box

#endif // BOX_SWITCH_HPP_
