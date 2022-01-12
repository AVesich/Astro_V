#ifndef _LIFT_H_
#define _LIFT_H_

void liftOp();
void clawOp();
void liftClawToggle();

void liftAuto(double deg, char dir);
void flipoutLift();
enum liftPosition { LIFT_FLIPOUT, LIFT_DOWN, LIFT_UP };
int liftAsync();//liftPosition m_liftPos);
void liftClawAuto(bool on);

void initLift();
void resetLift();
void resetLiftEncoders();
bool lifting();

#endif
