#include "vex.h"

// Used for drive toggle
bool driveReversed = false;

double x;
double y;

bool tryingToPark = false;

void revDrive() { driveReversed = !driveReversed; }

void driveOp() {
  // Store the x and y joystick values to their respective variable
  x = Controller1.Axis4.value();
  y = Controller1.Axis3.value();

  // Prevent drift
  if (fabs(x) < 0.15)
    x = 0;
  if (fabs(y) < 0.15)
    y = 0;
  
  // Flip the y input if necessary
  if (driveReversed)
    y = -y;

    // Convert controller values to volts
  x *= 120; // multiplies by 12, the proportion of max mvolts to max axis value
  y *= 120; // multiplies by 12, the proportion of max mvolts to max axis value

  if (!tryingToPark && mh8Imu.roll() < -20)
    tryingToPark = true;

  if (tryingToPark)
    m_driveTrain.setBrake('h');
  else
    m_driveTrain.setBrake('c');

  // Make the drivetrain move
  m_driveTrain.arcadeDrive(x, y);
}