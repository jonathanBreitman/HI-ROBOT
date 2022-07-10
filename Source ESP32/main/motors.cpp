#include "motors.h"

extern int robotMode;
extern int cornerNumber;
extern int currCornerNumber; //  0 - (corner_number-1)
extern time_t lastChargeTime;
extern time_t currentTime;
extern time_t startChargeTime;

int vSpeed = 255;   // Standard Speed can take a value between 0-255
bool backward = false;
bool right = false;
bool left = false;
bool forward = false;

//motor A is the left side motor
//red line is A1, black line is A2
//motor B is the right side motor
//red line is B2, black line is B1
Tb6612fng* motors;

void setupMotorPins() {
  motors = new Tb6612fng(STBY, AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
  motors->begin();
}


void setMotorsValueByCommand(int delay_movement) {
  float my_speed = (float)vSpeed / 255.0;
  Serial.printf("my speed: %f\n", my_speed);
  if (forward && !left && !right){
    Serial.println("Go Forward");
    motors->drive(my_speed, my_speed, delay_movement);
  } else if (left && !forward && !backward) {
    Serial.println("Go Left");
    motors->drive(-my_speed, my_speed, delay_movement);
  } else if (right && !forward && !backward) {
    Serial.println("Go Right");
    motors->drive(my_speed, -my_speed, delay_movement);
  } else if (backward && !left && !right) {
    Serial.println("Go Backward");
    motors->drive(-my_speed, -my_speed, delay_movement);
  } else if (!forward && !backward && !left && !right){
    Serial.println("Stopping");
    motors->brake();
  } else if (forward && left && !backward && !right){
    Serial.println("forward and left");
    motors->drive(-0.5*my_speed, my_speed, delay_movement);
  } else if (forward && !left && !backward && right){
    Serial.println("forward and right");
    motors->drive(my_speed, -0.5*my_speed, delay_movement);
  } else if (!forward && !left && backward && right){
    Serial.println("backwards and right");
    motors->drive(-my_speed, -0.5*my_speed, delay_movement);
  } else if (!forward && left && backward && !right){
    Serial.println("backwards and left");
    motors->drive(-0.5*my_speed, -my_speed, delay_movement);
  }
}

// Check if in corner, if so, check if this is the 0 corner, if so, check if we are after charging interaval 
// and if so get into station and start timer. 
// Return True if got into charging station, False otherwise.
bool chargingHandle(int distanceFront) {           
  if (distanceFront < MIN_DISTANCE_FRONT) {                   // We are in a corner
    currCornerNumber=(currCornerNumber+1)%cornerNumber;
    if (!(currCornerNumber % cornerNumber)) {                 // We are in a 0 corner => we are in charging station 
      time(&currentTime);                                     // Sample current time
      if ((currentTime - lastChargeTime)>CHARGING_INTERVAL) { // It's time to charge! 
        // TODO: get inside the charging station logic fine tuning   
        Serial.println("Go Forward into charging station");
        motors->drive(1.0, 1.0, FORWARD_CHARGING_DELAY);        
        while(!isCharging()){   // TODO: make sure u are connected if failed, make proper adjustment
          Serial.println("Error finding charging station");
          delay(100)
        }  
        Serial.println("Success - in charge");
        time(&startChargeTime);                               // Start clock for charging 
        return true;
      }   
    }    
  }
  return false;
}      

void setMotorsValueBySensors(int distance_right, int distance_front) {
  if (distance_front < MIN_DISTANCE_FRONT) {//We are close to a corner, turn left
    Serial.println("Go Left (90 degree corner)");
    motors->drive(-1.0, 1.0, CORNER_DELAY);
    motors->drive(1.0, 1.0, MOVE_FORWARD_DELAY / 2);
  }
  else if (distance_right > MAX_DISTANCE_RIGHT) {//We are far from the wall, change the direction right a little bit
    Serial.println("Go Right");
    //turn to the right
    motors->drive(1.0, 0.0, WALL_DIST_CORRECTION_DELAY);
    //continue slightly forward
    motors->drive(1.0, 1.0, WALL_DIST_CORRECTION_DELAY*5/2);
    //turn left
    motors->drive(0.0, 1.0, WALL_DIST_CORRECTION_DELAY/2);
  }
  else if (distance_right < MIN_DISTANCE_RIGHT) {//We are too close to the wall, change the direction left a little bit
    Serial.println("Go Left");
    //turn to the left
    motors->drive(-1.0, 1.0, WALL_DIST_CORRECTION_DELAY);
    //continue slightly forward
    motors->drive(1.0, 1.0, WALL_DIST_CORRECTION_DELAY);
    
  }
  else {//If we got to this point, we are not in a corner and the robot is in an acceptable distance from the right wall, move forward
    Serial.println("Go Forward");
    motors->drive(1.0, 1.0, MOVE_FORWARD_DELAY);
  }
}

void stopEngine(){
  motors->brake();
}
