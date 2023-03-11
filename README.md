# McNeil Minions Rubberbandit Code Vex V5

### This code is now deprecated, please look at the updated spinupGruPros for our new codebase

This code is written in C++ and is intended for a VEX robotics competition. The code controls a robot with various motors, including front and back left and right motors, intake motor, indexer motor, and flywheel motor.

The pre_auton function is called before the autonomous and user control tasks are started. In this function, the motors are initialized, and the velocities for the intake and indexer motors are set.

The autonomous function controls the robot during the autonomous phase of the competition. In this function, the robot's motors spin forward at 100% power for a certain distance. The intake motor also spins for 180 degrees in reverse. The robot then spins in reverse for 1080 degrees with all of the motors.

The usercontrol function controls the robot during the user control phase of the competition. The function sets the drive speed multiplier, dead zone, and turn speed. The function also defines several button functions for the controller. These include a button to turn rapid fire on, a button to turn the indexer, a button to switch the direction of the motors, and a button to shoot.

The inRange function is used to check whether the flywheel motor is in the correct range of velocity. If the flywheel velocity is within 5 RPM of the target velocity, the function returns true.

The index function is used to control the indexer motor. If the shooting variable is false, the function immediately returns. Otherwise, the function waits until the flywheel velocity is within range, spins the indexer motor for 50 degrees, sleeps for 200 milliseconds, and then spins the indexer motor for 310 degrees. The function then updates the indexer position by subtracting 360 degrees.

The shoot function is used to shoot a ball. The function sets the shooting variable to true and spins the flywheel motor forward. The function then waits until the flywheel velocity is within range. When this occurs, the controller vibrates to indicate that the flywheel is ready to shoot. The function continuously checks the velocity of the flywheel and waits until the user releases the button.

The rapidFire function is used to rapidly fire three balls. The function sets the shooting variable to true and spins the flywheel motor forward. The function then waits until the flywheel velocity is within range. When this occurs, the function waits for one second and checks if the velocity is still within range. If it is, the function increments the variable k. If the velocity is not within range, k is set to 0. The function then waits again for one second and checks the velocity again. This process continues until k is equal to 3. When k is equal to 3, the index function is called to fire a ball. The function then repeats this process two more times.
