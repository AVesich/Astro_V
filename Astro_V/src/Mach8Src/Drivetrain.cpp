#include "vex.h"

using namespace mh8_Variables;

// Basic control
//mh8_Drivetrain::
// Voltage-based
void mh8_Drivetrain::driveLeftVolt(double vltg) {
  L1.spin(fwd, vltg, voltageUnits::mV);
  L2.spin(fwd, vltg, voltageUnits::mV);
  L3.spin(fwd, vltg, voltageUnits::mV);
}
void mh8_Drivetrain::driveRightVolt(double vltg) {
  R1.spin(fwd, vltg, voltageUnits::mV);
  R2.spin(fwd, vltg, voltageUnits::mV);
  R3.spin(fwd, vltg, voltageUnits::mV);
}

// Variables for constant speed voltage PID
double speedError = 0.0;
double currActualSpeed = 0.0;
double prevSpeedError = 0.0;
double accumSpeedError = 0.0;
int counter = 0;

void mh8_Drivetrain::driveVoltPID(double targetSpeed, char driveSide) {
  const double kP = 1.5;//1.50;
  const double kI = 0.005;
  const double kD = 0.10;

  currActualSpeed = getAvgDriveSideVelo(driveSide); // Get the current actual speed for the required drive side
  prevSpeedError = speedError;
  speedError = fabs(targetSpeed)-fabs(currActualSpeed); // Get the speed difference between the target and current speed

  accumSpeedError += speedError;

  double speed = speedError*kP + accumSpeedError*kI + (speedError-prevSpeedError)*kD; // Basic P-D calculation

  if (targetSpeed < 0)
    speed *= -1;  

  switch (driveSide) {
    case 'l': driveLeftVolt(speed*120);   break;
    case 'r': driveRightVolt(speed*120);  break;
  }
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

// DriveOp modes
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

void mh8_Drivetrain::autoPark() {
  m_driveTrain.setBrake('h');
  resetDrive();

  while (mh8Imu.roll() > -24) { // Drive at full speed until going up the platform
    driveLeftVolt(90*120);
    driveRightVolt(90*120);
    wait(10, msec); // Save resources
  }

  o_driveStraight(12, 80, 'f');
  
  while (mh8Imu.roll() < -23.5) { // Drive with vex PID at 40% speed (120 rpm) until the platform is no longer fully tilted (begins balancing)
    driveLeft(15);
    driveRight(15);
    wait(10, msec); // Save resources
  }

  driveLeftVolt(0);
  driveRightVolt(0);

  wait(0.4, sec);

  o_driveStraight(3, 50, 'b');

  wait(2.0, sec);
  
  resetDrive();
}

// Normal inch-based funcs
  // Straight drive funcs
void mh8_Drivetrain::driveStraight(double inches, double maxPct, char dir) {
  // Reset the drive and make the brake mode "brake"
  setBrake('h');
  resetDrive();

  // Initialize variables
  const int wheelDiam = WHEEL_SIZE; // Diameter of the robot wheels in inches
  const int target = (inches / (wheelDiam * M_PI)) * 360 * 0.61; // Target distance converted from inches to encoder ticks; double after 360 is a constant tuned for the robot
  int lAvgTicks = 0; // Left average encoder ticks, needed for alignment
  int rAvgTicks = 0; // Right average encoder ticks, needed for alignment
  int avgTicks = 0; // Overall average encoder ticks
  float currentPower = 0; // Current power to be supplied to the motors
  float lPower = 0; // Left power for the left drive side, needed b/c of alignment
  float rPower = 0; // Right power for the rigth drive side, needed b/c of alignment
  float alignErr = 0; // Alignment error calculated to align each side of the robot

  // PID constants
  double kP = currPIDMode.kP; // Distance from target * kP
  if (inches < 12) kP *= 2; // Increase kP if driving a short distance
  const double kI = currPIDMode.kI; // Not really necessary, but implemented in case its needed // Total accumulative distance from target * kI
  const double kD = currPIDMode.kD; // Difference between current distance from target and previous distance from target * kD
  const double kA = 0.07; // Alignment constant
  double error = 0.0; // Distance from target in current iteration
  double prevError = 0.0; // Distance from target from previous iteration
  double accumulativeError = 0.0; // Total of every calculated error combined

  while(avgTicks < target)
  {
    // PID moment
    currentPower = error*kP + accumulativeError*kI + (error-prevError)*kD;

    // Limit the current power to the allowed maximum
    if (currentPower > maxPct) {
      currentPower = maxPct;
    } else if (currentPower < 5) {
      currentPower = 5;
    }

    // Align each drive side
    alignErr = fabs((lAvgTicks - rAvgTicks)) * currPIDMode.kA; // Calculate the alignment error between the two sides
    if(lAvgTicks > rAvgTicks) // If left is ahead of right
    {
      lPower = currentPower - alignErr;
      rPower = currentPower;
    }
    else if(rAvgTicks > lAvgTicks) // If right is ahead of left
    {
      rPower = currentPower - alignErr;
      lPower = currentPower;
    }
    else // If neither is ahead
    {
      lPower = currentPower;
      rPower = currentPower;
    }

    // Reverse the speeds if a backwards direction is passed to the function
    if(dir == 'b')
    {
      lPower = lPower * -1;
      rPower = rPower * -1;
    }


    // Make the motors move
    driveLeftVolt(lPower*120);//driveVoltPID(lPower, 'l');
    driveRightVolt(rPower*120);//driveVoltPID(rPower, 'r');

    // Update the average ticks for the next iteration
    rAvgTicks = fabs(getAvgDriveSideDeg('r'));
    lAvgTicks = fabs(getAvgDriveSideDeg('l'));
    avgTicks = (rAvgTicks + lAvgTicks)/2;

    // Update the PID variables
    prevError = error; // Set the previous error to the current error before it is updated
    accumulativeError += prevError; // Add the previous error to the accumuilative error; can't add the current error because that is current, not what it has already driven
    error = target - avgTicks; // Update current error
    
    // Wait to save brain resources
    wait(10, msec);
  }
  resetDrive(); // Reset the drive encoders and stop all of the motors after driving is completed
}

void mh8_Drivetrain::o_driveStraight(double inches, double maxPct, char dir) {
  // Reset the drive and make the brake mode "brake"
  setBrake('b');
  resetDrive();

  // Initialize variables
  const int wheelDiam = WHEEL_SIZE; // Diameter of the robot wheels in inches
  const int target = (inches / (wheelDiam * M_PI)) * 360 * 0.61; // Target distance converted from inches to encoder ticks; double after 360 is a constant tuned for the robot
  int lAvgTicks = 0; // Left average encoder ticks, needed for alignment
  int rAvgTicks = 0; // Right average encoder ticks, needed for alignment
  int avgTicks = 0; // Overall average encoder ticks
  float currentPower = 0; // Current power to be supplied to the motors
  float lPower = 0; // Left power for the left drive side, needed b/c of alignment
  float rPower = 0; // Right power for the rigth drive side, needed b/c of alignment
  float alignErr = 0; // Alignment error calculated to align each side of the robot

  // PID constants
  const double kP = 0.17; // Distance from target * kP
  const double kI = 0.0011; // Not really necessary, but implemented in case its needed // Total accumulative distance from target * kI
  const double kD = 0.85; // Difference between current distance from target and previous distance from target * kD
  const double kA = 0.07; // Alignment constant
  double error = 0.0; // Distance from target in current iteration
  double prevError = 0.0; // Distance from target from previous iteration
  double accumulativeError = 0.0; // Total of every calculated error combined

  while(avgTicks < target)
  {
    // PID moment
    currentPower = error*kP + accumulativeError*kI + (error-prevError)*kD;

    // Limit the current power to the allowed maximum
    if (currentPower > maxPct) {
      currentPower = maxPct;
    }

    // Align each drive side
    alignErr = fabs((lAvgTicks - rAvgTicks)) * kA; // Calculate the alignment error between the two sides
    if(lAvgTicks > rAvgTicks) // If left is ahead of right
    {
      lPower = currentPower - alignErr;
      rPower = currentPower;
    }
    else if(rAvgTicks > lAvgTicks) // If right is ahead of left
    {
      rPower = currentPower - alignErr;
      lPower = currentPower;
    }
    else // If neither is ahead
    {
      lPower = currentPower;
      rPower = currentPower;
    }

    // Reverse the speeds if a backwards direction is passed to the function
    if(dir == 'b')
    {
      lPower = lPower * -1;
      rPower = rPower * -1;
    }


    // Make the motors move
    driveLeft(lPower);
    driveRight(rPower);

    // Update the average ticks for the next iteration
    rAvgTicks = fabs(getAvgDriveSideDeg('r'));
    lAvgTicks = fabs(getAvgDriveSideDeg('l'));
    avgTicks = (rAvgTicks + lAvgTicks)/2;

    // Update the PID variables
    prevError = error; // Set the previous error to the current error before it is updated
    accumulativeError += prevError; // Add the previous error to the accumuilative error; can't add the current error because that is current, not what it has already driven
    error = target - avgTicks; // Update current error
    
    // Wait to save brain resources
    wait(10, msec);
  }
  resetDrive(); // Reset the drive encoders and stop all of the motors after driving is completed
}

  // Turning funcs
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
  resetDrive();

  // Determine which way to turn
  double currHeading = mh8Imu.heading(); // Current heading - updated every iteration

  double shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;
  double prevShortestAngle = shortestAngle;
  double totAccumAngle = 0.0;

  // PID Variables
  double kP = currTurnPIDMode.kP;

  kP *= 1 + (((180-abs(shortestAngle))/15)*0.22); // 1 + How far we start from the target/15 * .25 to get multiplier to prevent stall

  //if (fabs(shortestAngle)<90) kP *= 1.3;
  //else if (fabs(shortestAngle)<45) kP *= 1.7;

  const double kI = currTurnPIDMode.kI;//0.0017;
  const double kD = currTurnPIDMode.kD;//4.00;
  
  const double kdecel = currTurnPIDMode.kA;
  
  double targetSpeed = 0.0;
  double currentSpeed = 0.0;
  double speed = 0.0;

  double STEP = 9;
  double escapeTime = Brain.timer(msec);

  // Reset variables for speed controlled drive
  currActualSpeed = 0.0;
  speedError = 0.0;
  prevSpeedError = 0.0;
  accumSpeedError = 0.0;

  if (fabs(shortestAngle) < 1.0) { // Don't do anything if pretty much already there
    return;
  }

  while (Brain.timer(msec) < escapeTime + 60) { // Exit the loop if the angle is within the margin of error and the speed is below 5 (Speed cutoff prevents overshoot)
    //printf("%4f\n" , angle);
    targetSpeed = fabs(shortestAngle) * kP + fabs(totAccumAngle) * kI + (fabs(shortestAngle)-fabs(prevShortestAngle)) * kD; // Multiplies the shortest angle by 100 divided by the initial calculated shortest angle so that the drive starts at 100 and will gradually get lower as the target is neared
    
    if (targetSpeed > maxTurnSp)
      targetSpeed = maxTurnSp;
    // else if (targetSpeed < 3)
    //    targetSpeed = 3;

   //if(fabs(prevShortestAngle) - fabs(shortestAngle) > 0.1)
  // speed += 0.1;

    // End the loop if the angle and speed show that we are basically there so stalls dont happen
    if (fabs(shortestAngle) < 0.80 && targetSpeed < 1.0) {
       resetDrive();
       return;
    }
    
    // if (targetSpeed >= speed)
    // {
    //   STEP = 9;
    // } else 
    // {
    //   STEP = targetSpeed * kdecel;
    // }

    // Update the average speed
    //currentSpeed = (fabs(getAvgDriveSideVelo('l')) + fabs(getAvgDriveSideVelo('r')))/2;

    // Slew
    // if (targetSpeed > currentSpeed)
    //   speed += STEP;
    // if (targetSpeed < currentSpeed-STEP)
    //   speed -= STEP;


    //else if (targetSpeed < currentSpeed - STEP && acclerating)
      //speed -= STEP;
    //else
      //speed = targetSpeed;

    // Right Slew
    // if (targetSpeed > currentRight + STEP)
    //   rightSpeed += STEP;
    // else if (targetSpeed < currentRight - STEP)
    //   rightSpeed -= STEP;
    // else
    //   rightSpeed = targetSpeed;

    if (shortestAngle < 0) { // Negative = counterclockwise (left turn)
      driveVoltPID(-targetSpeed, 'l');
      driveVoltPID(targetSpeed, 'r');
      //driveLeftVolt(-speed*120);
      //driveRightVolt(speed*120);
    } else if (shortestAngle > 0) { // Positive = clockwise (right turn)
   //printf("Speed: %4.2f\n", targetSpeed);
      driveVoltPID(targetSpeed, 'l');
      driveVoltPID(-targetSpeed, 'r');
      //driveLeftVolt(speed*120);
      //driveRightVolt(-speed*120);
    } else { // Default to not turn if there is an error

    }

    currHeading = mh8Imu.heading(); // Current heading - updated every iteration

    prevShortestAngle = shortestAngle;
    shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;

    if (fabs(shortestAngle)-fabs(prevShortestAngle) > 15) // make sure that the angle doesnt change by more than 30 each iteration (to prevent accidental turn around)
      shortestAngle = prevShortestAngle;

    totAccumAngle += fabs(prevShortestAngle-shortestAngle);
    
    if (fabs(shortestAngle) > 0.65) escapeTime = Brain.timer(msec);

    wait(10, msec); // Save resources
  }
  resetDrive();
  return;
}

  // Arc
  // NOTE: (kinda odd with new bot dont use lol) ==================================================
