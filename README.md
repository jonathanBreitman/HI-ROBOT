# HI-ROBOT

A small security robot that can navigate around the house like an IROBOT, take pictures, provide live video feed, and has a self charging function.

## Backstory

This repo is the product of the course [236333 - Project in IoT](https://moodle.technion.ac.il/course/view.php?id=5976), at the Technion university.
This project was created by [Itamar Juwiler](#contributors), [Roi Yona](#contributors), [Jonathan Breitman](#contributors) and guided with the help of Harel Vaknin, Tom Sopher, and Itay Dabran.

## The HI-Robot, Capabilities

* Move autonomously around a perimeter.
* Move manually via controls used with a matching application.
* Provide live-stream video to the application.
* Take pictures on user request and every few minutes automatically, the pictures are then uploaded to the application.
* Recognize when there is low battery and dock into a charging point, exiting when fully charged or on user request.


## System Parts

* AI Thinker ESP32-cam.
* ESP32 Dev Module.
* Two SparkFun VL53L1X Distance Sensors.
* Two Adafruit N20 DC Motors.
* Sparkfun tb6612fng motors driver.
* Two 3D-Printed wheels.
* Robot Skeleton.
* 4V battery, equipped with Voltage Regulator.
* Wireless Charging module for the battery, and a Wireless charging station.
* Charging Rail.
* On/Off button.

## The Robot Physic

The robot has an On/Off button outside, for easy access as shown [here](https://github.com/jonathanBreitman/HI-ROBOT/blob/main/Robot%20Pictures/button_view.jpeg).

The robots physic is very tricycle like, with two wheels in the back, and a guiding rolling wheel in the front.\
The robot has two distance sensors, as seen [here](https://github.com/jonathanBreitman/HI-ROBOT/blob/main/Robot%20Pictures/sensors_view.jpeg), one facing forward and the other one, facing right. These
sensors are used for the autonomous movement, and docking accurately into the charging station.

The charging device is based at the front of the robot, as shown [here](https://github.com/jonathanBreitman/HI-ROBOT/blob/main/Robot%20Pictures/facing_forward.jpeg),
so when the robot is charging, it will go to the charging station, and dock into it facing forward.


## Autonomous Movement Algorithm Explanation
Using the sensors in their placement as defined earlier, the robot could advance in a straight line, while using the right facing sensor in order to ensure it maintains
the same distance from the right wall at any time, and using the front facing sensor in order to detect whenever the robot is approaching a corner in order to make the appropriate turn.

This algorithm is rather simple, but it makes sure the robot is walking in a perimeter around the requested area, and it is far more efficient then using four sensors and implementing a
random-based movement algorithm.

## Automated Charging Explanation

The robot can be configured anytime with the number of corners in the room through the application.
The robot will use that data, together with the data from the sensors, in order to detect when it's facing the corner containing the charging station.

It will then dock into the charging station, if one of the following conditions is met:
* The robot didn't charge in a long time.
* The user requested the robot to charge.
* The battery is low.

After the robot is done charging, the application will notify the user, and the robot will continue moving.

## The Application

We will now explain the application different features.

### Movement

### Photos

### Configurable parameters

## Project Time Table

| Week | Application | Hardware | Controller Code |
| ---- | ----------- | -------- | --------------- |
| | | | |

## Contributors

* [Itamar Juwiler](https://github.com/itamar1208)
* [Jonathan Breitman](https://github.com/jonathanBreitman)
* [Roi Yona](https://github.com/Roi-Yona)