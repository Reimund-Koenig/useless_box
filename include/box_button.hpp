#ifndef BOX_BUTTON_HPP_
#define BOX_BUTTON_HPP_

namespace box {

class Button {

  private:
    int pin_button;
    int last_button_state;
    int button_state;
    unsigned long last_debounce_time;
    unsigned long debounce_delay;

  protected:
    Button(int pin_button, unsigned long debounce_delay);
  public:
    ~Button();
    bool get_state();
};

} // namespace box

#endif // BOX_BUTTON_HPP_
