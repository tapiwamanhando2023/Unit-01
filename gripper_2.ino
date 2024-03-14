#define GRIPPER_PIN 9 // servo pin
#define GRIPPER_OPEN 1600 // pulse length gripper open
#define GRIPPER_CLOSE 1010// pulse length gripper close
#define GRIPPER_TIME_OUT 20 // gripper needs to be run every 20 ms
#define GRIPPER_TOGGLE 5000 // gripper opens eb closes every 100 ms

void setup() {
pinMode(GRIPPER_PIN, OUTPUT);
digitalWrite(GRIPPER_PIN, LOW);
}

void loop() {
toggleGripper();
}

void gripper(int pulse){
static unsigned long timer;
if( millis() > timer){
digitalWrite(GRIPPER_PIN, HIGH);
delayMicroseconds(pulse);
digitalWrite(GRIPPER_PIN, LOW);
timer = millis()+GRIPPER_TIME_OUT;
}
}

void toggleGripper(){
static bool state;
static unsigned long timer;
if( millis() > timer){
if (state == false){
gripper(GRIPPER_OPEN);
state = true;
}
else {
gripper(GRIPPER_CLOSE);
state = false;
}
timer = millis()+GRIPPER_TOGGLE;
}
}
