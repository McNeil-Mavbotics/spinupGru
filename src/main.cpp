// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// intake               motor         10              
// frontLeft            motor         12              
// frontRight           motor         19              
// backLeft             motor         11              
// backRight            motor         5               
// indexer              motor         6               
// flyWheels            motor_group   16, 17          
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// intake               motor         20              
// frontLeft            motor         12              
// frontRight           motor         19              
// backLeft             motor         11              
// backRight            motor         5               
// indexer              motor         6               
// flyWheels            motor_group   16, 17          
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "pid.h"

using namespace vex;

// A global instance of competition
competition Competition;
// bool started = false;
PIDController flyWheelController = PIDController();
int switched = 1;
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
  indexer.setVelocity(100, percent);
  indexer.spinFor(100, degrees, true);
  frontLeft.setStopping(hold);
  frontRight.setStopping(hold);
  backLeft.setStopping(hold);
  backRight.setStopping(hold);
  flyWheels.setStopping(coast);
  intake.setVelocity(100, percent);
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

void shoot() {
  flyWheelController.changeTarget(130);
  flyWheels.spin(forward);
  // vex::task::sleep(4000);
  // started = true;
  for (int i = 0; i < 3; i ++) {
    // started = false;
    for (int j = 0; j < 5 && (flyWheels.velocity(rpm) <= 120 || flyWheels.velocity(rpm) > 130); j++) {
      vex::task::sleep(1000);
    }
    Controller1.Screen.print("Launch");
    Controller1.Screen.print(i);
    indexer.spinFor(forward, 280, degrees);
    vex::task::sleep(200);
    indexer.spinFor(forward, 100, degrees);
    // started = true;
    // int seconds = 0;
    // while (true) {
    //   seconds += 1;
    //   vex::task::sleep(100);
    //   if (flyWheels.velocity(rpm) >= 120 || seconds >= 50) {
    //     break;
    //   }
    // }
  }
  // started = false;
  flyWheels.stop();
  flyWheelController.changeTarget(0);
}

void switchSides() {
  switched *= -1;
}

void usercontrol(void) {
  // User control code here, inside the loop
  double driveSpeedMultiplier = 1;
  Controller1.ButtonA.pressed(shoot);
  Controller1.ButtonB.pressed(switchSides);
  flyWheelController.setUp(1.3, 0, 200);
  double flyWheelVelocity = 0;
  // double max = 0;
  // double min = 150;

  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    double LDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() + Controller1.Axis1.value()) * switched;
    double RDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() - Controller1.Axis1.value()) * switched;
    frontLeft.spin(forward, LDriveSpeed / 2, vex::velocityUnits::pct);
    backLeft.spin(forward, LDriveSpeed / 2, vex::velocityUnits::pct);
    frontRight.spin(forward, RDriveSpeed / 2, vex::velocityUnits::pct);
    backRight.spin(forward, RDriveSpeed / 2, vex::velocityUnits::pct);
    if (Controller1.ButtonR1.pressing()) {
      intake.spin(forward);
    } else if (Controller1.ButtonL1.pressing()) {
      intake.spin(reverse);
    } else {
      intake.stop();
    }

    if (Controller1.ButtonR2.pressing() && Controller1.ButtonX.pressing()) {
      indexer.spinFor(forward, 10, degrees);
    } else if (Controller1.ButtonL2.pressing() && Controller1.ButtonX.pressing()) {
      indexer.spinFor(reverse, 10, degrees);
    }

    flyWheelController.changeValues(flyWheels.velocity(rpm));
    flyWheelController.changeValues(flyWheels.velocity(rpm));
    double velocityOut = flyWheelController.computePID(flyWheels.velocity(rpm)) * 0.02;
    flyWheelVelocity += velocityOut;
    if (flyWheelVelocity > 200) {
      flyWheelVelocity = 200;
    }
    flyWheels.setVelocity(flyWheelVelocity, rpm);
    vex::brain::lcd screen = vex::brain::lcd();
    // if (flyWheels.velocity(rpm) > max && started) {
    //   max = flyWheels.velocity(rpm);
    // } else if (flyWheels.velocity(rpm) < min && started) {
    //   min = flyWheels.velocity(rpm);
    // }
    // screen.print(flyWheels.velocity(rpm));
    // screen.print("\n");
    // screen.print(max);
    // screen.print("\n");
    // screen.print(min);
    // screen.print("\n");
    // screen.print(flyWheelVelocity);
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

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
