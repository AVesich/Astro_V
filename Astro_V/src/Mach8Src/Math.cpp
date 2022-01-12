#include "vex.h"

double mh8_Drivetrain::Delta(double v1, double v2){
  double delta = v2-v1;
  
  return delta;
}

double mh8_Drivetrain::distanceBetween(double delX, double delY) {
  double distance = sqrt(pow(delX, 2)+pow(delY, 2));

  return distance;
}

double mh8_Drivetrain::toRadians(double angle) {
  return angle * (M_PI/180);
}

double mh8_Drivetrain::toDegrees(double angle) {
  return angle * (180/M_PI);
} 