#include "vex.h"

using namespace mh8_Variables;

// Basic control
//mh8_Drivetrain::
// Voltage-based
void mh8_Drivetrain::driveLeftVolt(double vltg) {
  L1.spin(directionType::fwd, vltg, voltageUnits::mV);
  L2.spin(directionType::fwd, vltg, voltageUnits::mV);
  L3.spin(directionType::fwd, vltg, voltageUnits::mV);
}
void mh8_Drivetrain::driveRightVolt(double vltg) {
  R1.spin(directionType::fwd, vltg, voltageUnits::mV);
  R2.spin(directionType::fwd, vltg, voltageUnits::mV);
  R3.spin(directionType::fwd, vltg, voltageUnits::mV);
}

// Pct-based
void mh8_Drivetrain::driveLeft(double pct) {
  L1.spin(directionType::fwd, pct, velocityUnits::pct);
  L2.spin(directionType::fwd, pct, velocityUnits::pct);
  L3.spin(directionType::fwd, pct, velocityUnits::pct);
}
void mh8_Drivetrain::driveRight(double pct) {
  R1.spin(directionType::fwd, pct, velocityUnits::pct);
  R2.spin(directionType::fwd, pct, velocityUnits::pct);
  R3.spin(directionType::fwd, pct, velocityUnits::pct);
}

// DriveOp
// DriveOp
void mh8_Drivetrain::tankDrive(int lStick, int rStick) {
  driveLeftVolt(lStick);
  driveRightVolt(rStick);
}
void mh8_Drivetrain::arcadeDrive(int xInput, int yInput) {
  driveLeftVolt(yInput+xInput);
  driveRightVolt(yInput-xInput);
}

// Sensor-based driving
void mh8_Drivetrain::driveToObject(float maxPower, float curveTime, double sensitivity) {}

// Normal inch-based funcs
void mh8_Drivetrain::driveStraight(double inches, double maxRpm, char dir) {
  setBrake('b');
  resetDrive();
  const int wheelDiam = 4;//Should be set to the diameter of your drive wheels in inches.
  const int target = (inches / (wheelDiam * M_PI)) * 360 * 0.62;
  int lAvgTicks = 0;
  int rAvgTicks = 0;
  int avgTicks = 0;
  float currentPower = 0;
  float lPower = 0;
  float rPower = 0;
  float distErr = 0;
  float alignErr = 0;
  const float distKp = 0.35;//0.35;//Proportional constant used to control speed of robot.
  const float alignKp = 0.18;//0.18;//Proportional constant used to keep robot straight.
  const float kDecel = 3.25;//3.25;//3.45;//Constant used to determine when to decelerate.
  const float SLEW = .035;//.003;//Constant used to control acceleration in RPM/cycle.
  
  while(avgTicks < target)
  {
    lAvgTicks = abs(getAvgDriveSideDeg('l'));
    rAvgTicks = abs(getAvgDriveSideDeg('r'));

    avgTicks = (lAvgTicks + rAvgTicks) / 2;

    printf("Degrees: %4.2d\n", avgTicks);
    return;

    //Make sure we dont accelerate/decelerate too fast with slew.
    distErr = (target - avgTicks) * distKp;
    if(distErr > SLEW)
    {
      distErr = SLEW;
    }

    //Decide wether to accelerate or decelerate.
    if(currentPower * kDecel > (target - avgTicks))
    {
      distErr = distErr * -1;
      if(currentPower < 5)
      {
        distErr = 5 - currentPower;
      }
    }

    //Decide which side is too far ahead, apply alignment and speed corretions.
    alignErr = abs((lAvgTicks - rAvgTicks)) * alignKp;
    if(lAvgTicks > rAvgTicks)
    {
      lPower = (currentPower + distErr) - alignErr;
      rPower = currentPower + distErr;
    }
    else if(rAvgTicks > lAvgTicks)
    {
      rPower = (currentPower + distErr) - alignErr;
      lPower = currentPower + distErr;
    }
    else
    {
      lPower = currentPower + distErr;
      rPower = currentPower + distErr;
    }

    //Check what direction we should go, change motor velocities accordingly.
    if(dir == 'b')
    {
      lPower = lPower * -1;
      rPower = rPower * -1;
    }

    //Send velocity targets to both sides of the drivetrain.
    L1.spin(directionType::fwd, lPower, velocityUnits::rpm);
    L2.spin(directionType::fwd, lPower, velocityUnits::rpm);
    L3.spin(directionType::fwd, lPower, velocityUnits::rpm);
    R1.spin(directionType::fwd, rPower, velocityUnits::rpm);
    R2.spin(directionType::fwd, rPower, velocityUnits::rpm);
    R3.spin(directionType::fwd, rPower, velocityUnits::rpm);

    //Set current power for next cycle, make sure it doesn't get too high/low.
    /*As a side note, the distance(in ticks) at which deceleration starts is
      determined by the upper limit on currentPower's and kDecel's product.*/
    currentPower = currentPower + distErr;
    if(currentPower > maxRpm)
    {
      currentPower = maxRpm;
    }
    else if(currentPower < 35)
    {
      currentPower = 35;
    }
    
  }
  resetDrive();
  //return;
}

void mh8_Drivetrain::Turn(double deg, int maxSp, char dir) {
  double target = deg; // Put calculation here to make the robot actually go the inputted inches

  double leftTarget = target;
  double rightTarget = target;

  if (dir == 'r') { // If going right
    leftTarget = leftTarget;
    rightTarget = -rightTarget;
  } else { // If turning left
    leftTarget = -leftTarget;
    rightTarget = rightTarget;
  }

  // Start the motor rotation
  L1  .startRotateFor(leftTarget, rotationUnits::deg, maxSp, velocityUnits::pct);
  L2   .startRotateFor(leftTarget, rotationUnits::deg, maxSp, velocityUnits::pct);
  L3   .startRotateFor(leftTarget, rotationUnits::deg, maxSp, velocityUnits::pct);
  R1  .startRotateFor(rightTarget, rotationUnits::deg, maxSp, velocityUnits::pct);
  R2   .startRotateFor(rightTarget, rotationUnits::deg, maxSp, velocityUnits::pct);
  R3   .startRotateFor(rightTarget, rotationUnits::deg, maxSp, velocityUnits::pct);

  while (driving()) {} // Do nothing until the drive stops moving
}

void mh8_Drivetrain::inertialTurn(double angle, int maxTurnSp) {
  setBrake('h');

  // Determine which way to turn
  char dir; // Init value
  double currHeading = mh8Imu.heading(); // Current heading - updated every iteration

  double shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;
  double initShortestAngle = shortestAngle;

  long escapeTime = timer::system();

  if (shortestAngle < 0) { // Negative = counterclockwise (left turn)
    dir = 'l';
  } else if (shortestAngle > 0) { // Positive = clockwise (right turn)
    dir = 'r'; // Make a right turn
  } else { // Default to left turn if there is an error
    dir = 'l';
  }

  resetDrive();

  double speed = maxTurnSp; // Default speed value
  double turnKp = 0.6; // Maximum speed multiplier based on maximum optimized turn length (180 degrees) and the current optimized turn length

  while (timer::system() < escapeTime + 75) { // Exit the loop if the angle is within the margin of error and the speed is below 5 (Speed cutoff prevents overshoot)
    speed = shortestAngle * turnKp; // Multiplies the shortest angle by 100 divided by the initial calculated shortest angle so that the drive starts at 100 and will gradually get lower as the target is neared
    
    if (speed > maxTurnSp)
      speed = maxTurnSp;

    if (speed < 5)
      speed = 5;

    if (shortestAngle < 0) { // Negative = counterclockwise (left turn)
      driveLeft(-speed);
      driveRight(speed);
    } else if (shortestAngle > 0) { // Positive = clockwise (right turn)
      driveLeft(speed);
      driveRight(-speed);
    } else { // Default to not turn if there is an error

    }

    currHeading = mh8Imu.heading(); // Current heading - updated every iteration
    shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;

    if (fabs(shortestAngle) > 1.0) // If the turn over/undershot
      escapeTime = timer::system();

    printf("%4f\n" , shortestAngle);
    wait(20, msec); // Save resources
  }
  resetDrive();
  return;
}

void mh8_Drivetrain::Arc(double lInches, double rInches, double lSpeed, double rSpeed, char dir) {
  double leftTarget = (lInches / (WHEEL_SIZE * M_PI)) * 360 * 2.27; // Put calculation here to make the robot actually go the inputted inches
  double rightTarget = (rInches / (WHEEL_SIZE * M_PI)) * 360 * 2.27; // Put calculation here to make the robot actually go the inputted inches

  if (dir == 'f') {
    // If going forward, do not modify the target inches
  } else {
    leftTarget = -leftTarget;
    rightTarget = -rightTarget;
  }

  // Start the motor rotation
  L1   .startRotateFor(leftTarget, rotationUnits::deg, lSpeed, velocityUnits::rpm);
  L2   .startRotateFor(leftTarget, rotationUnits::deg, lSpeed, velocityUnits::rpm);
  L3   .startRotateFor(leftTarget, rotationUnits::deg, lSpeed, velocityUnits::rpm);
  R1   .startRotateFor(rightTarget, rotationUnits::deg, rSpeed, velocityUnits::rpm);
  R2   .startRotateFor(rightTarget, rotationUnits::deg, rSpeed, velocityUnits::rpm);
  R3   .startRotateFor(rightTarget, rotationUnits::deg, rSpeed, velocityUnits::rpm);

  while (driving()) {} // Do nothing until the drive stops moving
}

// Utility
int mh8_Drivetrain::getAvgDriveSideDeg(char side) {
  if (side == 'l') { // If returning left
    return ((L1.rotation(rotationUnits::deg) + L2.rotation(rotationUnits::deg) + L3.rotation(rotationUnits::deg))/3);
  } else { // Else, return right
    return ((R1.rotation(rotationUnits::deg) + R2.rotation(rotationUnits::deg) + R3.rotation(rotationUnits::deg))/3);
  }
}
void mh8_Drivetrain::resetDrive() {
  // Stop the motors
  L1.stop();
  L2.stop();
  L3.stop();
  R1.stop();
  R2.stop();
  R3.stop();

  // Reset encoders
  L1.resetRotation();
  L2.resetRotation();
  L3.resetRotation();
  R1.resetRotation();
  R2.resetRotation();
  R3.resetRotation();
}
void mh8_Drivetrain::setBrake(char mode) {
  brakeType m_brakeMode;
  switch(mode){
    case 'c': case 'C':
      m_brakeMode = coast;
      break;
    case 'b': case 'B':
      m_brakeMode = brake;
      break;
    case 'h': case 'H':
      m_brakeMode = hold;
      break;
  }
  L1.setBrake(m_brakeMode);
  L2.setBrake(m_brakeMode);
  L3.setBrake(m_brakeMode);
  R1.setBrake(m_brakeMode);
  R2.setBrake(m_brakeMode);
  R3.setBrake(m_brakeMode);
}

bool  mh8_Drivetrain::driving() {
  if ((L1.isSpinning() || L2.isSpinning() || L3.isSpinning()) || (R1.isSpinning() || R2.isSpinning() || R3.isSpinning()))
    return true;
  else
    return false;
}