#ifndef BOX_BUTTON_HPP_
#define BOX_BUTTON_HPP_

namespace box {

class Button {

  private:
    int pin_button;
    int last_button_state;
    int button_state;
    bool last_state_pressed;
    unsigned long last_debounce_time;
    unsigned long debounce_delay;
    bool get_state();

  public:
    Button(int pin_button, unsigned long debounce_delay);
    ~Button();
    bool pressed();
};

} // namespace box

#endif // BOX_BUTTON_HPP_
