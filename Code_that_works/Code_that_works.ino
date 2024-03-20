#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 4

#define LEFT_BACK_LED 0
#define LEFT_FRONT_LED 3

#define RIGHT_BACK_LED 1
#define RIGHT_FRONT_LED 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define ROTATION_SENSOR_LEFT 2  //R1 - 2 - left wheel - red
#define ROTATION_SENSOR_RIGHT 3 //R2 - 3 - right wheel - red

#define GRIPPER_PIN 9 //GR - 0 - gripper - green

#define LEFT_WHEEL_FORWARD 10 //A2 - 10 - left wheel forward - orange
#define LEFT_WHEEL_BACKWARDS 11 //A1 - 11 - left wheel backwards - orange

#define RIGHT_WHEEL_FORWARD 6 //B2 - 6 - right wheel forward - yellow
#define RIGHT_WHEEL_BACKWARDS 5 //B1 - 5 - right wheel backwards - yellow

int sensorPins[] = {A7,A6,A5,A4,A3,A2,A1,A0};

int sensors[] = {0,0,0,0,0,0,0,0};

volatile int countL = 0;
volatile int countR = 0;

void ISR_L(){
  countL++;
}

void ISR_R(){
  countR++;
}

void setup() {
  strip.begin();
  strip.show();

  pinMode(LEFT_WHEEL_FORWARD, OUTPUT);
  pinMode(LEFT_WHEEL_BACKWARDS, OUTPUT);

  pinMode(RIGHT_WHEEL_FORWARD, OUTPUT);
  pinMode(RIGHT_WHEEL_BACKWARDS, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  pinMode(ROTATION_SENSOR_LEFT,INPUT_PULLUP);
  pinMode(ROTATION_SENSOR_RIGHT,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_LEFT), ISR_L, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_RIGHT), ISR_R, CHANGE);

}

void loop() {
  solveMaze();
}

void checkSensorsValues(){
  for(int i = 0; i < sizeof(sensorPins)/sizeof(sensorPins[0]);i++){
    if(analogRead(sensorPins[i])>700){
      sensors[i]=1;
        }
    else{
      sensors[i]=0;
        }
    }
}

void turnRight(){
  colorTurnRight();
  countL=0;
  countR=0;
  while(countL < 10 && countR < 10){
    analogWrite(LEFT_WHEEL_FORWARD, 200);
    analogWrite(LEFT_WHEEL_BACKWARDS, 0);
    analogWrite(RIGHT_WHEEL_FORWARD, 0);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 200);
  }
  checkSensorsValues();
  while(!(sensors[4] || sensors[3])){
    checkSensorsValues();
    analogWrite(LEFT_WHEEL_FORWARD, 200);
    analogWrite(LEFT_WHEEL_BACKWARDS, 0);
    analogWrite(RIGHT_WHEEL_FORWARD, 0);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 200);
  }
}

void turnLeft(){
  colorTurnLeft();
  countL=0;
  countR=0;
  while(countL < 10 && countR < 10){
    analogWrite(LEFT_WHEEL_FORWARD, 0);
    analogWrite(LEFT_WHEEL_BACKWARDS, 200);
    analogWrite(RIGHT_WHEEL_FORWARD, 200);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
  }
  
  checkSensorsValues();
  while(!(sensors[4] || sensors[3])){
    checkSensorsValues();
    analogWrite(LEFT_WHEEL_FORWARD, 0);
    analogWrite(LEFT_WHEEL_BACKWARDS, 200);
    analogWrite(RIGHT_WHEEL_FORWARD, 200);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
  }
}

void beforeTurn(){
  colorForward();

  countL=0;
  countR=0;
  while(countL < 12 && countR < 12){
    analogWrite(LEFT_WHEEL_FORWARD, 200);
    analogWrite(LEFT_WHEEL_BACKWARDS, 0);
    analogWrite(RIGHT_WHEEL_FORWARD, 200);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
  }
}

 void solveMaze(){
    checkSensorsValues();
    
    if(sensors[7]){
      beforeTurn();
      turnRight();
    }
      
    else if(!sensors[0]&&!sensors[1]&&!sensors[2]&&!sensors[3]&&!sensors[4]&&!sensors[5]&&!sensors[6]&&!sensors[7]){
        beforeTurn();
        turnLeft();
    }
    else{
      colorForward();
         if(sensors[6]){
         analogWrite(LEFT_WHEEL_FORWARD, 255);
            analogWrite(LEFT_WHEEL_BACKWARDS, 0);
            analogWrite(RIGHT_WHEEL_FORWARD, 140);
            analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
          }
        else if(sensors[1]){
            analogWrite(LEFT_WHEEL_FORWARD, 140);
            analogWrite(LEFT_WHEEL_BACKWARDS, 0);
            analogWrite(RIGHT_WHEEL_FORWARD, 255);
            analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
        }
        else if(sensors[5]){
            analogWrite(LEFT_WHEEL_FORWARD, 255);
            analogWrite(LEFT_WHEEL_BACKWARDS, 0);
            analogWrite(RIGHT_WHEEL_FORWARD, 230);
            analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
            }
        else if(sensors[2]){
            analogWrite(LEFT_WHEEL_FORWARD, 230);
            analogWrite(LEFT_WHEEL_BACKWARDS, 0);
            analogWrite(RIGHT_WHEEL_FORWARD, 255);
            analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
            }
        else if(sensors[4] && !sensors[3]){
            analogWrite(LEFT_WHEEL_FORWARD, 255);
            analogWrite(LEFT_WHEEL_BACKWARDS, 0);
            analogWrite(RIGHT_WHEEL_FORWARD, 240);
            analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
            }
        else if(sensors[3] && !sensors[4]){
            analogWrite(LEFT_WHEEL_FORWARD, 240);
            analogWrite(LEFT_WHEEL_BACKWARDS, 0);
            analogWrite(RIGHT_WHEEL_FORWARD, 255);
            analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
            }
        else{
            analogWrite(LEFT_WHEEL_FORWARD, 255);
            analogWrite(LEFT_WHEEL_BACKWARDS, 0);
            analogWrite(RIGHT_WHEEL_FORWARD, 255);
            analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
            }
 }
}

//Left side yellow, right side green
void colorTurnLeft() {
  strip.setPixelColor(LEFT_BACK_LED, strip.Color(204,204,0));
  strip.setPixelColor(LEFT_FRONT_LED, strip.Color(204,204,0));

  strip.setPixelColor(RIGHT_BACK_LED, strip.Color(255,0,0));
  strip.setPixelColor(RIGHT_FRONT_LED, strip.Color(255,0,0));

  strip.show();
}

//Left side green, right side yellow
void colorTurnRight() {
  strip.setPixelColor(LEFT_BACK_LED, strip.Color(255, 0, 0));
  strip.setPixelColor(LEFT_FRONT_LED, strip.Color(255, 0, 0));

  strip.setPixelColor(RIGHT_BACK_LED, strip.Color(204,204,0));
  strip.setPixelColor(RIGHT_FRONT_LED, strip.Color(204,204,0));

  strip.show();
}

//All red
void colorStop() {
    for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,255,0));
  }
  strip.show();
}

//All green
void colorForward() {
    for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255,0,0));
  }
  strip.show();
}
