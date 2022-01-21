#include "vex.h"

using namespace vex;
#define DUMPING_ANGLE 98

void dump2Rings() {
  arm.spinToPosition(DUMPING_ANGLE, degrees, 20, velocityUnits::pct);
  wait(0.5, seconds);
}

void pickupRings() {
  topHookMotor.spinToPosition(-180, degrees, 100, velocityUnits::pct);
  arm.spinToPosition(0, degrees, 33, velocityUnits::pct);
  topHookMotor.spinToPosition(0, degrees, 20, velocityUnits::pct);
}
void dump8Rings() {
  dump2Rings();
  for (int i = 0; i < 3; i++) {
    pickupRings();
    dump2Rings();
  }
}
void driveBackHook(double distance, double speed) {
  Drivetrain.driveFor(reverse, distance, inches, speed, velocityUnits::pct,
                      false);
  while (!backHookOn) {
    if (Drivetrain.isDone())
      break;
    wait(50, msec);
  }
}
void getMogoBack(double distanceTo, double speedTo, double distanceBack,
                 double speedBack) {
  driveBackHook(distanceTo, speedTo);
  wait(200, msec);
  Drivetrain.driveFor(forward, distanceBack, inches, speedBack,
                      velocityUnits::pct);
  if (backHookOn)
    toggleBackHook();
}
void dumpRingBack(double distance) {
  arm.spinToPosition(33, degrees, 15, velocityUnits::pct);
  driveBackHook(24 * 0.6, 30);
  topHookMotor.spinToPosition(200, degrees, 30, velocityUnits::pct);
  wait(1, seconds);
  Drivetrain.driveFor(forward, distance, inches, 75, velocityUnits::pct);
  toggleBackHook();
  arm.spinToPosition(45, degrees, 50, velocityUnits::pct);
}

void seesawRings() {
  Drivetrain.setHeading(90, degrees);
  dump8Rings();
  pickupRings();
  dump2Rings();
}

void solo() {
  Drivetrain.setHeading(270, degrees);
  arm.spinToPosition(30, degrees, 20, velocityUnits::pct, false);
  topHookMotor.spinToPosition(80, degrees, 15, velocityUnits::pct);
  wait(1, seconds);
  topHookMotor.spinToPosition(0, degrees, 20, velocityUnits::pct, false);

  slideFor(20, 50, true);
  Drivetrain.driveFor(reverse, 24 * 3.4, inches, 50, velocityUnits::pct, true);

  dumpRingBack(15);
}

void awpRush() {
  Drivetrain.setHeading(180, degrees);
  dumpRingBack(15);
  slideFor(24, 50, true);
  double drift = 180 - Drivetrain.heading();
  Drivetrain.turnFor(drift, degrees);
  getMogoBack(24 * 2.5, 60, 30, 75);
}

void rushAwp() {
  Drivetrain.setHeading(180, degrees);
  arm.spinToPosition(33, degrees, 20, velocityUnits::pct, false);

  getMogoBack(24 * 2, 70, 35, 75);
  Drivetrain.driveFor(forward, 7, inches, 50, velocityUnits::pct);
  Drivetrain.turnToHeading(225, deg);
  dumpRingBack(18);
}
void seesawRush() {
  Drivetrain.setHeading(270, degrees);
  arm.spinToPosition(30, degrees, 20, velocityUnits::pct, false);
  topHookMotor.spinToPosition(200, degrees, 40, velocityUnits::pct);
  arm.spinToPosition(45, degrees, 20, velocityUnits::pct, false);
  topHookMotor.spinToPosition(0, degrees, 40, velocityUnits::pct, false);
  quickTurn(-75, 75, true);
  getMogoBack(24 * 2, 70, 24 * 1.5, 100);
}

void seesawRingMogo() {
  dump8Rings();
  quickTurn(100, 75, true);
  getMogoBack(24 * 2.5, 70, 24 * 1.5, 100);
}

// need to test
void awpRings() {
  Drivetrain.setHeading(0, degrees);
  Drivetrain.driveFor(forward, 4, inches, 20, velocityUnits::pct);
  dump8Rings();
  Drivetrain.driveFor(reverse, 7, inches, 75, velocityUnits::pct);
  quickTurn(180, 75, true);
  getMogoBack(15, 50, 24, 100);
}

// knock neutral mogo and opponent robot away
void seesawKnock() {
  Drivetrain.setHeading(0, degrees);
  Drivetrain.driveFor(forward, 24 * 2.5, inches, 100, velocityUnits::pct);
  arm.spinToPosition(45, degrees, 20, velocityUnits::pct, false);
  slideFor(48, 100, true);
}

// pick up both neutral mogos
void awp2Mogos() {
  Drivetrain.setHeading(180, degrees);
  arm.spinToPosition(33, degrees, 20, velocityUnits::pct, false);

  getMogoBack(48, 70, 35, 75);

  Drivetrain.driveFor(forward, 7, inches, 50, velocityUnits::pct);
  int orientation = DrivetrainInertial.heading();
  if (orientation > 270)
    orientation = -(360 - orientation);
  quickTurn(-45 - orientation, 75, true);

  getMogoBack(48, 50, 35, 100);
}


void skills() {
  Drivetrain.driveFor(forward, 4, inches, 20, velocityUnits::pct);
  seesawRings();
  Drivetrain.setHeading(0, degrees);
  toggleBackHook();
  Drivetrain.driveFor(forward, 24 * 2.7, inches, 100, velocityUnits::pct);
  slideFor(-25, 50, true);
  Drivetrain.driveFor(reverse, 24 * 1.7, inches, 100, velocityUnits::pct);
  slideFor(-25 * 1.5, 50, true);
  Drivetrain.driveFor(forward, 24 * 2, inches, 100, velocityUnits::pct);

  int orientation = DrivetrainInertial.heading();
  if (orientation > 270)
    orientation = -(360 - orientation);
  quickTurn(-orientation, 50, true);

  slideFor(-25 * 1.5, 50, true);
  Drivetrain.driveFor(reverse, 24 * 2, inches, 100, velocityUnits::pct);
  Drivetrain.driveFor(forward, 24 * 3.5, inches, 100, velocityUnits::pct);

  orientation = DrivetrainInertial.heading();
  if (orientation > 270)
    orientation = -(360 - orientation);
  quickTurn(-orientation, 50, true);

  slideFor(-25, 50, true);
  Drivetrain.driveFor(reverse, 24 * 3.5, inches, 100, velocityUnits::pct);
  toggleBackHook();
}

void test() {}
