/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  McNeil Minions Rubberbandit Code Vex V5                   */
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
#include <cmath>

using namespace vex;

// A global instance of competition
competition Competition;
// define your global instances of motors and other devices here
signed char switched = 1;
bool shooting = false;
char flyWheelVelocity = 130;
// int indexerLen = 170;

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
  intake.setVelocity(75, percent);
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

// void setIndexer(void) {
//   indexer.setPosition(0, degrees);
//   indexer.spinFor(indexerLen, degrees, true);
//   indexer.setStopping(hold);
// }

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  // setIndexer();
  frontLeft.spin(forward, 100, vex::velocityUnits::pct);
  backLeft.spin(forward, 100, vex::velocityUnits::pct);
  frontRight.spin(forward, 100, vex::velocityUnits::pct);
  backRight.spin(forward, 100, vex::velocityUnits::pct);
  intake.spinFor(reverse, 180, degrees);
  frontLeft.stop();
  backLeft.stop();
  frontRight.stop();
  backRight.stop();
  frontLeft.spinFor(reverse, 1080, degrees);
  backLeft.spinFor(reverse, 1080, degrees);
  frontRight.spinFor(reverse, 1080, degrees);
  backRight.spinFor(reverse, 1080, degrees);
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
  char range = 5;
  return flyWheel.velocity(rpm) >= (flyWheelVelocity - range) && flyWheel.velocity(rpm) <= (flyWheelVelocity + range);
}

void index(void) {
  if (!shooting)
    return;
  while (!inRange())
    vex::task::sleep(20);
  if (shooting) {
    indexer.spinFor(forward, 50, degrees, true);
    vex::task::sleep(200);
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
    vex::task::sleep(20);
  }
  flyWheel.stop();
}

void rapidFire(void) {
  shooting = true;
  flyWheel.spin(forward);
  for (char i = 0; i < 3 && shooting; i ++) {
    for (char j = 0, k = 0; k < 3 && shooting; j++) {
      vex::task::sleep(1000);
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
  // setIndexer();
  float driveSpeedMultiplier = 1;
  char deadZone = 1;
  float turnSpeed = 1.5;
  Controller1.ButtonY.pressed(rapidFire);
  Controller1.ButtonR2.pressed(index);
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
    float frontBack = Controller1.Axis3.position(percent);
    float leftRight = Controller1.Axis1.position(percent);

    frontBack = std::abs(frontBack) < 1 ? 0 : frontBack;
    leftRight = std::abs(leftRight) < 1 ? 0 : leftRight;
    
    float LDriveSpeed = driveSpeedMultiplier * (frontBack * switched + leftRight * turnSpeed);
    float RDriveSpeed = driveSpeedMultiplier * (frontBack * switched - leftRight * turnSpeed);
    frontLeft.spin(forward, LDriveSpeed, vex::velocityUnits::pct);
    backLeft.spin(forward, LDriveSpeed, vex::velocityUnits::pct);
    frontRight.spin(forward, RDriveSpeed, vex::velocityUnits::pct);
    backRight.spin(forward, RDriveSpeed, vex::velocityUnits::pct);
    flyWheel.setVelocity(flyWheelVelocity, rpm);

    // Modifier Controls
    if (Controller1.ButtonL2.pressing()) {
      if (Controller1.ButtonUp.pressing()) {
        indexer.spinFor(10, degrees, true);
      } else if (Controller1.ButtonDown.pressing()) {
        indexer.spinFor(-10, degrees, true);
      } else if (Controller1.ButtonRight.pressing()) {
        indexer.spinToPosition(0, degrees, true);
      } else if (Controller1.Axis4.position(percent) <= 0) {
        driveSpeedMultiplier = (float) (Controller1.Axis4.position(percent) + 100) / 100;
      } else if (Controller1.Axis2.position(percent) <= 0) {
        flyWheelVelocity = (Controller1.Axis2.position(percent) + 100) * 1.3;
      } else if (Controller1.Axis2.position(percent) > 0) {
        flyWheelVelocity = Controller1.Axis2.position(percent) * 0.7 + 130;
      }
      turnSpeed = 0.5;
      driveSpeedMultiplier = 0.5;
      // Logs Data on change
      Controller1.Screen.setCursor(0, 0);
      Controller1.Screen.print(driveSpeedMultiplier);
      Controller1.Screen.print(" Drive Speed Multiplier");
      Controller1.Screen.newLine();
      Controller1.Screen.print(flyWheelVelocity);
      Controller1.Screen.print(" rpm ");
      Controller1.Screen.print(turnSpeed);
      Controller1.Screen.print(" turn speed");
      Controller1.Screen.newLine();
      Controller1.Screen.print(indexer.position(degrees));
      Controller1.Screen.print(" degrees");
    } else {
      turnSpeed = 1.5;
      driveSpeedMultiplier = 1;
    }

    // Intake Controls
    if (Controller1.ButtonR1.pressing())
      intake.spin(forward);
    else if(Controller1.ButtonL1.pressing())
      intake.spin(reverse);
    else
      intake.stop();
    vex::task::sleep(5);
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
    vex::task::sleep(100);
  }
}
