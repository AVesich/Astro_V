#include "vex.h"

/*
  // TURNS:
  // 525 deg input -> 90 deg turn
*/

// TASKS
task posTracking;
task liftTask;
task mogoLiftTask;
task intakeTask;

// PID Modes
const struct pid autoPID = pid(0.07, 0.0013, 0.85, 0.07);
const struct pid skillsPID = pid(0.07, 0.0013, 0.85, 0.02);

void leftTip() {
  //flipoutLift();
  //m_driveTrain.inertialTurn(10, 100);
  //currPIDMode = {0.40, 0.0003, 0.15, 0.07}; // Standard straight auto pid
  double pullTime = Brain.timer(sec);
  m_driveTrain.Turn(26, 100, 'r');
  m_driveTrain.driveStraight(42, 100, 'f');
  liftClawAuto(true);
  m_driveTrain.driveStraight(2, 100, 'f');
  wait(100, msec);
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);
  m_driveTrain.driveStraight(38, 90, 'b');
  liftClawAuto(false);
  wait(100, msec);
  m_driveTrain.Turn(25, 85, 'r');
  wait(100, msec);
  m_driveTrain.driveStraight(14, 90, 'b');
  wait(100, msec);
  m_driveTrain.Turn(228, 85, 'l');
  wait(100, msec);
  m_driveTrain.o_driveStraight(21, 100, 'b');
  mogoAuto(true);
  wait(100, msec);
  liftAuto(100, 'u');
  intakeAsync(INTAKE_IN);
  m_driveTrain.o_driveStraight(16, 40, 'f');
  wait(1, sec);
  intakeAsync(INTAKE_OFF);
  liftAuto(100, 'd');
  mogoAuto(false);
}

void sauce() {
  currPIDMode = {1.5, 0.00, 0.06, 0.07};
  double pullTime = Brain.timer(sec);
  m_driveTrain.driveStraight(39, 100, 'f');
  liftClawAuto(true);
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);
  m_driveTrain.o_driveStraight(38, 100, 'b');
  
}

void sauce_() {
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
  m_driveTrain.driveStraight(41, 100, 'f');
  liftClawAuto(true);
  m_driveTrain.driveStraight(2.5, 100, 'f');
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);

  m_driveTrain.driveStraight(24.5, 100, 'b');
  liftAuto(150, 'u');
  m_driveTrain.Turn(185, 60, 'l');
  wait(200, msec);
  m_driveTrain.driveStraight(16, 100, 'b');
  mogoAuto(true);
  wait(200, msec);
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(20, 80, 'f');
  intakeAsync(INTAKE_OFF);
  mogoAuto(false);
  m_driveTrain.driveStraight(23, 80, 'f');
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
  double pullTime = Brain.timer(sec);
  m_driveTrain.o_driveStraight(42.5, 100, 'f');
  liftClawAuto(true);
  pullTime = Brain.timer(sec)-pullTime;
  Brain.Screen.print(pullTime);

  m_driveTrain.driveStraight(27, 100, 'b');
  liftAuto(150, 'u');
  m_driveTrain.Turn(180, 60, 'l');
  wait(200, msec);
  m_driveTrain.o_driveStraight(15, 100, 'b');
  mogoAuto(true);
  wait(200, msec);
  intakeAsync(INTAKE_IN);
  m_driveTrain.driveStraight(20, 80, 'f');
  intakeAsync(INTAKE_OFF);
}

void winPoint() {
  currTurnPIDMode = {0.23, 0.00, 0.03, 0.30 }; // Turn pid accounting for goal weight

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
  m_driveTrain.inertialTurn(90.0, 100);//Turn(200, 100, 'l'); // face the middle of the field
  wait(200, msec);
  liftTarget = 100; // Move the lift up to accomodate for the intake
  m_driveTrain.driveStraight(32, 100, 'b'); // Drive to the middle of the field
  liftClawAuto(false); // Open the claw
  intakeAuto(300, 'u', 600);
  
  wait(200, msec);
  m_driveTrain.inertialTurn(6.0, 100);//Turn(195, 100, 'l'); // face the opposing alliance goal
  currPIDMode = {0.29, 0.00, 0.00, 0.07}; // Standard straight auto pid
  wait(300, msec);
  m_driveTrain.driveStraight(48, 100, 'f'); // Drive to the middle of the field
  mogoAuto(false); // Drop the mogo
  wait(200, msec);
  m_driveTrain.driveStraight(56, 100, 'f'); // Drive to other mogo
  wait(200, msec);
  m_driveTrain.Turn(265, 100, 'l'); // face the opposing alliance goal
  wait(200, msec);
  m_driveTrain.driveStraight(24, 100, 'b'); // Drive forward to the goal
  mogoAuto(true); // Grab the Right alliance mogo
  wait(200, msec);
  intakeAsync(INTAKE_IN);
  m_driveTrain.Turn(80, 100, 'l');
  m_driveTrain.driveStraight(24, 100, 'f'); // Drive forward to the goal
  intakeAsync(INTAKE_OFF);

  liftTarget = 0; // Move the lift back down
  wait(1000, msec);

  // End tasks
  liftTask.stop();
}

