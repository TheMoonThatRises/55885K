
# TeamK-VexRobot

Code for vex robotics team 55885K-21-22

## Motor Mapping

### Chassis Motors

    Right back chassis motor: 6

    Right front chassis motor: 7
    
    Left back chassis motor: 5

    Left front chassis motor: 8

### Fourbar Motors

    Front fourbar right motor: 9

    Front fourbar left motor: 10

### Back Motors

    Backdrag motor: 11

    Backlift motor: 12

### Miscellaneous

    Radio: 13

    Vision Sensor: 14

    Proximity Sensor: 15

    Piston Claw: A

    Select Auton Button: B

    Lock-in Auton Button: C

## The Controller

### Controlling the Robot

#### **Button B**

    Resets the front fourbar. While resetting, the robot will be unable to perform any other actions for 1 second.

#### **Button A**

    Changes the sensitivy of the controller. Default is / 1. Can go up to / 3.5.

#### **Button X**

    Changes the speed of the robot, cycling by adding 50 with a max of 100 extra speed and a default of 0.

#### **Button Y**

    Opens / Closes the claw for the fourbar.

#### **Left Joystick**

    Controls the left-side of the chassis.

#### **Right Joystick**

    Controls the right-side of the chassis.

#### **Left Button 1/2**

    Move front fourbar up and down.

#### **Right Button 1/2**

    Move back lift up and down.

#### **Up/Down Arrow**

    Move back drag up and down.

### Controller Screen

    Beneath the robot's stats, the text shows what value is being set to what using the less than (<) sign.

## Auton

    By default, auton starts on `midGoals`, which goes for the right neutral goal.

### Setup

    To toggle between autons, press the upper of the two buttons on the robot's chassis. The controller will output which auton is currently selected. To ensure that you don't accidently change the auton, press the bottom button. It is best practice to press the bottom button either way.

### Debugging

    The auton works reliably, though it is desynced with the tower, which may cause the robot to either not run auton, or have one side of the robot move faster than the other side.
