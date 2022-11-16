// https://www.vexforum.com/t/vexcode-pid-tutorial/73706
#include "vex.h"

using namespace vex;

class PIDController {
  public:
    double previous_error = 0;
    double integral = 0;
    double Kp;
    double Ki;
    double Kd;
    double output;
    int dt = 20;
    PIDController(double KpInput, double KiInput, double KdInput) {
      Kp = KpInput;
      Ki = KiInput;
      Kd = KdInput;
    }
  
  double pid_velocity(double setpoint, double input) {
    double error = setpoint - input;
    integral = integral + error * dt;
    double derivative = (error - previous_error) / dt;
    output = Kp * error + Ki * integral + Kd * derivative;
    previous_error = error;
    return output;
  }
};