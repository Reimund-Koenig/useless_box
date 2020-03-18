#ifndef Servo_h
#define Servo_h

#include <inttypes.h>
#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds
#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)
#define INVALID_SERVO         255     // flag indicating an invalid servo index

class Servo {
  public:
    Servo();
    uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
    void detach();
    void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds
    void writeMicroseconds(int value); // Write pulse width in microseconds
    int read();                        // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
    bool attached();                   // return true if this servo is attached, otherwise false
  private:
    uint8_t servoIndex;               // index into the channel data for this servo
    int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH
    int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH
};

#endif //Servo_h
