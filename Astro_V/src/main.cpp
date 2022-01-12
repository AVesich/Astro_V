/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Intake               motor         3               
// Lift                 motor         19              
// Controller1          controller                    
// LiftClaw             digital_out   A               
// R1                   motor         17              
// R2                   motor         11              
// L1                   motor         15              
// L2                   motor         14              
// LAutoClick           limit         C               
// RAutoClick           limit         D               
// mh8Gps               gps           6               
// R3                   motor         13              
// L3                   motor         16              
// mh8Imu               inertial      7               
// mogoClamp            digital_out   H               
// LiftBumper           limit         B               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // Init stuff
  initIntake();
  initLift();

  // Toggle the claw when the down button is pressed
  Controller1.ButtonDown.pressed(liftClawToggle);

  // Toggle the mogo clamp when the down button is pressed
  Controller1.ButtonR1.pressed(mogoToggle);

  // Toggle the mogo clamp when the down button is pressed
  Controller1.ButtonR2.pressed(mogoToggle);

  // Reverse "front" of the robot when the b button is pressed
  Controller1.ButtonB.pressed(revDrive);

  LiftBumper.pressed(resetLiftEncoders);

  autoSelector();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  runAuto();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    
    driveOp();
    intakeOp(); // Intake operator control
		liftOp(); // Lift operator control

    // Allow running auto from the controller if not in competition
    if (!Competition.isCompetitionSwitch()) {
      if (Controller1.ButtonLeft.pressing())
        runAuto();
    }

    wait(10, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
