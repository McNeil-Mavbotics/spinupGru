using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor intake;
extern motor frontLeft;
extern motor frontRight;
extern motor backLeft;
extern motor backRight;
extern motor indexer;
extern motor_group flyWheels;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );