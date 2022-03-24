#include "vex.h"


void intakeOp() {
    directionType intakeDir;
    int intakeSp;

    if(Controller1.ButtonL1.pressing()) {
      if (Lift.position(deg) < 80) {
        Lift.rotateTo(80, deg, 100, rpm);
      }
      intakeDir = directionType::fwd;
      intakeSp = 450;
    } else if (Controller1.ButtonL2.pressing()) {
      if (Lift.position(deg) < 80) {
        Lift.rotateTo(80, deg, 100, rpm);
      }
      intakeDir = directionType::rev;
      intakeSp = 600;
    } else {
      intakeSp = 0;
    }
    Intake.spin(intakeDir, intakeSp, velocityUnits::rpm);
}

// UTILITY
void initIntake() {
  // Motors
  Intake.setBrake(brakeType::hold);
  Intake.resetRotation();
}

void resetIntake() {
    Intake.resetRotation();
}

void stopIntake() {
  Intake.stop();
}

// CONTROL
void intakeAuto(int deg, char dir, int sp) {
  resetIntake();

  directionType intakeDir;

  if (dir == 'u')
    intakeDir = directionType::fwd;
  else if (dir == 'd')
    intakeDir = directionType::rev;

  if (Lift.position(rotationUnits::deg) < 80) {
    Lift.rotateTo(80, rotationUnits::deg, 100, velocityUnits::rpm);
  }

  // Set the average intake degrees to 0
  Intake.rotateFor(deg, rotationUnits::deg, sp, velocityUnits::rpm);
}

void intakeAsync(intakeState m_intakeState) {
    if (m_intakeState == INTAKE_IN) {
        Intake.spin(directionType::fwd, 450, velocityUnits::rpm);
    } else if (m_intakeState == INTAKE_OUT) {
        Intake.spin(directionType::rev, 600, velocityUnits::rpm);
    } else if (m_intakeState == INTAKE_OFF) {
      Intake.stop();
    }
    resetIntake();
}

int intakeSkills() {
  while (1) {
    if (Lift.rotation(deg) > 65) {
      Intake.spin(directionType::fwd, 450, velocityUnits::rpm);
    } else {
      Intake.stop();
    }
    task::sleep(20); // Sleep to save resources
  }

  return 0;
}