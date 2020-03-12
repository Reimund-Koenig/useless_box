#ifndef UB_MAIN_HPP_
#define UB_MAIN_HPP_

namespace ub {

class Main {

  private:
    int m_lastResult = -1;

  protected:
  public:
    Main();
    virtual ~Main();
    virtual void blink();
    virtual void addition(int, int);
    virtual int lastResult();
};

} // namespace ub

#endif // UB_MAIN_HPP_
