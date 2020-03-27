#ifndef BOX_SWITCH_HPP_
#define BOX_SWITCH_HPP_

namespace box {

class Switch {

  private:
    int pin_switch;
    bool last_state;
    bool m_has_changed;
    bool check();

  public:
    Switch(int pin_switch);
    virtual ~Switch();
    virtual bool is_high();
    virtual bool has_changed();
};

} // namespace box

#endif // BOX_SWITCH_HPP_
