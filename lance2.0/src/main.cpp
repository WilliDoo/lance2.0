#include "vex.h"

using namespace vex;



// A global instance of competition
competition Competition;

int selection = -1;

// define your global instances of motors and other devices here
void menu() { // shift is 1 or -1

  int auton = 0;
  std::string txt = "seesaw: rings";
  while (selection == -1) {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(red);
    Brain.Screen.drawRectangle(0, 120, 200, 120);
    Brain.Screen.setPenColor(white);
    Brain.Screen.printAt(23, 144, false, "%s", txt.c_str());
    Brain.Screen.drawRectangle(280, 120, 200, 120);
    Brain.Screen.printAt(300, 144, false, "Next");
    while (!(Brain.Screen.pressing())) {
    }
    int x = Brain.Screen.xPosition();
    //   int y = Brain.Screen.yPosition();
    if (x > 240) {
      auton = (auton + 1) % 11;
      txt = (auton == 0)
              ? "seesaw: 10 rings"
              : (auton == 1)
                    ? "solo"
                    : (auton == 2)
                          ? "awp: ring & mogo"
                          : (auton == 3)
                                ? "awp: mogo & ring"
                                : (auton == 4)
                                      ? "awp: 8 rings"
                                      : (auton == 5)
                                            ? "seesaw: 8 rings & mogo"
                                            : (auton == 6)
                                                  ? "seesaw: 2 rings & mogo"
                                                  : (auton == 7)
                                                        ? "awp: 2 mogo"
                                                        : (auton == 8)
                                                              ? "seesaw: knock"
                                                              : (auton == 9)
                                                                    ? "skills"
                                                                    : "test";
      while (Brain.Screen.pressing()) {
      }
      continue;
    } else if (x < 240) {
      selection = auton;
      Brain.Screen.clearScreen();
      break;
    } else {
      while (Brain.Screen.pressing()) {
      }
      continue;
    }
  }
}
bool backHookOn = false;
bool leftHookOn = false;








void autonomous(void) {
  switch (selection) {
  case 0:
    seesawRings();
    break;
  case 1:
    solo();
    break;
  case 2:
    awpRush();
    break;
  case 3:
    rushAwp();
    break;
  case 4:
    awpRings();
    break;
  case 5:
    seesawRingMogo();
    break;
  case 6:
    seesawRush();
    break;
  case 7:
    awp2Mogos();
    break;
  case 8:
    seesawKnock();
    break;
  case 9:
    skills();
    break;
  case 10:
    test();
    break;
  }
}

void usercontrol(void) {
  arm.setVelocity(10, percent);
  Drivetrain.setStopping(hold);
  while (true) {
    wait(20, msec);
  }
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  menu();
  setupRobot();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}