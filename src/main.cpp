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
// bool started = false;
// PIDController flyWheelController = PIDController();
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
  indexer.setStopping(coast);
  vex::task::sleep(1000);
  indexer.spinFor(indexerLen, degrees, true);
  indexer.setStopping(hold);
}

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  setIndexer();
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

void index(void) {
  while (flyWheel.velocity(rpm) <= (flyWheelVelocity - 5) || flyWheel.velocity(rpm) >= (flyWheelVelocity + 5)) // numbers should be changed
    vex::task::sleep(20);
  indexer.spinFor(forward, 50, degrees, true);
  vex::task::sleep(200);
  indexer.spinFor(forward, 360 - (50), degrees);
  indexer.setPosition(170, degrees);
}

void shoot(void) {
  shooting = true;
  // flyWheelController.changeTarget(130);
  flyWheel.spin(forward);
  // vex::task::sleep(500);
  while (shooting) {
    if (flyWheel.velocity(rpm) >= flyWheelVelocity - 5 && flyWheel.velocity(rpm) <= flyWheelVelocity + 5)
      Controller1.rumble(".");
    if (Controller1.ButtonR2.pressing())
      index();
    vex::task::sleep(20);
  }
  flyWheel.stop();
  // flyWheelController.changeTarget(0);
}

void rapidFire(void) {
  // flyWheelController.changeTarget(130);
  shooting = true;
  flyWheel.spin(forward);
  indexer.setVelocity(100, percent);
  // vex::task::sleep(4000);
  // started = true;
  for (int i = 0; i < 3; i ++) {
    // started = false;
    for (int j = 0, k = 0; k < 3; j++) {
      vex::task::sleep(1000);
      double sped = flyWheel.velocity(rpm);
      Controller1.Screen.setCursor(0, 0);
      Controller1.Screen.print(sped);
      if (sped >= (flyWheelVelocity - 5) && sped <= (flyWheelVelocity + 5)) {
        k++;
      } else {
        k = 0;
      }
      if (!shooting) {
        Controller1.Screen.newLine();
        Controller1.Screen.print(j);
        Controller1.Screen.print(k);
        flyWheel.stop();
        // flyWheelController.changeTarget(0);
        return;
      }
    }
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print(flyWheel.velocity(rpm));
    // Controller1.Screen.print("Launch");
    // Controller1.Screen.print(i);
    if (!shooting)
      break;
    index();
    // started = true;
    // int seconds = 0;
    // while (true) {
    //   seconds += 1;
    //   vex::task::sleep(100);
    //   if (flyWheel.velocity(rpm) >= 120 || seconds >= 50) {
    //     break;
    //   }
    // }
  }
  // started = false;
  flyWheel.stop();
  // flyWheelController.changeTarget(0);
}

// void switchSides(void) {
//   switched *= -1;
// }

// void intakeForward() {
//   if (intake.velocity(percent) > 0) {
//     intake.stop();
//   } else {
//     intake.spin(forward);
//   }
// }

// void intakeBackward() {
//   Controller1.Screen.setCursor(0, 0);
//   if (intake.velocity(percent) < 0) {
//     intake.stop();
//   } else {
//     intake.spin(reverse);
//   }
// }

