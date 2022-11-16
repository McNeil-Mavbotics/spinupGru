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
// intake               motor         1               
// frontLeft            motor         4               
// frontRight           motor         5               
// backLeft             motor         6               
// backRight            motor         7               
// indexer              motor         8               
// flyWheels            motor_group   2, 3            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "pid-controller.cpp"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

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

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
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

void usercontrol(void) {
  // User control code here, inside the loop
  double driveSpeedMultiplier = 1;
  frontLeft.setStopping(coast);
  frontRight.setStopping(coast);
  backLeft.setStopping(coast);
  backRight.setStopping(coast);

  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    double LDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() + Controller1.Axis1.value());
    double RDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() - Controller1.Axis1.value());
    frontLeft.spin(forward, (frontLeft.velocity(percent) + LDriveSpeed) / 2, vex::velocityUnits::pct);
    backLeft.spin(forward, (backLeft.velocity(percent) + LDriveSpeed) / 2, vex::velocityUnits::pct);
    frontRight.spin(forward, (frontRight.velocity(percent) + RDriveSpeed) / 2, vex::velocityUnits::pct);
    backRight.spin(forward, (backRight.velocity(percent) + RDriveSpeed) / 2, vex::velocityUnits::pct);
    if (Controller1.ButtonR1.pressing()) {
      intake.spin(forward);
    } else if (Controller1.ButtonL1.pressing()) {
      intake.spin(reverse);
    } else {
      intake.stop();
    }
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

void shoot() {
  intake.setStopping(coast);
  flyWheels.setStopping(coast);
  intake.setVelocity(100, percent);
  flyWheels.setVelocity(100, percent);
  flyWheels.stop();
  flyWheels.spinFor(15, turns, false);
  indexer.spinFor(350, degrees, true);
  vex::task::sleep(1000);
  indexer.spinFor(10, degrees);
}

void switchSides() {
  vex::motor temp = frontLeft;
  frontLeft = frontRight;
  frontRight = temp;
  temp = backLeft;
  backLeft = backRight;
  backRight = temp;
}

void userControlSplitArcade()
{
  // User control code here, inside the loop
  double driveSpeedMultiplier = 1;
  frontLeft.setStopping(coast);
  frontRight.setStopping(coast);
  backLeft.setStopping(coast);
  backRight.setStopping(coast);

  while (true)
  {
    double LDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() + Controller1.Axis1.value());
    double RDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() - Controller1.Axis1.value());
    frontLeft.spin(forward, LDriveSpeed, vex::velocityUnits::pct);
    backLeft.spin(forward, LDriveSpeed, vex::velocityUnits::pct);
    frontRight.spin(forward, RDriveSpeed, vex::velocityUnits::pct);
    backRight.spin(forward, RDriveSpeed, vex::velocityUnits::pct);
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and calbackLefts.
//
int main() {
  // Set up calbackLefts for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  Controller1.ButtonA.pressed(shoot);
  Controller1.ButtonB.pressed(switchSides);

  // Run the pre-autonomous function.
  pre_auton();

  userControlSplitArcade();
  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
