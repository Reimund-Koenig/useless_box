#ifndef BOX_SWITCH_HPP_
#define BOX_SWITCH_HPP_

namespace box {

class Switch {

  private:
    int pin_switch;

  protected:
    Switch(int pin_switch);
  public:
    ~Switch();
    int get_state();
};

} // namespace box

#endif // BOX_SWITCH_HPP_
