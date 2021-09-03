#ifndef BOX_SWITCH_HPP_
#define BOX_SWITCH_HPP_

namespace box {

class Switch {

  private:
    int pin_switch;
    bool switch_state;
    bool last_switch_state;
    bool switch_state_has_changed;
    unsigned long last_debounce_time;
    unsigned long debounce_delay_ms;
    bool check();

  public:
    Switch(int pin_switch);
    virtual ~Switch();
    virtual bool has_changed();
    virtual bool is_high();
};

} // namespace box

#endif // BOX_SWITCH_HPP_
