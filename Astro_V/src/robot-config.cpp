#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor Intake = motor(PORT3, ratio6_1, true);
motor Lift = motor(PORT19, ratio36_1, true);
controller Controller1 = controller(primary);
digital_out LiftClaw = digital_out(Brain.ThreeWirePort.A);
motor R1 = motor(PORT17, ratio18_1, true);
motor R2 = motor(PORT11, ratio18_1, false);
motor L1 = motor(PORT15, ratio18_1, false);
motor L2 = motor(PORT14, ratio18_1, true);
limit LAutoClick = limit(Brain.ThreeWirePort.C);
limit RAutoClick = limit(Brain.ThreeWirePort.D);
gps mh8Gps = gps(PORT6, 110.00, -135.00, mm, 180);
motor R3 = motor(PORT13, ratio18_1, true);
motor L3 = motor(PORT16, ratio18_1, false);
inertial mh8Imu = inertial(PORT7);
digital_out mogoClamp = digital_out(Brain.ThreeWirePort.H);
limit LiftBumper = limit(Brain.ThreeWirePort.B);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}