/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// intake               motor         10              
// frontLeft            motor         12              
// frontRight           motor         19              
// backLeft             motor         11              
// backRight            motor         20              
// indexer              motor         6               
// flyWheel             motor         17              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "pid.h"

using namespace vex;

// A global instance of competition
competition Competition;
// define your global instances of motors and other devices here
int switched = 1;
bool shooting = false;
int flyWheelVelocity = 130;
int indexerLen = 170;
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  frontLeft.setStopping(hold);
  frontRight.setStopping(hold);
  backLeft.setStopping(hold);
  backRight.setStopping(hold);
  flyWheel.setStopping(coast);
  intake.setVelocity(100, percent);
  indexer.setVelocity(100, percent); 
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void setIndexer(void) {
  indexer.setPosition(0, degrees);
  indexer.spinFor(indexerLen, degrees, true);
  indexer.setStopping(hold);
}

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  // setIndexer();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

bool inRange() {
  int range = 5;
  return flyWheel.velocity(rpm) >= (flyWheelVelocity - range) && flyWheel.velocity(rpm) <= (flyWheelVelocity + range);
}

void index(void) {
  while (!inRange())
    wait(20, msec);
  if (shooting) {
    indexer.spinFor(forward, 50, degrees, true);
    wait(200, msec);
    indexer.spinFor(forward, 310, degrees);
    indexer.setPosition(indexer.position(degrees) - 360, degrees);
  }
}

void shoot(void) {
  shooting = true;
  flyWheel.spin(forward);
  while (shooting) {
    if (inRange())
      Controller1.rumble(".");
    if (Controller1.ButtonR2.pressing())
      index();
    wait(20, msec);
  }
  flyWheel.stop();
}

void rapidFire(void) {
  shooting = true;
  flyWheel.spin(forward);
  for (int i = 0; i < 3 && shooting; i ++) {
    for (int j = 0, k = 0; k < 3 && shooting; j++) {
      wait(1000, msec);
      if (inRange()) {
        k++;
      } else {
        k = 0;
      }
    }
    index();
  }
  flyWheel.stop();
}

void usercontrol(void) {
  setIndexer();
  double driveSpeedMultiplier = 1;
  Controller1.ButtonY.pressed(rapidFire);
  Controller1.ButtonB.pressed([]() {
    switched *= -1;
  });
  Controller1.ButtonX.pressed(shoot);
  Controller1.ButtonLeft.pressed([]() {
    flyWheel.stop();
    shooting = false;
  });

  while (1) {
    // Drive Train Controls
    double LDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() * switched + Controller1.Axis1.value() * 2);
    double RDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() * switched - Controller1.Axis1.value() * 2);
    frontLeft.spin(forward, LDriveSpeed / 2, vex::velocityUnits::pct);
    backLeft.spin(forward, LDriveSpeed / 2, vex::velocityUnits::pct);
    frontRight.spin(forward, RDriveSpeed / 2, vex::velocityUnits::pct);
    backRight.spin(forward, RDriveSpeed / 2, vex::velocityUnits::pct);
    flyWheel.setVelocity(flyWheelVelocity, rpm);

    // Modifier Controls
    if (Controller1.ButtonL2.pressing()) {
      if (Controller1.ButtonUp.pressing()) {
        indexer.spinFor(10, degrees, true);
      } else if (Controller1.ButtonDown.pressing()) {
        indexer.spinFor(-10, degrees, true);
      } else if (Controller1.ButtonRight.pressing()) {
        indexer.spinToPosition(0, degrees, true);
      } else if (Controller1.Axis2.position(percent) <= 0) {
        flyWheelVelocity = (Controller1.Axis2.position(percent) + 100) * 1.3;
      } else if (Controller1.Axis2.position(percent) > 0) {
        flyWheelVelocity = Controller1.Axis2.position(percent) * 0.7 + 130;
      }
    }
    
    // Intake Controls
    if (Controller1.ButtonR1.pressing())
      intake.spin(forward);
    else if(Controller1.ButtonL1.pressing())
      intake.spin(reverse);
    else
      intake.stop();

    // Logs Data every time
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print(flyWheelVelocity);
    Controller1.Screen.print(" rpm");
    Controller1.Screen.newLine();
    Controller1.Screen.print(indexer.position(degrees));
    Controller1.Screen.print(" degrees");
    wait(20, msec);
  }
}

//
// Main will set up the competition functions and calbackLefts.
//
int main() {
  // Set up calbackLefts for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
