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
  double pullTime = Brain.timer(sec);
  m_driveTrain.Turn(28, 100, 'r');
  m_driveTrain.driveStraight(45.5, 100, 'f');
  liftClawAuto(true);
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);
  m_driveTrain.driveStraight(38, 90, 'b');
  liftClawAuto(false);
  wait(100, msec);
  m_driveTrain.Turn(25, 85, 'r');
  wait(100, msec);
  m_driveTrain.driveStraight(14, 90, 'b');
  wait(100, msec);
  m_driveTrain.Turn(220, 85, 'l');
  wait(100, msec);
  m_driveTrain.driveStraight(24, 90, 'b');
  mogoAuto(true);
  wait(100, msec);
  liftAuto(100, 'u');
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(13, 40, 'f');
  wait(1, sec);
  intakeAsync(INTAKE_OFF);
  liftAuto(100, 'd');
}

void sauce() {
  double pullTime = Brain.timer(sec);
  m_driveTrain.Turn(28, 100, 'r');

  // Start lift tasks
  liftTask = task (liftAsync);
  intakeTask = task (intakeSkills);

  liftTarget = 0;

  m_driveTrain.driveStraight(45.5, 100, 'f');
  liftClawAuto(true);
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);
  liftTarget = 50;
  m_driveTrain.Turn(125, 100, 'l');
  wait(150, msec);
  m_driveTrain.driveStraight(12, 250, 'b');
  wait(150, msec);
  m_driveTrain.Turn(125, 100, 'l');
  liftClawAuto(false);

  /*wait(250, msec);

  liftTarget = 350;
  m_driveTrain.driveStraight(20, 230, 'f');
  wait(150, msec);
  liftTarget = 100;
  m_driveTrain.Turn(150, 100, 'r');
  
  liftClawAuto(false);
  liftTarget = 45;

  wait(150, msec);

  m_driveTrain.driveStraight(12, 300, 'f');

  m_driveTrain.Arc(43, 59, 70, 107, 'f');

  liftTarget = 15;

  m_driveTrain.Turn(500, 100, 'r');

  m_driveTrain.driveStraight(12, 500, 'b');
  liftClawAuto(true);
  wait(250, msec);
  m_driveTrain.driveStraight(12, 500, 'f');
*/
  intakeTask.stop();
  liftTask.stop();
}

void rightTip() {
  double pullTime = Brain.timer(sec);
  m_driveTrain.driveStraight(42, 100, 'f');
  liftClawAuto(true);
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);
  m_driveTrain.driveStraight(26, 100, 'b');
  liftAuto(150, 'u');
  m_driveTrain.Turn(185, 60, 'l');
  wait(200, msec);
  m_driveTrain.driveStraight(14, 100, 'b');
  mogoAuto(true);
  wait(200, msec);
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(20, 80, 'f');
  intakeAsync(INTAKE_OFF);
  mogoAuto(false);
  m_driveTrain.driveStraight(13, 80, 'f');
  liftAuto(400, 'u');
  m_driveTrain.Turn(205, 60, 'l');
  wait(200, msec);
  m_driveTrain.driveStraight(16, 50, 'f');
  liftClawAuto(false);
  m_driveTrain.Turn(25, 100, 'r');
  wait(200, msec);
  m_driveTrain.driveStraight(43, 75, 'b');
  mogoAuto(true);
  m_driveTrain.driveStraight(24, 100, 'f');
  wait(200, msec);
  m_driveTrain.Turn(100, 100, 'r');
  mogoAuto(false);
}
void rightClaw() { 
  // Calibrate the gps
  //mh8Gps.calibrate();
  //while (mh8Gps.isCalibrating()) { task::sleep(50); }

  //m_driveTrain.driveToCoord(0.0, 0.0, 0, 100, false);
  //m_driveTrain.inertialTurn(10, 80);
  //m_driveTrain.o_driveStraight(24, 200, 'f');
  //m_driveTrain.turnWithGPS(2.0, 100);
  //m_driveTrain.autoPark();
  double pullTime = Brain.timer(sec);
  m_driveTrain.driveStraight(42, 100, 'f');
  liftClawAuto(true);
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);
  m_driveTrain.driveStraight(26, 100, 'b');
  liftAuto(150, 'u');
  m_driveTrain.Turn(185, 60, 'l');
  wait(200, msec);
  m_driveTrain.driveStraight(14, 100, 'b');
  mogoAuto(true);
  wait(200, msec);
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(20, 80, 'f');
  intakeAsync(INTAKE_OFF);
}

void winPoint() {
  // Prep for and start task
  liftTarget = 0;
  liftTask = task (liftAsync);

  liftClawAuto(true); // Make sure the claw isnt going to hit the wall
  mogoAuto(false); // Make sure the mogo lift is open
  m_driveTrain.driveStraight(16, 100, 'b'); // Drive forward to the goal
  mogoAuto(true); // Grab the left alliance mogo
  wait(200, msec);
  m_driveTrain.Turn(20, 100, 'r');
  wait(200, msec);
  m_driveTrain.driveStraight(16, 100, 'f'); // Drive back from the goal
  wait(200, msec);
  m_driveTrain.Turn(200, 100, 'l'); // face the middle of the field
  wait(200, msec);
  liftTarget = 100; // Move the lift up to accomodate for the intake
  m_driveTrain.driveStraight(32, 100, 'b'); // Drive to the middle of the field
  liftClawAuto(false); // Open the claw
  intakeAuto(600, 'u', 500);
  
  wait(200, msec);
  m_driveTrain.Turn(195, 100, 'l'); // face the opposing alliance goal
  wait(300, msec);
  m_driveTrain.driveStraight(44, 100, 'f'); // Drive to the middle of the field
  mogoAuto(false); // Drop the mogo
  wait(200, msec);
  m_driveTrain.driveStraight(44, 100, 'f'); // Drive to other mogo
  wait(200, msec);
  m_driveTrain.Turn(285, 100, 'l'); // face the opposing alliance goal
  wait(200, msec);
  m_driveTrain.driveStraight(20, 60, 'b'); // Drive forward to the goal
  mogoAuto(true); // Grab the Right alliance mogo
  wait(200, msec);
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(20, 100, 'f'); // Back up
  intakeAsync(INTAKE_OFF);

  liftTarget = 0; // Move the lift back down
  wait(1000, msec);

  // End tasks
  liftTask.stop();
}

void skills() {
  // Calibrate the gps
  mh8Gps.calibrate();
  while (mh8Gps.isCalibrating()) { task::sleep(50); }

  liftTarget = 0;
  intakeTask = task (intakeSkills);
  liftTask = task (liftAsync); // Start lift task
  liftTarget = 0;
  mogoAuto(false);
  m_driveTrain.driveStraight(16, 100, 'b');
  mogoAuto(true);
  m_driveTrain.Turn(24, 100, 'r');
  m_driveTrain.driveStraight(9, 100, 'f');
  m_driveTrain.Turn(165, 100, 'l');
  m_driveTrain.driveStraight(24, 100, 'b');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.40, -0.89, 294.5, 100, false);
  /*liftClawAuto(true);
  wait(250, msec);
  liftTarget = 550;
  wait(250, msec);
  m_driveTrain.driveToCoord(-0.90, -0.10, 291, 100, false);
  wait(250, msec);
  m_driveTrain.inertialTurn(270.0, 100);
  m_driveTrain.driveStraight(12, 100, 'f');
  //m_driveTrain.driveToCoord(-1.05, -0.03, 291, 100, false);
  liftClawAuto(false);
  wait(200, msec);
  liftTarget = 450;
  m_driveTrain.driveStraight(10, 100, 'b');
  liftTarget = 0;
  m_driveTrain.inertialTurn(185.0, 100);
  wait(200, msec);
  mogoAuto(false);
  m_driveTrain.driveStraight(8, 100, 'f');
  m_driveTrain.inertialTurn(0.0, 100);
  m_driveTrain.driveStraight(24, 100, 'f');
  liftClawAuto(true);
  liftTarget = 550;
  wait(250, msec);
  m_driveTrain.inertialTurn(270.0, 100);
  wait(100, msec);
  m_driveTrain.driveStraight(16, 100, 'f');
  wait(250, msec);
  liftTarget = 450;
  liftClawAuto(false);
  m_driveTrain.driveStraight(19, 100, 'b');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.68, 0.61, 291, 100, true);
  wait(250, msec);
  m_driveTrain.driveToCoord(0.35, 0.84, 291, 100, false);
  liftClawAuto(true);
  wait(100, msec);
  liftTarget = 700;
  m_driveTrain.driveToCoord(-0.95, 0.2, 291, 100, false);
  m_driveTrain.inertialTurn(270.0, 100);
  m_driveTrain.driveStraight(4, 100, 'f');
  liftTarget = 500;
  wait(250, msec);
  liftClawAuto(false);
  wait(200, msec);
  m_driveTrain.driveStraight(5, 100, 'b');
  liftTarget = 0;
  m_driveTrain.driveStraight(10, 100, 'b');*/
  
  // Last 2 + Park
  m_driveTrain.inertialTurn(180.0, 100);
  wait(200, msec);
  m_driveTrain.driveToCoord(-0.86, -1.1, 291, 100, false);
  m_driveTrain.inertialTurn(180.0, 100);
  m_driveTrain.driveStraight(10, 100, 'f');
  liftClawAuto(true);
  wait(200, msec);
  m_driveTrain.driveStraight(16, 100, 'b');
  wait(200, msec);
  liftTarget = 700;
  m_driveTrain.inertialTurn(86.0, 100);
  wait(300, msec);
  m_driveTrain.driveToCoord(1.40, -1.15, 291, 100, false);
  m_driveTrain.inertialTurn(92.0, 100);
  m_driveTrain.Turn(190, 100, 'l');
  wait(300, msec);
  m_driveTrain.driveStraight(20, 100, 'f');
  liftTarget = 0;
  m_driveTrain.inertialTurn(0.0, 100);
  wait(1000, msec);
  m_driveTrain.autoPark();
  /*m_driveTrain.driveToCoord(-1.14, 1.16, 291, 100, false);
  //m_driveTrain.driveToCoord(-1.20, 1.03, 291, 100, false);
  liftClawAuto(true);
  wait(200, msec);
  m_driveTrain.driveStraight(12, 100, 'b');
  wait(200, msec);
  liftTarget = 550;
  m_driveTrain.driveToCoord(1.45, 1.40, 291, 100, true);
  wait(200, msec);
  m_driveTrain.Turn(270, 100, 'l');
  m_driveTrain.driveStraight(10, 100, 'f');
  m_driveTrain.Turn(30, 100, 'r');
  m_driveTrain.driveStraight(8, 100, 'b');
    // Park
  liftTarget = 0;
  wait(750, msec);
  //m_driveTrain.autoPark();*/

  liftTask.stop();
  intakeTask.stop();
}

