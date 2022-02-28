#include "vex.h"

using namespace mh8_Variables;

#define INCH_PER_M 39.3701

void mh8_Drivetrain::initGps(double xOffset, double yOffset, double rotation) {
  mh8Gps.setOrigin(xOffset, yOffset); // Set the offset from the point of rotation
  mh8Gps.resetRotation(); // Reset the preset rotation
  mh8Gps.setRotation(rotation, rotationUnits::deg); // Set the sensor's rotation
}

void mh8_Drivetrain::driveToCoord(double x, double y, double angle, double maxTurnSp, bool reversed) {  
  // Convert target x and y to mm
  x *= 1000;
  y *= 1000;
  
  double initX = mh8Gps.xPosition(mm);
  double initY = mh8Gps.yPosition(mm);

  // Turn
  
  // Calculating turn angle
  /*
   * atan2() returns the angle in radians from the origin of any given point (using rectangular coordinates)
   * The target x and y positions are subtracted from the initial x and y positions to account for the robot's origin not being the origin of the field (middle)
   */
  /*double angleRaw = atan2(y-(mh8Gps.yPosition(mm)), x-(mh8Gps.xPosition(mm))); // Gets the standard angle
  double angleBearing = 90-toDegrees(angleRaw); // Convert the angle to compass bearing

  if (angleBearing < 0)
    angleBearing += 360;
    */

  float turnAngle = atan((x - mh8Gps.xPosition(mm)) / (y - mh8Gps.yPosition(mm))) * 180 / M_PI;
  if (y - mh8Gps.yPosition(mm) < 0) {
    turnAngle = turnAngle + 180;
  }

    // Flip the angle if the robot should go backwards
  if (reversed) {
    if (turnAngle > 180) turnAngle -= 180;
    else turnAngle += 180;
  }

  if (angle > 0)
    inertialTurn(angle, maxTurnSp);
  else if (angle == -1) {
    inertialTurn(turnAngle, maxTurnSp);
  }
  
  resetDrive();
  wait(500, msec);

  //double coords[2] = {x, y}; 

  //turnWithGPSCoords(coords, angleBearing, maxTurnSp, reversed);
  //wait(100, msec);

  // Drive to the target
  double delX = Delta(initX, x);
  double delY = Delta(initY, y);

  // Determine whether we are far enough away from the target to require driving to it
  if (fabs(delX) < 0.01 && fabs(delY) < 0.01) {
    return;
  }

  double distMeters = distanceBetween(delX, delY)/1000; // Convert mm to m
  double distInches = distMeters*INCH_PER_M;
  Brain.Screen.clearScreen();
  Brain.Screen.print(mh8Gps.xPosition(mm));
  Brain.Screen.print(initY);

  char driveDir;
  if (reversed)
    driveDir = 'b';
  else
    driveDir = 'f';

  driveStraight(distInches, 85, driveDir);
}

