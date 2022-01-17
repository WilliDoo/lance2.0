#include "vex.h"

#define K_STRAFE 1.2
#define DEADBAND_RANGE 5.0

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
motor leftMotorA = motor(PORT11, ratio18_1, false);
motor leftMotorB = motor(PORT20, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT1, ratio18_1, true);
motor rightMotorB = motor(PORT10, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
inertial DrivetrainInertial = inertial(PORT13);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart,
                                   DrivetrainInertial, 319.19, 320, 40, mm, 1);
controller Controller1 = controller(primary);
motor arm = motor(PORT12, ratio36_1, true);
distance DistanceSensor = distance(PORT19);
motor topHook = motor(PORT2, ratio18_1, false);
motor leftHook = motor(PORT14, ratio18_1, true);
motor backHook = motor(PORT8, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

void strafec(vex::directionType myDirection, float distance, float speed) {
  float numberDeg;
  numberDeg = distance / (4 * 3.1415) * 360  * K_STRAFE;

  leftMotorA.spinFor(myDirection, numberDeg, deg, speed, velocityUnits::pct,
                     false);
  rightMotorA.spinFor(myDirection, -1 * numberDeg, deg, speed,
                      velocityUnits::pct, false);
  leftMotorB.spinFor(myDirection, -1 * numberDeg, deg, speed,
                     velocityUnits::pct, false);
  rightMotorB.spinFor(myDirection, numberDeg, deg, speed, velocityUnits::pct);
}

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  int straight;
  int rotate;
  int drivetrainLeftMotorASpeed,drivetrainLeftMotorBSpeed, drivetrainRightMotorASpeed, drivetrainRightMotorBSpeed;
  bool DrivetrainLMANeedsToBeStopped_Controller1 = false, DrivetrainRMANeedsToBeStopped_Controller1 = false, DrivetrainLMBNeedsToBeStopped_Controller1 = false, DrivetrainRMBNeedsToBeStopped_Controller1 = false;
  while (true) {
    if (RemoteControlCodeEnabled) {

      // if (Controller1.ButtonA.pressing()) {
      //  //Drivetrain.driveFor(vex::forward, 24, distanceUnits::in, 50, velocityUnits::pct);
      //   strafec(forward, 24, 50);
      // }

      int sideways = Controller1.Axis4.position();
      if (abs(sideways) < 20){
        sideways = 0;
      } else {
        rightMotorA.spin(vex::forward, -sideways, percent);
        leftMotorA.spin(vex::forward, sideways, percent);
        rightMotorB.spin(vex::forward, sideways, percent);
        leftMotorB.spin(vex::forward, -sideways, percent);
      }
      straight = Controller1.Axis3.position();
      rotate = Controller1.Axis1.position();
      if (abs(straight) < 50 && abs(straight) > 10) {
        straight = 10 * straight/abs(straight) ;
      } else if (abs(straight) < 100) {
        straight = 50 * straight/abs(straight);
      } else if (abs(straight) == 100) {
        straight = 100 * straight/abs(straight);  
      }
      int sabs = abs(sideways);
      int ssign = sabs/sideways;
      int stabs = abs(straight);
      int stsign = stabs/(straight);
      int rabs = abs(rotate);
      int rsign = rabs/rotate;
      sabs = ((sabs < 5)? 0:(sabs < 10)? sabs:(sabs<50)? 10: (sabs<90)? 50: 100) * ssign;
      stabs = ((stabs < 5)? 0:(stabs < 10)? stabs:(stabs<50)? 10: (sabs<90)? 50: 100) * stsign;
      rabs = ((rabs < 5)? 0:(stabs < 10)? stabs:(rabs<50)? 10:(rabs<90)? 50:100) * rsign;
      drivetrainLeftMotorASpeed = straight + rotate + sideways;   
      drivetrainLeftMotorBSpeed = straight + rotate - sideways;   
      drivetrainRightMotorASpeed =  straight - rotate - sideways;
      drivetrainRightMotorBSpeed =  straight - rotate + sideways;
      int drivetrainHeading = Drivetrain.heading();
      if (drivetrainHeading > 45) {
        std::swap(drivetrainRightMotorASpeed, drivetrainLeftMotorASpeed);
        std::swap(drivetrainLeftMotorASpeed, drivetrainRightMotorBSpeed);
        std::swap(drivetrainLeftMotorASpeed, drivetrainLeftMotorBSpeed);
      } else if (drivetrainHeading > 135) {
        std::swap(drivetrainLeftMotorASpeed, drivetrainRightMotorBSpeed);
        std::swap(drivetrainLeftMotorBSpeed, drivetrainRightMotorASpeed);
      } else if (drivetrainHeading > 225) {
        std::swap(drivetrainRightMotorASpeed, drivetrainLeftMotorASpeed);
        std::swap(drivetrainRightMotorASpeed, drivetrainLeftMotorASpeed);
        std::swap(drivetrainRightMotorASpeed, drivetrainRightMotorBSpeed);
      }
      // calculate the drivetrain motor velocities from the controller joystick
      // axies left = Axis3 + Axis1 right = Axis3 - Axis1
      //
      if (abs(drivetrainLeftMotorASpeed) < 5) {
        if (DrivetrainLMANeedsToBeStopped_Controller1) {
          leftMotorA.stop();
          DrivetrainLMANeedsToBeStopped_Controller1 = false;
        }
      } else {
        DrivetrainLMANeedsToBeStopped_Controller1 = true;
      }
      if (DrivetrainLMANeedsToBeStopped_Controller1) {
        leftMotorA.setVelocity(drivetrainLeftMotorASpeed, percent);
        leftMotorA.spin(forward);
      }
      //
      if (abs(drivetrainLeftMotorBSpeed) < 5) {
        if (DrivetrainLMBNeedsToBeStopped_Controller1) {
          leftMotorB.stop();
          DrivetrainLMBNeedsToBeStopped_Controller1 = false;
        }
      } else {
        DrivetrainLMBNeedsToBeStopped_Controller1 = true;
      }
      if (DrivetrainLMBNeedsToBeStopped_Controller1) {
        leftMotorB.setVelocity(drivetrainLeftMotorBSpeed, percent);
        leftMotorB.spin(forward);
      }
      //
      if (abs(drivetrainRightMotorASpeed) < 5) {
        if (DrivetrainRMANeedsToBeStopped_Controller1) {
          rightMotorA.stop();
          DrivetrainRMANeedsToBeStopped_Controller1 = false;
        }
      } else {
        DrivetrainRMANeedsToBeStopped_Controller1 = true;
      }
      if (DrivetrainRMANeedsToBeStopped_Controller1) {
        rightMotorA.setVelocity(drivetrainRightMotorASpeed, percent);
        rightMotorA.spin(forward);
      }
      //
      if (abs(drivetrainRightMotorBSpeed) < 5) {
        if (DrivetrainRMBNeedsToBeStopped_Controller1) {
          rightMotorB.stop();
          DrivetrainRMBNeedsToBeStopped_Controller1 = false;
        }
      } else {
        DrivetrainRMBNeedsToBeStopped_Controller1 = true;
      }
      if (DrivetrainRMBNeedsToBeStopped_Controller1) {
        rightMotorB.setVelocity(drivetrainRightMotorBSpeed, percent);
        rightMotorB.spin(forward);
      }
      //
      if (Controller1.ButtonL1.pressing()) {
        topHook.spin(forward);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL2.pressing()) {
        topHook.spin(reverse);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        topHook.stop();
        // set the toggle so that we don't constantly tell the motor to stop
        // when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
    //  arm.setVelocity(5,percent);
      // check the ButtonR1/ButtonR2 status to control arm
      if (Controller1.ButtonR1.pressing()) {
        arm.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing()) {
        arm.spin(reverse);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        arm.stop();
        // set the toggle so that we don't constantly tell the motor to stop
        // when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain Inertial
  wait(200, msec);
  DrivetrainInertial.calibrate();
  Brain.Screen.print("Calibrating Inertial for Drivetrain");
  // wait for the Inertial calibration process to finish
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
  wait(50, msec);
  Brain.Screen.clearScreen();
}