void o_skills() {
  liftTarget = 0;
  intakeTask = task (intakeSkills);
  liftTask = task (liftAsync); // Start lift task
  liftTarget = 0;
  //liftAsync(LIFT_FLIPOUT);
  mogoAuto(false);
  m_driveTrain.driveStraight(13, 500, 'b');
  mogoAuto(true);
  m_driveTrain.Turn(24, 100, 'r');
  m_driveTrain.driveStraight(9, 500, 'f');
  m_driveTrain.Turn(165, 100, 'l');
  m_driveTrain.driveStraight(24, 500, 'b');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.33, -0.95, 294.5, 100, false);//m_driveTrain.driveToCoord(0.33, -1.02, 294.5, 100, false);
  //m_driveTrain.turnWithGPS(273, 300);
  liftClawAuto(true);
  wait(250, msec);
  liftTarget = 550;
  wait(250, msec);
  m_driveTrain.driveToCoord(-0.73, -0.02, 291, 100, false);//m_driveTrain.driveToCoord(-0.85, -0.75, 291, 100, false);
  //m_driveTrain.driveToCoord(-0.85, -0.1, 350, 100, false);
  //m_driveTrain.driveToCoord(-0.95, -0.1, 283, 100, false);
  liftTarget = 350;
  wait(250, msec);
  liftClawAuto(false);
  m_driveTrain.driveStraight(1.5, 500, 'b');
  liftTarget = 550;
  wait(100, msec);
  //m_driveTrain.turnWithGPS(182, 500);//driveToCoord(-0.96, -0.1, 175, 500, true);
  liftTarget = 0;
  mogoAuto(false);
  wait(250, msec);
  m_driveTrain.driveToCoord(-0.98, -1.21, 182, 500, false);
  liftClawAuto(true);
  wait(100, msec);
  m_driveTrain.driveToCoord(-0.98, -0.86, 180, 500, true);
  liftTarget = 550;
  m_driveTrain.driveToCoord(1.12, 1.20, 214, 500, true);
  m_driveTrain.driveStraight(17.5, 500, 'f');
  //m_driveTrain.turnWithGPS(182, 500);
  m_driveTrain.driveStraight(26, 500, 'f');
  m_driveTrain.driveToCoord(0.87, 0.0, 110, 500, false);
  liftTarget = 350;
  wait(250, msec);
  liftClawAuto(false);
  m_driveTrain.driveStraight(1.5, 500, 'b');
  liftTarget = 550;
  m_driveTrain.driveStraight(3, 500, 'b');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.97, 1.22, 20, 500, false);
  liftClawAuto(true);
  wait(250, msec);
  liftTarget = 550;
  m_driveTrain.driveStraight(17.5, 500, 'f');
  m_driveTrain.driveToCoord(-0.97, 0.36, 255, 500, false);
  liftClawAuto(false);
  m_driveTrain.driveStraight(12, 500, 'b');

  liftTask.stop();
  intakeTask.stop();
}

void test() {
  //m_driveTrain.driveStraight(1, 50, 'f');
  //m_driveTrain.inertialTurn(180, 100);
  m_driveTrain.driveToCoord(0.0,0.0,0,100,false);
}

void runAuto() {
  //Brain.Screen.printAt(24, 128, "Running Auto");
  switch(currAuto){
    case 0: test(); break;//leftTip(); break;
    case 1: sauce(); break;
    case 2: rightTip(); break;
    case 3: rightClaw(); break;
    case 4: winPoint(); break;
    case 5: skills(); break;
  }
}