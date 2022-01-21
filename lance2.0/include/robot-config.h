using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor arm;
extern motor leftHookMotor;
extern motor backHookMotor;
extern motor topHookMotor;

extern motor leftMotorA;
extern motor leftMotorB;
extern motor rightMotorA;
extern motor rightMotorB;

extern controller Controller1;

extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;

extern smartdrive Drivetrain;
extern inertial DrivetrainInertial;

extern bumper backBumper;
extern bumper sideBumper;
extern distance sideDistance;
extern distance backDistance;

extern bool backHookOn;
extern bool leftHookOn;
extern bool Controller1RightShoulderControlMotorsStopped;
extern bool topHookMotorStopped;
extern bool DrivetrainNeedsToBeStopped;
extern bool anyOrientation;

int setupBand(int position);
int getOrientation();

void vexcodeInit(void);

void quickTurn(float, float, bool);
void slideFor(float, float, bool);

void turnNorth();
void turnSouth();
void turnEast();
void turnWest();
void resetHeading();

void toggleBackHook();
void toggleLeftHook();
void onBackBumperPressed();
void onLeftBumperPressed();
void setupRobot();

void dump2rings();
void pickupRings();
void dump8Rings();
void driveBackHook(double distance, double speed);
void getMogoBack(double distanceTo, double speedTo, double distanceBack, double speedBack);
void dumpRingBack(double distance);

void seesawRings();
void awpRush();
void solo();
void rushAwp();
void seesawRush();
void seesawRingMogo();
void awpRings();
void seesawKnock();
void awp2Mogos();
void skills();
void test();

void toggleOrientation();
void slideFor(float distance, float speed, bool wait_commplete);
void quickTurn(float degree, float speed, bool wait_commplete);
// void slideFor(vex::directionType, float, float, bool );