#include "vex.h"

bool clawClosed = false;

// Funcs
// Operator control
void liftOp() {
  if (Controller1.ButtonY.pressing()) { // Up
    if (Controller1.ButtonA.pressing()) { // Macro up if A is pressed
      Lift.startRotateTo(700, deg, 100, velocityUnits::pct);
    } else {
      if (Lift.position(rotationUnits::deg) < 700) { // Manual down if A is not pressed
        Lift.spin(directionType::fwd, 100, velocityUnits::pct);
      } else {
        Lift.stop();
      }
    }
  } else if (Controller1.ButtonRight.pressing()) { // Down
    if (Controller1.ButtonA.pressing()) { // Macro down if A is pressed
      while (!LiftBumper.pressing()) { Lift.spin(directionType::rev, 100, velocityUnits::pct); }
    } else {
      if (!LiftBumper.pressing()) { // Manual down if A is not pressed
        Lift.spin(directionType::rev, 100, velocityUnits::pct);
      } else {
        Lift.stop();
      }
    }
  } else {
    Lift.stop();
  }
}

void liftClawToggle() {
  LiftClaw.set(!clawClosed);
  clawClosed = !clawClosed;
}

void liftAuto(double deg, char dir) {
  // Tare the lift's position
  resetLift();

  directionType liftDir;

  if (dir == 'u')
    liftDir = directionType::fwd;
  else if (dir == 'd')
    liftDir = directionType::rev;

  Lift.startRotateFor(liftDir, deg, rotationUnits::deg, 100, velocityUnits::pct);

  while (lifting()) {}
}

void flipoutLift() {
  Lift.rotateTo(100, rotationUnits::deg, 100, velocityUnits::pct, true);
  Lift.rotateTo(10, rotationUnits::deg, 100, velocityUnits::pct, true);
}

int liftAsync() {
  while (1) {
    if (Lift.rotation(deg) < liftTarget-5 || Lift.rotation(deg) > liftTarget+5) {
      Lift.rotateTo(liftTarget, rotationUnits::deg, 100, velocityUnits::pct, true);
    }
    task::sleep(20); // Sleep to save resources
  }

  return 0;
}

void liftClawAuto(bool on) {
  LiftClaw.set(on);
  clawClosed = !clawClosed;
}

// Utility
void initLift() {
  Lift.resetRotation();
  Lift.setBrake(brakeType::hold);
}

void resetLift() {
  Lift.resetRotation();
  Lift.stop();
}

void resetLiftEncoders() {
  Lift.resetRotation();
}

bool lifting() {
  if (Lift.isSpinning())
    return true;
  else
    return false;
}