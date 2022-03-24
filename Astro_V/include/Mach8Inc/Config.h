#ifndef CONFIG_H
#define CONFIG_H

using namespace vex;

namespace mh8_Variables {
  // Drivetrain motor ports
  /*const int L_FRONT = 3;
  const bool L_FRONT_REV = true;

  const int L_BACK = 4;
  const bool L_BACK_REV = true;

  const int R_FRONT = 13;
  const bool R_FRONT_REV = false;

  const int R_BACK = 14;
  const bool R_BACK_REV = false;*/

  // Drivetrain variables
  const double WHEEL_SIZE = 4.00; // Robot's wheel size
  const double TRACKING_WHEEL_SIZE = 2.75;
  const double GEAR_RATIO = 1.5; // External gear ratio compensation (gear on motor/gear on wheel)

  const double TURN_CONST = 1.5; // Turn constant

  const double FRICTION = 1; // 1 is standard, higher than 1 used for low friction, lower than 1 used for high friction

  const double TrOffset = 6.5; // Right tracking wheel offset in inches
  const double TlOffset = 6.5; // Left tracking wheel offset in inches
  const double TbOffset = 1.75; // Back tracking wheel offset in inches

  struct pid {
    double kP;
    double kI;
    double kD;
    double kA;

    pid (double p, double i, double d, double a) {
      kP = p;
      kI = i;
      kD = d;
      kA = a;
    }
  };
}

#endif