#include "vex.h"

mh8_Drivetrain m_driveTrain; // Create a drivetrain object

// Auto task variables
double liftTarget;
double mogoLiftTarget;
double intakeEnabled;

// PID modes
struct pid currPIDMode = {0, 0, 0, 0};
struct pid currTurnPIDMode = {0, 0, 0, 0};