void skills() {
  //currPIDMode = {0.07, 0.0013, 0.85, 0.02}; // Skills pid

  //currTurnPIDMode = {0.23, 0.00, 0.03, 0.30 }; // Turn pid accounting for goal weight
  currTurnPIDMode = {0.40, 0.00, 0.56/*0.0543*/, 0.05 }; // Turn pid accounting for goal weight

  // Calibrate the gps
  //mh8Gps.calibrate();
  //while (mh8Gps.isCalibrating()) { task::sleep(50); }

  liftTarget = 0;
  intakeTask = task (intakeSkills);
  //liftTask = task (liftAsync); // Start lift task
  //liftTarget = 0;
  mogoAuto(false);
  m_driveTrain.driveStraight(16, 100, 'b');
  wait(200, msec);
  mogoAuto(true);
  m_driveTrain.Turn(24, 100, 'r');
  wait(200, msec);
  //currPIDMode = {0.17, 0.0011, 0.85, 0.07}; // Standard auto pid
  m_driveTrain.o_driveStraight(9, 100, 'f');
  wait(200, msec);
  m_driveTrain.Turn(165, 100, 'l');
  wait(200, msec);
  m_driveTrain.o_driveStraight(24, 100, 'b');
  wait(200, msec);
  //liftTarget = 0;
  m_driveTrain.inertialTurn(263, 70);
  wait(200, msec);
  m_driveTrain.driveStraight(21, 70, 'f');
  liftClawAuto(true);
  liftAuto(100, 'u');
  wait(200, msec);
  m_driveTrain.o_driveStraight(8, 60, 'f');
  //currTurnPIDMode = {0.80, 0.0017, 0.15, 0.00 }; // Turn pid accounting for goal weight
  liftAuto(600, 'u');//liftTarget = 700;
  currTurnPIDMode = {0.42, 0.00, 0.55/*0.0543*/, 0.05 }; // Turn pid accounting for goal weight
  m_driveTrain.inertialTurn(311, 100);
  currTurnPIDMode = {0.40, 0.00, 0.56/*0.0543*/, 0.05 }; // Turn pid accounting for goal weight
  wait(200, msec);
  m_driveTrain.driveStraight(48, 100, 'f');
  wait(200, msec);
  m_driveTrain.inertialTurn(274.0, 100);
  wait(200, msec);
  m_driveTrain.o_driveStraight(13, 50, 'f');
  //currTurnPIDMode = {0.7, 0.000, 0.20, 0.00 }; // Turn pid accounting for goal weight
  liftAuto(250, 'd');//liftTarget = 500;
  wait(500, msec);
  liftClawAuto(false);
  wait(200, msec);
  liftAuto(150, 'u');//liftTarget = 600;
  m_driveTrain.driveStraight(8, 100, 'b');
  liftAuto(598, 'd');//liftTarget = 0;
  wait(200, msec);
    currTurnPIDMode = {0.36, 0.00, 0.56/*0.0543*/, 0.05 }; // Turn pid accounting for goal weight
  m_driveTrain.inertialTurn(181.0, 100);
  wait(200, msec);
  mogoAuto(false);
  m_driveTrain.driveStraight(12, 100, 'f');
  wait(200, msec);
  m_driveTrain.inertialTurn(0, 100);
    currTurnPIDMode = {0.40, 0.00, 0.58/*0.0543*/, 0.05 }; // Turn pid accounting for goal weight
  wait(200, msec);
  m_driveTrain.driveStraight(24, 100, 'f');
  liftClawAuto(true);
  liftAuto(100, 'u');
  m_driveTrain.o_driveStraight(4, 100, 'f');
  wait(100, msec);
  liftAuto(600, 'u');//liftTarget = 700;
  wait(200, msec);
  m_driveTrain.inertialTurn(272.0, 70);
  wait(200, msec);
  m_driveTrain.o_driveStraight(13, 100, 'f');
  liftAuto(200, 'd');//liftTarget = 500;
  wait(250, msec);
  liftClawAuto(false);
  wait(250, msec);
  m_driveTrain.o_driveStraight(2, 100, 'b');
      currTurnPIDMode = {0.945, 0.00, 0.00/*0.0543*/, 0.05 };
  m_driveTrain.inertialTurn(270.0, 100);
      currTurnPIDMode = {0.40, 0.00, 0.57/*0.0543*/, 0.05 };
  m_driveTrain.driveStraight(20, 100, 'b');
      //liftAuto(610, 'd');//liftTarget = 0;
      //m_driveTrain.driveStraight(10, 100, 'b');
  //currTurnPIDMode = {0.53, 0.0012, 0.50, 0.00 }; // Turn pid accounting for goal weight
  wait(200, msec);
  m_driveTrain.inertialTurn(36.5, 65);
  wait(200, msec);
  m_driveTrain.driveStraight(21, 60, 'f');
  wait(200, msec);
  m_driveTrain.driveStraight(5, 60, 'b');
  liftAuto(510, 'd');
  m_driveTrain.driveStraight(12, 70, 'f');
  liftClawAuto(true);
  m_driveTrain.o_driveStraight(4.5, 100, 'f');
  wait(200, msec);
  liftAuto(100, 'u');
  m_driveTrain.Turn(21, 100, 'l');
  m_driveTrain.o_driveStraight(48, 100, 'b');
  liftAuto(500, 'u');
  wait(200, msec);
  m_driveTrain.inertialTurn(272.0, 50);
  wait(200, msec);
  m_driveTrain.o_driveStraight(14, 100, 'f');
  wait(200, msec);
  // Drop last goal
  liftClawAuto(false);
  wait(200, msec);
  m_driveTrain.driveStraight(5, 100, 'b');
      currTurnPIDMode = {1.35, 0.00, 0.85/*0.0543*/, 0.05 };
  m_driveTrain.inertialTurn(270, 65);
      currTurnPIDMode = {0.43, 0.00, 0.575/*0.0543*/, 0.05 };
  wait(200, msec);
  m_driveTrain.driveStraight(10, 100, 'b');
  // Push Big In
  m_driveTrain.o_driveStraight(41, 50, 'b');
    // Quick clamp and unclamp prevents tippy so it doesnt move out of zone or hit the platform
    mogoAuto(true);
    wait(300, msec);
    mogoAuto(false);
  m_driveTrain.driveStraight(49, 90, 'f');

  wait(200, msec);
  m_driveTrain.inertialTurn(179.0, 15);
  wait(200, msec);
      currTurnPIDMode = {0.85, 0.00, 0.00/*0.0543*/, 0.05 };
  m_driveTrain.inertialTurn(184.0, 70);
      currTurnPIDMode = {0.35, 0.00, 0.56/*0.0543*/, 0.05 };
  wait(200, msec);
  m_driveTrain.o_driveStraight(44, 100, 'f');
  wait(200, msec);
  m_driveTrain.o_driveStraight(5, 60, 'b');
  liftAuto(600, 'd');
  m_driveTrain.o_driveStraight(10, 60, 'f');
  liftClawAuto(true);
  wait(200, msec);
  m_driveTrain.o_driveStraight(3, 100, 'b');
  wait(200, msec);
  liftAuto(700, 'u');//liftTarget = 700;
  m_driveTrain.o_driveStraight(3, 100, 'b');
  wait(200, msec);
      currTurnPIDMode = {0.33, 0.00, 0.58/*0.0543*/, 0.05 }; // Turn pid accounting for goal weight
  m_driveTrain.inertialTurn(95.0, 30);
  wait(300, msec);
  m_driveTrain.driveStraight(75.5, 60, 'f');
  wait(200, msec);
      currTurnPIDMode = {0.85, 0.00, 0.00/*0.0543*/, 0.05 };
  m_driveTrain.inertialTurn(90.0, 30);
  wait(200, msec);
    // No need to change turn pid back because no more turns are done
  m_driveTrain.Arc(12, 32, 48, 155, 'f');
  wait(500, msec);
  // m_driveTrain.inertialTurn(45.0, 30);
  // wait(200, msec);
  // m_driveTrain.driveStraight(8, 60, 'f');
  // wait(200, msec);
  // currTurnPIDMode = {0.70, 0.00, 0.00/*0.0543*/, 0.05 };
  // m_driveTrain.inertialTurn(10.0, 30);
  liftAuto(690, 'd');//liftTarget = 0;
  m_driveTrain.autoPark();


  //wait(200, msec);
  //m_driveTrain.inertialTurn(272.0, 100);
  //m_driveTrain.driveStraight(7.5, 100, 'f');

  //liftTask.stop();
  intakeTask.stop();
}

