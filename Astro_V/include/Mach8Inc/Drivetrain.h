#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "Mach8Inc/Config.h"

using namespace mh8_Variables;

namespace mh8_Drive {
  class mh8_Drivetrain {
    // Motor declarations
    /*motor lFront = motor(3, ratio6_1, true);
    motor rFront = motor(13, ratio6_1, true);
    motor lBack = motor(4, ratio6_1, true);
    motor rBack = motor(14, ratio6_1, true);*/

    public:
      // Basic control
      void driveLeftVolt(double vltg);
      void driveRightVolt(double vltg);
      void driveVoltPID(double targetSpeed, char driveSide);

      void driveLeft(double pct);
      void driveRight(double pct);

      // DriveOp
      void tankDrive(int lStick, int rStick);
      void arcadeDrive(int xInput, int yInput);

      // Sensor-based driving
      void driveToObject(float maxPower, float curveTime, double sensitivity);
      void autoPark();

          // Odom
          static int trackPos();

          void resetPos();
          void setPos(double x, double y);

          double getX();
          double getY();
          double getAngle();

          static void printDebug();

      // Normal inch-based funcs
      void driveStraight(double inches, double maxPct, char dir);
      void o_driveStraight(double inches, double maxPct, char dir);
      void Turn(double deg, int maxSp, char dir);
      void inertialTurn(double angle, int maxTurnSp);
      void Arc(double lInches, double rInches, double lSpeed, double rSpeed, char dir);

      // Gps-based funcs
      void initGps(double xOffset, double yOffset, double rotation);

      void driveToCoord(double x, double y, double angle, double maxTurnSp, bool reversed);
      void turnWithGPSCoords(double coordsTo[2], double angle, double maxTurnSp, bool reversed);
      void turnWithGPS(double angle, double maxTurnSp);
      //double angleBetween(double delX, double delY);

      // Math
      double static Delta(double v1, double v2);
      double distanceBetween(double delX, double delY);
      double toRadians(double angle);
      double toDegrees(double angle);

      // Utility
      double getAvgDriveSideDeg(char side);
      static double getAvgDriveSideRev(char side);
      double getAvgDriveSideVelo(char side);
      bool driving();
      void resetDrive();
      void setBrake(char mode);
  };
}

#endif
