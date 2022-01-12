#include "vex.h"

/*
  // TURNS:
  // 525 deg input -> 90 deg turn
*/

// TASKS
task liftTask;
task mogoLiftTask;
task intakeTask;

void leftTip() {
  //flipoutLift();
  //m_driveTrain.inertialTurn(10, 100);
  m_driveTrain.Turn(28, 85, 'r');
  m_driveTrain.driveStraight(46.5, 200, 'f');
  liftClawAuto(true);
  m_driveTrain.Turn(13, 85, 'r');
  m_driveTrain.driveStraight(38, 180, 'b');
  liftClawAuto(false);
  liftAuto(20, 'u');
  m_driveTrain.driveStraight(14, 180, 'b');
  wait(100, msec);
  m_driveTrain.Turn(220, 85, 'l');
  wait(100, msec);
  //mogoAuto(580, 'd');
  m_driveTrain.driveStraight(24, 180, 'b');
  //mogoAuto(580, 'u');
  liftAuto(100, 'u');
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(14, 180, 'f');
  wait(1, sec);
  intakeAsync(INTAKE_OFF);
  liftAuto(100, 'd');
}
void sauce() {
  m_driveTrain.Turn(70, 100, 'r');

  liftTarget = 0;
  mogoLiftTarget = 0;
  // Start lift tasks
  mogoLiftTask = task (mogoLiftAsync);
  liftTask = task (liftAsync);
  intakeTask = task (intakeSkills);

  liftTarget = 0;
  mogoLiftTarget = -580;

  m_driveTrain.driveStraight(46, 550, 'f');

  mogoLiftTarget = 0;

  wait(350, msec);
  m_driveTrain.Turn(90, 100, 'l');
  
  wait(100, msec);

  m_driveTrain.Arc(67, 35, 355, 180, 'b');
  wait(150, msec);
  m_driveTrain.driveStraight(12, 250, 'b');
  liftClawAuto(true);

  wait(250, msec);

  liftTarget = 350;
  m_driveTrain.driveStraight(20, 230, 'f');
  wait(150, msec);
  liftTarget = 100;
  m_driveTrain.Turn(450, 100, 'r');
  
  liftClawAuto(false);
  liftTarget = 45;

  wait(150, msec);

  m_driveTrain.driveStraight(12, 300, 'f');

  m_driveTrain.Arc(43, 59, 210, 320, 'f');

  liftTarget = 15;

  m_driveTrain.Turn(500, 100, 'r');

  m_driveTrain.driveStraight(12, 500, 'b');
  liftClawAuto(true);
  wait(250, msec);
  m_driveTrain.driveStraight(12, 500, 'f');

  intakeTask.stop();
  liftTask.stop();
  mogoLiftTask.stop();
}

void rightTip() {
  m_driveTrain.driveStraight(44, 550, 'b');
  liftClawAuto(true);
  m_driveTrain.driveStraight(24, 550, 'f');
  liftClawAuto(false);
  m_driveTrain.driveStraight(4, 550, 'f');
  m_driveTrain.Turn(570, 100, 'l');
  //mogoAuto(580, 'd');
  m_driveTrain.driveStraight(12, 550, 'f');
  //mogoAuto(580, 'u');
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(28, 550, 'b');
  intakeAsync(INTAKE_OFF);
  liftClawAuto(false);
  m_driveTrain.driveStraight(12, 550, 'f');
  m_driveTrain.Turn(210, 100, 'r');
  m_driveTrain.driveStraight(36, 550, 'b');
  liftClawAuto(true);
  m_driveTrain.driveStraight(40, 550, 'f');
  liftClawAuto(false);
}
void rightClaw() { 
  //m_driveTrain.driveToCoord(0.0, 0.0, 0, 500, true);
  //m_driveTrain.inertialTurn(10, 80);
  m_driveTrain.driveStraight(6, 200, 'b');
}

