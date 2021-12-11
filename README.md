
#  TeamK-VexRobot

Code for vex robotics team 55885K-21-22

  

## Motor Mapping

### Chassis Motors
* Right back chassis motor: 6
* Right front chassis motor: 7
* Left back chassis motor: 5
* Left front chassis motor: 8

### Fourbar Motors
* Front fourbar right motor: 9
* Front fourbar left motor: 10


### Miscellaneous
* Radio: 11
* Back lift: 12
* Piston Claw: A
* Select Auton Button: B
* Lock-in Auton Button: C


##  The Controller

### Controlling the Robot
* Button B

    * Resets the front fourbar. While resetting, the robot will be unable to perform any other actions for 1 second.
* Button A

    * Changes the sensitivy of the controller. Default is / 1. Can go up to / 3.5.
* Button X

    * Changes the speed of the robot, cycling by adding 50 with a max of 100 extra speed and a default of 0
* Button Y

    * Opens / Closes the claw for the fourbar
* Left Joystick

    * Controls the left-side of the chassis
* Right Joystick

    * Controls the right-side of the chassis
* Left Button 1/2

    * Move front fourbar up and down.
* Right Button 1/2

    * Move back lift up and down.

### Controller Screen
The section beneath the table shows what value is being set to what using the less than (<) sign.