void mh8_Drivetrain::turnWithGPSCoords(double coordsTo[2], double angle, double maxTurnSp, bool reversed) {
  //printf("%4.2f", angle);
  setBrake('h');
  resetDrive();

  // Declare variables
  const double kP = 0.40;
  const double kI = 0.000;
  const double kD = 4.0;

  double speed = 0.0;

  double x = coordsTo[0];
  double y = coordsTo[1];

  // Determine which way to turn
  char dir; // Init value
  double currHeading = mh8Gps.heading(); // Current heading - updated every iteration

  double shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;
  double prevShortestAngle = shortestAngle;
  double degTurned = 0.0; // Accumulative degrees turned
  double angleRaw = 0.0;
  double angleBearing = 0.0;

  double escapeTime = Brain.timer(msec);

  //printf("%4.2f", shortestAngle);

  if (shortestAngle < 0) { // Negative = counterclockwise (left turn)
    dir = 'l';
  } else if (shortestAngle > 0) { // Positive = clockwise (right turn)
    dir = 'r'; // Make a right turn
  } else { // Default to left turn if there is an error
    dir = 'l';
  }

  while (Brain.timer(msec) < escapeTime + 45) {
    //double maxSpeedMult = shortestAngle/180; // Maximum speed multiplier based on maximum optimized turn length (180 degrees) and the current optimized turn length

    speed = fabs(shortestAngle)*kP + (fabs(shortestAngle)-fabs(prevShortestAngle))*kD + degTurned*kI; //shortestAngle*((100)/initShortestAngle) * maxSpeedMult; // Multiplies the shortest angle by 100 divided by the initial calculated shortest angle so that the drive starts at 100 and will gradually get lower as the target is neared
    
    if (speed < 4)
      speed = 4;

    if (shortestAngle < 0) { // Negative = counterclockwise (left turn)
      driveLeft(-speed);
      driveRight(speed);
    } else if (shortestAngle > 0) { // Positive = clockwise (right turn)
      driveLeft(speed);
      driveRight(-speed);
    } else { // Default to not turn if there is an error

    }

    currHeading = mh8Gps.heading(); // Current heading - updated every iteration
    prevShortestAngle = shortestAngle;

    // Calculate the the angle of the target point relative to the center of the bot
    angleRaw = atan2(y-(mh8Gps.yPosition()/1000), x-(mh8Gps.xPosition()/1000)); // Gets the standard angle
    angleBearing = 90-toDegrees(angleRaw); // Convert the angle to compass bearing
    // Convert the angle to a bearing
    if (angleBearing < 0)
      angleBearing += 360;
    // Flip the angle if the robot should go backwards
    if (reversed) {
      if (angleBearing > 180) angleBearing -= 180;
      else angleBearing += 180;
    }
    // Set the target angle to the (now positive) bearing
    angle = angleBearing;

    //printf("%4.2f\n", angle);

    // Calculate the shortest angle to the target angle
    shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;

    degTurned+=fabs(prevShortestAngle);

    if (fabs(angleBearing - mh8Gps.heading()) > 1.8/*fabs(shortestAngle) > 0.75*/) escapeTime = Brain.timer(msec);

    wait(10, msec);
  }
  driveLeft(0);
  driveRight(0);
  resetDrive();
  return;
}

void mh8_Drivetrain::turnWithGPS(double angle, double maxTurnSp) {
  setBrake('h');
  resetDrive();

  // Declare variables
  const double kP = 0.35;
  const double kI = 0.000;
  const double kD = 3.7;
  /*const double kP = 0.25;//22;
  const double kI = 0.0;
  const double kD = 0.25;*/

  double speed = 0.0;

  // Determine which way to turn
  char dir; // Init value
  double currHeading = mh8Gps.heading(); // Current heading - updated every iteration

  double shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;
  double prevShortestAngle = shortestAngle;
  double degTurned = 0.0; // Accumulative degrees turned

  if (shortestAngle < 0) { // Negative = counterclockwise (left turn)
    dir = 'l';
  } else if (shortestAngle > 0) { // Positive = clockwise (right turn)
    dir = 'r'; // Make a right turn
  } else { // Default to left turn if there is an error
    dir = 'l';
  }

  while (fabs(shortestAngle) > 0.55) {
    //double maxSpeedMult = shortestAngle/180; // Maximum speed multiplier based on maximum optimized turn length (180 degrees) and the current optimized turn length

    speed = fabs(shortestAngle)*kP + (fabs(shortestAngle)-fabs(prevShortestAngle))*kD + degTurned*kI; //shortestAngle*((100)/initShortestAngle) * maxSpeedMult; // Multiplies the shortest angle by 100 divided by the initial calculated shortest angle so that the drive starts at 100 and will gradually get lower as the target is neared
    
    if (speed < 3)
      speed = 3;

    if (shortestAngle < 0) { // Negative = counterclockwise (left turn)
      driveLeft(-speed);
      driveRight(speed);
    } else if (shortestAngle > 0) { // Positive = clockwise (right turn)
      driveLeft(speed);
      driveRight(-speed);
    } else { // Default to not turn if there is an error

    }

    currHeading = mh8Gps.heading(); // Current heading - updated every iteration
    prevShortestAngle = shortestAngle;

    // Calculate the shortest angle to the target angle
    shortestAngle = fmod((angle-currHeading+540.0), 360.0)-180.0;

    degTurned+=fabs(prevShortestAngle);

    wait(10, msec);
  }
  driveLeft(0);
  driveRight(0);
  resetDrive();
  return;
}