void winPoint() { // TEMP SKILLS
  // Prep for and start task
  liftTarget = 0;
  liftTask = task (liftAsync);

  mogoAuto(false); // Make sure the mogo lift is open
  m_driveTrain.driveStraight(11, 200, 'b'); // Drive forward to the goal
  mogoAuto(true); // Grab the left alliance mogo
  //liftTarget = 100; // Move the lift up to accomodate for the intake
  m_driveTrain.driveStraight(11, 400, 'f'); // Drive forward to the goal
  m_driveTrain.Turn(179, 100, 'l'); // face the middle of the field
  //m_driveTrain.driveStraight(36, 500, 'f'); // Drive to the middle of the field
  //intakeAuto(600, 'u', 500);

  // End tasks
  liftTask.stop();
}

void skills() {
  liftTarget = 0;
  mogoLiftTarget = 0;
  intakeTask = task (intakeSkills);
  liftTask = task (liftAsync); // Start lift task
  liftTarget = 0;
  //liftAsync(LIFT_FLIPOUT);
  //mogoAuto(580, 'd');
  m_driveTrain.driveStraight(9, 500, 'f');
  //mogoAuto(580, 'u');
  mogoLiftTask = task (mogoLiftAsync); // Start mogo lift task
  m_driveTrain.Turn(50, 100, 'r');
  m_driveTrain.driveStraight(9, 500, 'b');
  m_driveTrain.Turn(555, 100, 'l');
  m_driveTrain.driveStraight(24, 500, 'f');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.33, -1.02, 294.5, 500, true);
  //m_driveTrain.turnWithGPS(273, 300);
  liftClawAuto(true);
  wait(250, msec);
  liftTarget = 550;
  wait(250, msec);
  m_driveTrain.driveToCoord(-0.85, -0.75, 291, 500, true);
  m_driveTrain.driveToCoord(-0.85, -0.1, 350, 500, true);
  m_driveTrain.driveToCoord(-0.95, -0.1, 283, 500, true);
  liftTarget = 350;
  wait(250, msec);
  liftClawAuto(false);
  m_driveTrain.driveStraight(1.5, 500, 'f');
  liftTarget = 550;
  wait(100, msec);
  m_driveTrain.turnWithGPS(182, 500);//driveToCoord(-0.96, -0.1, 175, 500, true);
  liftTarget = 0;
  mogoLiftTarget = -590;
  wait(250, msec);
  m_driveTrain.driveToCoord(-0.98, -1.21, 182, 500, true);
  liftClawAuto(true);
  wait(100, msec);
  m_driveTrain.driveToCoord(-0.98, -0.86, 180, 500, false);
  liftTarget = 550;
  m_driveTrain.driveToCoord(1.12, 1.20, 214, 500, false);
  m_driveTrain.driveStraight(17.5, 500, 'b');
  m_driveTrain.turnWithGPS(182, 500);
  m_driveTrain.driveStraight(26, 500, 'b');
  mogoLiftTarget = 0;
  m_driveTrain.driveToCoord(0.87, 0.0, 110, 500, true);
  liftTarget = 350;
  wait(250, msec);
  liftClawAuto(false);
  m_driveTrain.driveStraight(1.5, 500, 'f');
  liftTarget = 550;
  m_driveTrain.driveStraight(3, 500, 'f');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.97, 1.22, 20, 500, true);
  liftClawAuto(true);
  wait(250, msec);
  liftTarget = 550;
  m_driveTrain.driveStraight(17.5, 500, 'b');
  m_driveTrain.driveToCoord(-0.97, 0.36, 255, 500, true);
  liftClawAuto(false);
  m_driveTrain.driveStraight(12, 500, 'f');

  liftTask.stop();
  mogoLiftTask.stop();
  intakeTask.stop();
}

void runAuto() {
  //Brain.Screen.printAt(24, 128, "Running Auto");
  switch(currAuto){
    case 0: leftTip(); break;
    case 1: sauce(); break;
    case 2: rightTip(); break;
    case 3: rightClaw(); break;
    case 4: winPoint(); break;
    case 5: skills(); break;
  }
}