void o_skills() {
  //currPIDMode = {0.07, 0.0013, 0.85, 0.02}; // Skills pid

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
  //currPIDMode = {0.17, 0.0011, 0.85, 0.07}; // Standard auto pid
  m_driveTrain.driveStraight(9, 100, 'f');
  m_driveTrain.Turn(165, 100, 'l');
  m_driveTrain.driveStraight(24, 100, 'b');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.24, -0.89, 282, 100, false);
  liftClawAuto(true);
  wait(200, msec);
  liftTarget = 700;
  m_driveTrain.driveToCoord(-0.96, -0.15, 315, 100, false);
  wait(200, msec);
  m_driveTrain.inertialTurn(272.0, 100);
  m_driveTrain.driveStraight(7.5, 100, 'f');
  //m_driveTrain.driveToCoord(-1.05, -0.03, 291, 100, false);
  liftTarget = 450;
  wait(650, msec);
  liftClawAuto(false);
  liftTarget = 600;
  m_driveTrain.driveStraight(10, 100, 'b');
  liftTarget = 0;
  //m_driveTrain.inertialTurn(177.0, 100);
  //wait(200, msec);
  m_driveTrain.inertialTurn(180.0, 100);
  mogoAuto(false);
  m_driveTrain.driveStraight(11, 100, 'f');
  m_driveTrain.inertialTurn(358.0, 100);
  m_driveTrain.driveStraight(16, 100, 'f');
  liftClawAuto(true);
  liftTarget = 600;
  m_driveTrain.driveToCoord(-0.98, 0.15, -1, 100, false);
  m_driveTrain.inertialTurn(272.0, 100);
  wait(100, msec);
  m_driveTrain.driveStraight(7.5, 100, 'f');
  wait(250, msec);
  liftTarget = 450;
  wait(250, msec);
  liftClawAuto(false);
  liftTarget = 600;
  m_driveTrain.driveStraight(19, 100, 'b');
  liftTarget = 0;
  m_driveTrain.driveToCoord(0.68, 0.63, -1, 100, true);
  wait(250, msec);
  m_driveTrain.driveToCoord(0.36, 0.90, 280, 100, false);
  liftClawAuto(true);
  wait(100, msec);
  liftTarget = 700;
  m_driveTrain.driveToCoord(-0.98, 0.0, -1, 100, false);
  currPIDMode = {0.17, 0.0011, 0.85, 0.07}; // Standard auto pid
  m_driveTrain.inertialTurn(270.0, 100);
  m_driveTrain.driveStraight(7.5, 100, 'f');
  liftTarget = 500;
  wait(250, msec);
  liftClawAuto(false);
  wait(200, msec);
  m_driveTrain.driveStraight(5, 100, 'b');
  liftTarget = 0;
  m_driveTrain.driveStraight(10, 100, 'b');
  
  // Last 2 + Park
  m_driveTrain.inertialTurn(180.0, 100);
  wait(200, msec);
  m_driveTrain.driveToCoord(-0.88, -1.2, 291, 100, false);
  wait(200, msec);
  /*liftClawAuto(true);
  wait(200, msec);
  m_driveTrain.driveStraight(16, 100, 'b');
  wait(250, msec);
  liftTarget = 700;
  m_driveTrain.inertialTurn(90.0, 100);
  wait(250, msec);
  m_driveTrain.driveToCoord(1.40, -1.10, 291, 100, false);
  wait(250, msec);
  m_driveTrain.inertialTurn(0, 100);
  wait(250, msec);
  m_driveTrain.driveStraight(16, 100, 'f');
  liftTarget = 0;
  m_driveTrain.inertialTurn(0.0, 100);
  wait(1000, msec);
  //m_driveTrain.autoPark();*/
  

  liftTask.stop();
  intakeTask.stop();
}

void test() {
  /*m_driveTrain.resetPos();

  posTracking = task (m_driveTrain.trackPos);
  m_driveTrain.printDebug();*/

  // printf("x: %4.2f", );
  // printf("y: %4.2f", m_driveTrain.getY());

  m_driveTrain.Arc(12, 32, 48, 155, 'f');
  wait(500, msec);
  liftAuto(700, 'd');//liftTarget = 0;
  m_driveTrain.autoPark();
}

void runAuto() {
  //Brain.Screen.printAt(24, 128, "Running Auto");
  currPIDMode = {0.31, 0.0003, 0.00, 0.07}; // Standard straight auto pid
  currTurnPIDMode = {0.25, 0.0011, 0.02, 0.00 }; // Turn pid accounting for goal weight
  switch(currAuto){
    case 0: leftTip(); break;//leftTip(); break;
    case 1: sauce(); break;
    case 2: rightTip(); break;
    case 3: rightClaw(); break;
    case 4: winPoint(); break;
    case 5: skills(); break;
    case 6: test(); break;
  }
}