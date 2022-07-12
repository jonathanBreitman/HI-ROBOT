#include "motors.h"
#include "charge_detect.h"

extern int cornerNumber;
extern int currCornerNumber; //  0 - (corner_number-1)
extern time_t lastChargeTime;
extern time_t currentTime;
extern time_t startChargeTime;
int charging_interval;
extern int charging_forward_delay;

//motor A is the left side motor
//red line is A1, black line is A2
//motor B is the right side motor
//red line is B2, black line is B1
Tb6612fng* motors;

void setupMotorPins() {
  //WebSerial.println("starting motor setup pins");
  motors = new Tb6612fng(STBY, AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);
  motors->begin();
}


void setMotorsValueByCommand(int delay_movement, int vSpeed, bool backward, bool right, bool left, bool forward) {
  float my_speed = (float)vSpeed / 255.0;
  //WebSerial.print("my speed: ");
  //WebSerial.println(my_speed);
  if (forward && !left && !right){
    WebSerial.println("Go Forward");
    motors->drive(my_speed, my_speed, delay_movement);
  } else if (left && !forward && !backward) {
    WebSerial.println("Go Left");
    motors->drive(-my_speed, my_speed, delay_movement);
  } else if (right && !forward && !backward) {
    WebSerial.println("Go Right");
    motors->drive(my_speed, -my_speed, delay_movement);
  } else if (backward && !left && !right) {
    WebSerial.println("Go Backward");
    motors->drive(-my_speed, -my_speed, delay_movement);
  } else if (!forward && !backward && !left && !right){
    //WebSerial.println("Stopping");
    motors->brake();
  } else if (forward && left && !backward && !right){
    WebSerial.println("forward and left");
    motors->drive(-0.5*my_speed, my_speed, delay_movement);
  } else if (forward && !left && !backward && right){
    WebSerial.println("forward and right");
    motors->drive(my_speed, -0.5*my_speed, delay_movement);
  } else if (!forward && !left && backward && right){
    WebSerial.println("backwards and right");
    motors->drive(-my_speed, -0.5*my_speed, delay_movement);
  } else if (!forward && left && backward && !right){
    WebSerial.println("backwards and left");
    motors->drive(-0.5*my_speed, -my_speed, delay_movement);
  }
}

// Check if in corner, if so, check if this is the 0 corner, if so, check if we are after charging interaval 
// and if so get into station and start timer. 
// Return True if got into charging station, False otherwise.
bool chargingHandle(int distanceFront, int user_pressed_charge, bool battery_need_charging) {
  //WebSerial.println("charging handle enter");           
  if (distanceFront < MIN_DISTANCE_FRONT) {// We are in a corner
    WebSerial.print("entered a corner, number: ");
    currCornerNumber=(currCornerNumber+1)%cornerNumber;
    WebSerial.println(currCornerNumber);
    if (currCornerNumber == 0) {                 // We are in a 0 corner => we are in charging station
      time(&currentTime);                                     // Sample current time
      if ((currentTime - lastChargeTime)>charging_interval || user_pressed_charge == 1 || battery_need_charging) { // It's time to charge!
        return true;
      }
    }   
  }
  return false;    
}

void move_into_charging_position(){
    // TODO: get inside the charging station logic fine tuning   
    WebSerial.println("Go Forward into charging station");
    motors->drive(1.0, 1.0, charging_forward_delay); 
    int i = 1;
    bool notCharging = !isCharging();
    delay(20);
    notCharging = notCharging || !isCharging(); 
    delay(20);
    notCharging = notCharging || !isCharging(); 
    while(notCharging){
      WebSerial.println("Error finding charging station, shaking");
      shake_to_charge(i);
      i += 1;
      delay(1500);
      notCharging = !isCharging(); 
      delay(20);
      notCharging = notCharging || !isCharging();
      delay(20);
      notCharging = notCharging  || !isCharging(); 
    }  
    WebSerial.println("Success - in charge");
    time(&startChargeTime);
}

void turn_90_degree_left(){
  WebSerial.println("Go Left (90 degree corner)");
  motors->drive(-1.0, 1.0, CORNER_DELAY);
  motors->drive(1.0, 1.0, CORNER_DELAY_FORWARD);
}

void turn_90_degree_right(){
  WebSerial.println("Go Right (90 degree corner)");
  motors->drive(1.0, -1.0, CORNER_DELAY);
  motors->drive(1.0, 1.0, CORNER_DELAY_FORWARD);
}

void shake_to_charge(int iteration){
  if(iteration % 2 == 0){
    WebSerial.println("shaking right");
    motors->drive(1.0, -1.0, SHAKE_DELAY*iteration);
  } else {
    WebSerial.println("shaking left");
    motors->drive(-1.0, 1.0, SHAKE_DELAY*iteration);
  }
  //drive forward a little
  motors->drive(1.0, 1.0, FIX_FORWARD_CHARGING);
}

void setMotorsValueBySensors(int distance_right, int distance_front) {
  if (distance_front < MIN_DISTANCE_FRONT) {//We are close to a corner, turn left
    turn_90_degree_left();
  }
  else if (MAX_MAX_DISTANCE_RIGHT > distance_right && distance_right > MAX_DISTANCE_RIGHT) {//We are far from the wall, change the direction right a little bit
    WebSerial.println("Go Right");
    //turn to the right
    motors->drive(1.0, 0.0, WALL_DIST_CORRECTION_DELAY);
    //continue slightly forward
    motors->drive(1.0, 1.0, RIGHT_CORRECTION_FORWARD);
    //turn left
    motors->drive(0.0, 1.0, WALL_DIST_CORRECTION_DELAY * 3/4);
  } else if(MAX_MAX_DISTANCE_RIGHT < distance_right){
    turn_90_degree_right();
  } else if (distance_right < MIN_DISTANCE_RIGHT) {//We are too close to the wall, change the direction left a little bit
    WebSerial.println("Go Left");
    //turn to the left
    motors->drive(0.0, 1.0, LEFT_CORRECTION);
    //continue slightly forward
    motors->drive(1.0, 1.0, LEFT_CORRECTION_FORWARD);
    //turn right
    motors->drive(1.0, 0.0, LEFT_CORRECTION * 2/3);    
  }
  else {//If we got to this point, we are not in a corner and the robot is in an acceptable distance from the right wall, move forward
    WebSerial.println("Go Forward");
    motors->drive(1.0, 1.0, MOVE_FORWARD_DELAY);
  }
}

void stopEngine(){
  //WebSerial.println("robot stop engine");
  motors->brake();
}