void usercontrol(void) {
  setIndexer();
  // if (driver != urjith)
  //   die();
  // User control code here, inside the loop
  double driveSpeedMultiplier = 1;
  Controller1.ButtonY.pressed(rapidFire);
  Controller1.ButtonB.pressed([]() {
    switched *= -1;
  });
  Controller1.ButtonX.pressed(shoot);
  // Controller1.ButtonL2.pressed([]() {
  //   if (Controller1.ButtonUp.pressing()) {
  //     indexer.spinFor(10, degrees, true);
  //   } else if (Controller1.ButtonDown.pressing()) {
  //     indexer.spinFor(-10, degrees, true);
  //   } else if (Controller1.ButtonRight.pressing()) {
  //     indexer.spinFor(-1 * indexerLen, degrees, true);
  //   } else if (Controller1.Axis2.position(percent) <= 0) {
  //     flyWheelVelocity = (Controller1.Axis2.position(percent) + 100) * 1.3;
  //   } else if (Controller1.Axis2.position(percent) > 0) {
  //     flyWheelVelocity = Controller1.Axis2.position(percent) * 0.7 + 130;
  //   }
  //   Controller1.Screen.setCursor(0, 0);
  //   Controller1.Screen.print(flyWheelVelocity);
  //   Controller1.Screen.print(" rpm");
  //   Controller1.Screen.newLine();
  //   Controller1.Screen.print(indexer.position(degrees));
  //   Controller1.Screen.print(" degrees");
  // });
  // Controller1.ButtonUp.pressed([]() {
  //   if (Controller1.ButtonL2.pressing()) {
  //     indexer.spinFor(10, degrees, true);
  //   }
  //   Controller1.Screen.setCursor(0, 0);
  //   Controller1.Screen.print(indexer.position(degrees));
  //   Controller1.Screen.print(" degrees");
  // // });
  // Controller1.ButtonDown.pressed([]() {
  //   if (Controller1.ButtonL2.pressing()) {
  //     indexer.spinFor(-10, degrees, true);
  //   }
  //   Controller1.Screen.setCursor(0, 0);
  //   Controller1.Screen.print(indexer.position(degrees));
  //   Controller1.Screen.print(" degrees");
  // });
  // Controller1.ButtonUp.pressed(intakeForward);
  // Controller1.ButtonDown.pressed(intakeBackward);
  Controller1.ButtonLeft.pressed([]() {
    flyWheel.stop();
    indexer.setVelocity(0, percent);
    shooting = false;
  });

  // flyWheelController.setUp(1, 0, 600);
  // double flyWheelVelocity = 0;
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
    double LDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() * switched + Controller1.Axis1.value());
    double RDriveSpeed = driveSpeedMultiplier * (Controller1.Axis3.value() * switched - Controller1.Axis1.value());
    frontLeft.spin(forward, LDriveSpeed / 2, vex::velocityUnits::pct);
    backLeft.spin(forward, LDriveSpeed / 2, vex::velocityUnits::pct);
    frontRight.spin(forward, RDriveSpeed / 2, vex::velocityUnits::pct);
    backRight.spin(forward, RDriveSpeed / 2, vex::velocityUnits::pct);
    flyWheel.setVelocity(flyWheelVelocity, rpm);

    // if (Controller1.ButtonR2.pressing() && Controller1.ButtonX.pressing()) {
    //   indexer.spinFor(forward, 10, degrees);
    // } else if (Controller1.ButtonL2.pressing() && Controller1.ButtonX.pressing()) {
    //   indexer.spinFor(reverse, 10, degrees);
    // }

    // flyWheelController.changeValues(flyWheel.velocity(rpm));
    // flyWheelController.changeValues(flyWheel.velocity(rpm));
    // double velocityOut = flyWheelController.computePID(flyWheel.velocity(rpm)) * 0.02;
    // flyWheelVelocity += velocityOut;
    // if (flyWheelVelocity > 200) {
    //   flyWheelVelocity = 200;
    // }
    // flyWheel.setVelocity(130, rpm);
    // vex::brain::lcd screen = vex::brain::lcd();
    // if (flyWheel.velocity(rpm) > max && started) {
    //   max = flyWheel.velocity(rpm);
    // } else if (flyWheel.velocity(rpm) < min && started) {
    //   min = flyWheel.velocity(rpm);
    // }
    // screen.print(flyWheel.velocity(rpm));
    // screen.print("\n");
    // screen.print(max);
    // screen.print("\n");
    // screen.print(min);
    // screen.print("\n");
    // screen.print(flyWheelVelocity);
    if (Controller1.ButtonL2.pressing()) {
      if (Controller1.ButtonUp.pressing()) {
        indexer.spinFor(10, degrees, true);
      } else if (Controller1.ButtonDown.pressing()) {
        indexer.spinFor(-10, degrees, true);
      } else if (Controller1.ButtonRight.pressing()) {
        indexer.spinFor(-1 * indexerLen, degrees, true);
      } else if (Controller1.Axis2.position(percent) <= 0) {
        flyWheelVelocity = (Controller1.Axis2.position(percent) + 100) * 1.3;
      } else if (Controller1.Axis2.position(percent) > 0) {
        flyWheelVelocity = Controller1.Axis2.position(percent) * 0.7 + 130;
    }
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print(flyWheelVelocity);
    Controller1.Screen.print(" rpm");
    Controller1.Screen.newLine();
    Controller1.Screen.print(indexer.position(degrees));
    Controller1.Screen.print(" degrees");
    };
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print(flyWheelVelocity);
    Controller1.Screen.print(" rpm");
    Controller1.Screen.newLine();
    Controller1.Screen.print(indexer.position(degrees));
    Controller1.Screen.print(" degrees");
    if (Controller1.ButtonR1.pressing())
      intake.spin(forward);
    else if(Controller1.ButtonL1.pressing())
      intake.spin(reverse);
    else
      intake.stop();
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
