using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor Intake;
extern motor Lift;
extern controller Controller1;
extern digital_out LiftClaw;
extern motor R1;
extern motor R2;
extern motor L1;
extern motor L2;
extern limit LAutoClick;
extern limit RAutoClick;
extern gps mh8Gps;
extern motor R3;
extern motor L3;
extern inertial mh8Imu;
extern digital_out mogoClamp;
extern limit LiftBumper;
extern encoder mh8BackTracker;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );