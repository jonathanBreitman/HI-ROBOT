#include <TB6612FNG.h>

#define AIN1 13
#define BIN1 23
#define AIN2 14
#define BIN2 19
#define PWMA 25
#define PWMB 18
#define STBY 33

//motor A is the left side motor
//red line is A1, black line is A2
//motor B is the right side motor
//red line is B2, black line is B1
Tb6612fng motors(STBY, AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(5000);
  Serial.println("starting setup");
  motors.begin();
  Serial.println("finished setup");
}

void loop() {
  Serial.println("moving forward for 5 seconds");
  motors.drive(1.0, 1.0, 5000);
  delay(2000);
}
