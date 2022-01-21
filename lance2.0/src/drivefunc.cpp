#include "vex.h"

using namespace vex;

#define K_STRAFE 1.07
#define K_TURN 5.0

void turnNorth() {
  int orientation = DrivetrainInertial.heading();
  if (orientation <= 180) {
    quickTurn(-orientation, 75, true);
  } else {
    quickTurn(360 - orientation, 75, true);
  }
}
void turnSouth() {
  int orientation = DrivetrainInertial.heading();
  quickTurn(180 - orientation, 75, true);
}

void turnEast() {
  int orientation = DrivetrainInertial.heading();
  if (orientation < 270 && orientation > 90)
    quickTurn(90 - orientation, 75, true);
  else
    quickTurn((180 + 90 - orientation) % 180, 75, true);
}

void turnWest() {
  int orientation = DrivetrainInertial.heading();
  if (orientation < 270 && orientation > 90)
    quickTurn((180 + 90 - orientation) % 180, 75, true);
  else
    quickTurn(-((orientation + 90) % 360), 75, true);
}

void resetHeading() { DrivetrainInertial.setHeading(0, degrees); }

void toggleBackHook() {
  if (backHookOn) {
    backHookMotor.rotateTo(0, degrees);
    backHookOn = false;
  } else {
    backHookMotor.rotateTo(90, degrees);
    backHookOn = true;
  }
}

void toggleLeftHook() {
  if (leftHookOn) {
    leftHookMotor.rotateTo(0, degrees);
    leftHookOn = false;
  } else {
    leftHookMotor.rotateTo(90, degrees);
    leftHookOn = true;
  }
}

void onBackBumperPressed() {
  Drivetrain.stop();
  if (!backHookOn) {
    toggleBackHook();
    Controller1.rumble(".");
  }
}
void onLeftBumperPressed() {
  Drivetrain.stop();
  if (!leftHookOn) {
    toggleLeftHook();
    Controller1.rumble(".");
  }
}
void setupRobot() {
  arm.setBrake(hold);
  arm.setRotation(0, degrees);
  arm.setTimeout(3, seconds);
  leftHookMotor.setBrake(hold);
  leftHookMotor.setRotation(0, degrees);
  leftHookMotor.setTimeout(3, seconds);
  backHookMotor.setBrake(hold);
  backHookMotor.setRotation(0, degrees);
  backHookMotor.setTimeout(3, seconds);
  topHookMotor.setBrake(hold);
  topHookMotor.setRotation(0, degrees);
  topHookMotor.setTimeout(3, seconds);

  Drivetrain.setHeading(0, degrees);

  Controller1.ButtonY.pressed(toggleLeftHook);
  Controller1.ButtonB.pressed(toggleBackHook);
  Controller1.ButtonX.pressed(resetHeading);
  Controller1.ButtonLeft.pressed(turnWest);
  Controller1.ButtonRight.pressed(turnEast);
  Controller1.ButtonUp.pressed(turnNorth);
  Controller1.ButtonDown.pressed(turnSouth);

  backBumper.pressed(onBackBumperPressed);
  sideBumper.pressed(onLeftBumperPressed);
}


int setupBand(int position) {
  int bandValue = position;
  int sign = position / abs(position);
  if (abs(position) < 5)
    bandValue = 0;
  if (abs(position) > 10 && abs(position) < 50) {
    bandValue = 10 * sign;
  } else if (abs(position) >= 50 && abs(position) < 90) {
    bandValue = 50 * sign;
  }
  return bandValue;
}

int getOrientation() {
  if (!anyOrientation)
    return 0;
  double heading = DrivetrainInertial.heading();
  if (heading > 45 && heading <= 90 + 45)
    return 1;
  if (heading > 90 + 45 && heading <= 180 + 45)
    return 2;
  if (heading > 180 + 45 && heading <= 270 + 45)
    return 3;
  return 0;
}

void toggleOrientation() { anyOrientation = !anyOrientation; }

void slideFor(float distance, float speed, bool wait_commplete) {
  float numberDeg;
  numberDeg = (distance / (4 * 3.1415)) * 360 * K_STRAFE;
  leftMotorA.spinFor(numberDeg, deg, speed, velocityUnits::pct, false);
  leftMotorB.spinFor(-numberDeg, deg, speed, velocityUnits::pct, false);
  rightMotorA.spinFor(-numberDeg, deg, speed, velocityUnits::pct, false);
  rightMotorB.spinFor(numberDeg, deg, speed, velocityUnits::pct,
                      wait_commplete);
}

void quickTurn(float degree, float speed, bool wait_commplete) {
  double numberDeg;
  numberDeg = degree * K_TURN;
  rightMotorA.spinFor(-1 * numberDeg, deg, speed, velocityUnits::pct, false);
  leftMotorA.spinFor(numberDeg, deg, speed, velocityUnits::pct, false);
  rightMotorB.spinFor(-1 * numberDeg, deg, speed, velocityUnits::pct, false);
  leftMotorB.spinFor(numberDeg, deg, speed, velocityUnits::pct, wait_commplete);
}
