## HI-ROBOT
A small security robot that can navigate around the house like an IROBOT, take pictures, and charge itself.

# The HI-Robot is a tiny security robot that can:
Move autonomously around a defined perimeter.
Move via controls used with a matching application.
Take pictures when a certain criteria is met (time, or other triggers such as movement) and upload them to a DB such that it can be access using the app and the proper user.
Upload live -stream video.
Recognize low battery and dock into a charging point.


# System Parts

ESP32-cam.
ESP32.
2 distance sensors.
2 DC N20 Motors.
L9110s motors driver.
2 wheels and skeleton.
5V battery.
Infrared sensor.
Charging point, Rail, and infrared signal.


# Movement Algorithm Explanation
Using the sensors in their placement as defined earlier, the robot could advance in a straight line, while using the right facing sensor in order to ensure it maintains the same distance from the right wall at any time, and using the front facing sensor in order to detect whenever the robot is approaching a corner in order to make the appropriate turn.

This algorithm is rather simple, but it makes sure the robot is walking in a perimeter around the requested area, and it is far more efficient then using four sensors and implementing a random-based movement algorithm.

