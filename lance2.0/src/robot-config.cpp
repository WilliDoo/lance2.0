#include "vex.h"


using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
motor arm = motor(PORT12, ratio36_1, true);
controller Controller1 = controller(primary);
motor leftMotorA = motor(PORT11, ratio18_1, false);
motor leftMotorB = motor(PORT20, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT1, ratio18_1, true);
motor rightMotorB = motor(PORT10, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
inertial DrivetrainInertial = inertial(PORT3);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart,
                                   DrivetrainInertial, 319.19, 320, 40, mm, 1);

motor leftHookMotor = motor(PORT14, ratio18_1, true);
motor backHookMotor = motor(PORT8, ratio18_1, false);
motor topHookMotor = motor(PORT2, ratio18_1, false);

bumper backBumper = bumper(Brain.ThreeWirePort.F);
bumper sideBumper = bumper(Brain.ThreeWirePort.H);
distance backDistance = distance(PORT19);
// distance sideDistance = distance(PORT18);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs

bool Controller1RightShoulderControlMotorsStopped = false;
bool topHookMotorStopped = true;

bool DrivetrainNeedsToBeStopped = false;


// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while (true) {
    if (RemoteControlCodeEnabled) {
      // use heading to automatically switch axis control, e.g. reverse the
      // drive.

      int axis1Value = setupBand(Controller1.Axis1.position());
      int axis3Value = setupBand(Controller1.Axis3.position());
      int axis4Value = setupBand(Controller1.Axis4.position());

      int orientation = getOrientation();
      // int orientation = 0;
      int tmp;
      switch (orientation) {
      case 0:
        // do nothing
        break;
      case 1:
        tmp = axis3Value;
        axis3Value = axis4Value;
        axis4Value = -tmp;
        break;
      case 2:
        axis3Value = -axis3Value;
        axis4Value = -axis4Value;
        break;
      case 3:
        tmp = axis3Value;
        axis3Value = -axis4Value;
        axis4Value = tmp;
        break;
      }

      if (axis1Value == 0 && axis3Value == 0 && axis4Value == 0) {
        if (DrivetrainNeedsToBeStopped) {
          Drivetrain.stop();
          DrivetrainNeedsToBeStopped = false;
        }
      } else {
        int drivetrainLeftASpeed = axis3Value + axis1Value + axis4Value;
        int drivetrainLeftBSpeed = axis3Value + axis1Value - axis4Value;
        int drivetrainRightASpeed = axis3Value - axis1Value - axis4Value;
        int drivetrainRightBSpeed = axis3Value - axis1Value + axis4Value;

        leftMotorA.spin(forward, drivetrainLeftASpeed, percent);
        leftMotorB.spin(forward, drivetrainLeftBSpeed, percent);
        rightMotorA.spin(forward, drivetrainRightASpeed, percent);
        rightMotorB.spin(forward, drivetrainRightBSpeed, percent);

        DrivetrainNeedsToBeStopped = true;
      }

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

      if (Controller1.ButtonL1.pressing()) {
        topHookMotor.spin(forward);
        topHookMotorStopped = false;
      } else if (Controller1.ButtonL2.pressing()) {
        topHookMotor.spin(reverse);
        topHookMotorStopped = false;
      } else if (!topHookMotorStopped) {
        topHookMotor.stop();
        topHookMotorStopped = true;
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

  Controller1.ButtonA.pressed(toggleOrientation);

  wait(50, msec);
  Brain.Screen.clearScreen();
  Controller1.rumble("...");
}