#ifndef UB_MOTOR_HPP_
#define UB_MOTOR_HPP_

#define MOTOR_DIRECTION_FORWARD 0
#define MOTOR_DIRECTION_BACKWARD 1
#define MOTOR_DIRECTION_STOP 2

#define MOTOR_DEFAULT_SPEED 100
#define MOTOR_DEFAULT_DIRECTION MOTOR_DIRECTION_STOP

namespace ub {

class Motor {

  private:
    int direction;
    int speed;
    virtual void move_forward();
    virtual void move_backward();
  protected:
    Motor();
  public:
    virtual ~Motor();
    virtual void set_direction(int);
    virtual void set_speed(int);
    virtual int get_direction();
    virtual int get_speed();
    virtual void move();
};

} // namespace ub

#endif // UB_MOTOR_HPP_