void mh8_Drivetrain::Arc(double lInches, double rInches, double lSpeed, double rSpeed, char dir) {
  setBrake('h');
  
  double leftTarget = (lInches / (WHEEL_SIZE * M_PI)) * 360 * 0.62; // Put calculation here to make the robot actually go the inputted inches
  double rightTarget = (rInches / (WHEEL_SIZE * M_PI)) * 360 * 0.62; // Put calculation here to make the robot actually go the inputted inches

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
  // Drive motor feedback
double mh8_Drivetrain::getAvgDriveSideDeg(char side) {
  if (side == 'l') { // If returning left
    return ((L1.rotation(rotationUnits::deg) + L2.rotation(rotationUnits::deg) + L3.rotation(rotationUnits::deg))/3);
  } else { // Else, return right
    return ((R1.rotation(rotationUnits::deg) + R2.rotation(rotationUnits::deg) + R3.rotation(rotationUnits::deg))/3);
  }
}

double mh8_Drivetrain::getAvgDriveSideRev(char side) {
  if (side == 'l') { // If returning left
    return ((L1.position(rotationUnits::rev) + L2.position(rotationUnits::rev) + L3.position(rotationUnits::rev))/3);
  } else { // Else, return right
    return ((R1.position(rotationUnits::rev) + R2.position(rotationUnits::rev) + R3.position(rotationUnits::rev))/3);
  }
}

double mh8_Drivetrain::getAvgDriveSideVelo(char side) {
  if (side == 'l') { // If returning left
    return ((L1.velocity(percentUnits::pct) + (-L2.velocity(percentUnits::pct)) + L3.velocity(percentUnits::pct))/3);
  } else if (side == 'r') { // Else, return right
    return ((-R1.velocity(percentUnits::pct)) + R2.velocity(percentUnits::pct) + (-R3.velocity(percentUnits::pct))/3);
  } else {
    return 0;
  }
}

bool mh8_Drivetrain::driving() {
  if ((L1.isSpinning() || L2.isSpinning() || L3.isSpinning()) || (R1.isSpinning() || R2.isSpinning() || R3.isSpinning()))
    return true;
  else
    return false;
}

  // Drive modifiers
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