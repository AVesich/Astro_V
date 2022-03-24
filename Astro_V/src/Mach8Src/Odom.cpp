#include "vex.h"

using namespace mh8_Variables;

double xPos = 0;
double yPos = 0;
double m_Angle = 0;

// Position tracking
int mh8_Drivetrain::trackPos() {
  double currLeft = getAvgDriveSideRev('l')*1.5*(WHEEL_SIZE*M_PI);
  double currRight = getAvgDriveSideRev('r')*1.5*(WHEEL_SIZE*M_PI);
  double currBack = mh8BackTracker.position(rev)*(TRACKING_WHEEL_SIZE*M_PI);

  double prevLeft = getAvgDriveSideRev('l')*1.5*(WHEEL_SIZE*M_PI);
  double prevRight = getAvgDriveSideRev('r')*1.5*(WHEEL_SIZE*M_PI);
  double prevBack = mh8BackTracker.position(rev)*(TRACKING_WHEEL_SIZE*M_PI);

  double deltaLeft = 0; // Prev left side movement
  double deltaRight = 0; // Prev right side movement
  double deltaBack = 0; // Prev back movement

  double arcAngle = 0; // Robot arc theta
  double xArcAngle = 0;

  int flipped = 1;

  while (true) {
    prevLeft = currLeft;
    prevRight = currRight;
    prevBack = currBack;

    currLeft = getAvgDriveSideRev('l')*GEAR_RATIO*(WHEEL_SIZE*M_PI);
    currRight = getAvgDriveSideRev('r')*GEAR_RATIO*(WHEEL_SIZE*M_PI);
    currBack = mh8BackTracker.position(rev)*(TRACKING_WHEEL_SIZE*M_PI); // No gear ratio on back tracker

    deltaLeft = currLeft-prevLeft;
    deltaRight = currRight-prevRight;
    deltaBack = currBack-prevBack;

    arcAngle = (deltaLeft-deltaRight)/(TlOffset + TrOffset);
    xArcAngle = deltaBack/TbOffset;

    if (fabs(m_Angle) > 180) flipped = -1;
    else flipped = 1;

    // if is required to prevent accumulative inf/und values for a nan result
    if (arcAngle != 0) yPos += 2*((deltaRight/arcAngle)+TrOffset)*(sin(arcAngle/2));
    if (xArcAngle != 0) xPos += ((deltaBack/xArcAngle)+TbOffset)*(sin(xArcAngle/2));

    m_Angle += arcAngle*(180/M_PI);
    if (m_Angle > 360) m_Angle -= 360; // Wrap angle
    if (m_Angle < -360) m_Angle += 360;

    printDebug();

    wait(20, msec); // Wait to save brain resources
  }

  return 0;
}

// Utility
  // Mutators
void mh8_Drivetrain::resetPos() {
  xPos = 0;
  yPos = 0;
}

void mh8_Drivetrain::setPos(double x, double y) {
  xPos = x;
  yPos = y;
}

  // Accessors
double mh8_Drivetrain::getX() {
  return xPos;
}

double mh8_Drivetrain::getY() {
  return yPos;
}

double mh8_Drivetrain::getAngle() {
  return m_Angle;
}

  // Debug
void mh8_Drivetrain::printDebug() {
  printf("x: %4.2f\t", xPos);
  printf("y: %4.2f\t", yPos);
  printf("t: %4.2f\n", m_Angle);
}