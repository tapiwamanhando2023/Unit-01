#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 4

#define LEFT_BACK_LED 0
#define LEFT_FRONT_LED 3

#define RIGHT_BACK_LED 1
#define RIGHT_FRONT_LED 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define ROTATION_SENSOR_LEFT 3   //R1 - 2 - left wheel - red
#define ROTATION_SENSOR_RIGHT 2  //R2 - 3 - right wheel - red

#define GRIPPER_PIN 9  //GR - 0 - gripper - green

#define CLOSE_GRIP_ANGLE 0
#define OPEN_GRIP_ANGLE 90
#define PULSE_PERIOD_MICROS 20000

#define LEFT_WHEEL_FORWARD 10    //A2 - 10 - left wheel forward - orange
#define LEFT_WHEEL_BACKWARDS 11  //A1 - 11 - left wheel backwards - orange

#define RIGHT_WHEEL_FORWARD 6    //B2 - 6 - right wheel forward - yellow
#define RIGHT_WHEEL_BACKWARDS 5  //B1 - 5 - right wheel backwards - yellow

#define TRIGGER_PIN 8  // trig - 8 pin for sending the sound - yellow
#define ECHO_PIN 12    // echo - 12 pin for receiving the sound - blue

#define SENSOR_TIME_OUT 100  // gripper needs to be run every 20 ms

int sensorPins[] = { A7, A6, A5, A4, A3, A2, A1, A0 };

int sensors[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

volatile int countL = 0;
volatile int countR = 0;

void ISR_L() {
  countL++;
}

void ISR_R() {
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

  pinMode(ROTATION_SENSOR_LEFT, INPUT_PULLUP);
  pinMode(ROTATION_SENSOR_RIGHT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_LEFT), ISR_L, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_RIGHT), ISR_R, CHANGE);

  pinMode(GRIPPER_PIN, OUTPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  openGripper();
  delay(100);

  waitForStart();
  delay(2000);

  startRace();

  delay(500);

  Serial.begin(9600);
}

void waitForStart() {
  delay(100);

  bool wait = true;

  int distance = 25;

  while (wait) {

    int thisDistance = 0;
    for(int i=0;i<3;i++){
      thisDistance+=getDistance();
    }
    
    thisDistance = thisDistance/3;


    if (thisDistance > distance) {
      stop();
      delay(10);
    } else {
      wait = false;
    }
  }
}


void startRace() {


  colorForward();
  countL = 0;
  countR = 0;

  do {
    analogWrite(LEFT_WHEEL_FORWARD, 255);
    analogWrite(LEFT_WHEEL_BACKWARDS, 0);
    analogWrite(RIGHT_WHEEL_FORWARD, 250);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 0);

    checkSensorsValues();
  } while ((countL < 45 && countR < 45) || (sensors[0] && sensors[1] && sensors[2] && sensors[3] && sensors[4] && sensors[5] && sensors[6] && sensors[7]));
  closeGripper();

  colorTurnLeft();
  countL = 0;
  countR = 0;

  do {
    analogWrite(LEFT_WHEEL_FORWARD, 0);
    analogWrite(LEFT_WHEEL_BACKWARDS, 200);
    analogWrite(RIGHT_WHEEL_FORWARD, 200);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    checkSensorsValues();
  } while (!(sensors[4] || sensors[3]) || (countR < 10 && countL < 10));

  do {
    analogWrite(LEFT_WHEEL_FORWARD, 250);
    analogWrite(LEFT_WHEEL_BACKWARDS, 0);
    analogWrite(RIGHT_WHEEL_FORWARD, 250);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
  } while (countL < 10 && countR < 10);
}

void loop() {
  solveMaze();
}

void checkSensorsValues() {
  for (int i = 0; i < sizeof(sensorPins) / sizeof(sensorPins[0]); i++) {
    if (analogRead(sensorPins[i]) > 800) {
      sensors[i] = 1;
    } else {
      sensors[i] = 0;
    }
  }
}

void turnRight() {
  colorTurnRight();
  countL = 0;
  countR = 0;

  do {
    analogWrite(LEFT_WHEEL_FORWARD, 200);
    analogWrite(LEFT_WHEEL_BACKWARDS, 0);
    analogWrite(RIGHT_WHEEL_FORWARD, 0);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 200);
    checkSensorsValues();
  } while (!(sensors[4] || sensors[3]) || (countL < 15 && countR < 15));
}

void turnLeft() {
  colorTurnLeft();
  countL = 0;
  countR = 0;

  do {
    analogWrite(LEFT_WHEEL_FORWARD, 0);
    analogWrite(LEFT_WHEEL_BACKWARDS, 200);
    analogWrite(RIGHT_WHEEL_FORWARD, 200);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    checkSensorsValues();
  } while (!(sensors[4] || sensors[3]) || (countL < 15 && countR < 15));
}

void beforeTurn() {
  colorForward();
  goForwardToCheckSquare();
  // countL = 0;
  // countR = 0;
  // while (countL < 12 && countR < 12) {
  //   analogWrite(LEFT_WHEEL_FORWARD, 255);
  //   analogWrite(LEFT_WHEEL_BACKWARDS, 0);
  //   analogWrite(RIGHT_WHEEL_FORWARD, 255);
  //   analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
  // }
}

void goForwardToCheckSquare() {
  int n = 0;

  for (int i = 0; i < 3; i++) {
    countL = 0;
    countR = 0;

    do {
      analogWrite(LEFT_WHEEL_FORWARD, 255);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 255);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    } while (countL < 4 && countR < 4);
    checkSensorsValues();
    if (sensors[0] && sensors[1] && sensors[2] && sensors[3] && sensors[4] && sensors[5] && sensors[6] && sensors[7]) {
      n++;
    }
  }
  if (n == 3) {
    goBack();
    while (true) {
      stop();
    }
  }
}

void solveMaze() {
  checkSensorsValues();

  if (sensors[7]) {
    beforeTurn();
    turnRight();
  }

  else if (!sensors[0] && !sensors[1] && !sensors[2] && !sensors[3] && !sensors[4] && !sensors[5] && !sensors[6] && !sensors[7]) {
    beforeTurn();
    turnLeft();
  } else {
    colorForward();
    if (sensors[6]) {
      analogWrite(LEFT_WHEEL_FORWARD, 255);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 140);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    } else if (sensors[1]) {
      analogWrite(LEFT_WHEEL_FORWARD, 140);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 255);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    } else if (sensors[5]) {
      analogWrite(LEFT_WHEEL_FORWARD, 255);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 230);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    } else if (sensors[2]) {
      analogWrite(LEFT_WHEEL_FORWARD, 230);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 255);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    } else if (sensors[4] && !sensors[3]) {
      analogWrite(LEFT_WHEEL_FORWARD, 255);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 240);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    } else if (sensors[3] && !sensors[4]) {
      analogWrite(LEFT_WHEEL_FORWARD, 240);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 255);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    } else {
      analogWrite(LEFT_WHEEL_FORWARD, 255);
      analogWrite(LEFT_WHEEL_BACKWARDS, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 255);
      analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
    }
  }
}

//Left side yellow, right side green
void colorTurnLeft() {
  strip.setPixelColor(LEFT_BACK_LED, strip.Color(204, 204, 0));
  strip.setPixelColor(LEFT_FRONT_LED, strip.Color(204, 204, 0));

  strip.setPixelColor(RIGHT_BACK_LED, strip.Color(255, 0, 0));
  strip.setPixelColor(RIGHT_FRONT_LED, strip.Color(255, 0, 0));

  strip.show();
}

//Left side green, right side yellow
void colorTurnRight() {
  strip.setPixelColor(LEFT_BACK_LED, strip.Color(255, 0, 0));
  strip.setPixelColor(LEFT_FRONT_LED, strip.Color(255, 0, 0));

  strip.setPixelColor(RIGHT_BACK_LED, strip.Color(204, 204, 0));
  strip.setPixelColor(RIGHT_FRONT_LED, strip.Color(204, 204, 0));

  strip.show();
}

//All red
void colorStop() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
}

//All green
void colorForward() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
}

void stop() {
  analogWrite(LEFT_WHEEL_FORWARD, 0);
  analogWrite(LEFT_WHEEL_BACKWARDS, 0);
  analogWrite(RIGHT_WHEEL_FORWARD, 0);
  analogWrite(RIGHT_WHEEL_BACKWARDS, 0);
}

void goBack() {
  countL = 0;
  countR = 0;
  do {
    if (countL == 12) {
      openGripper();
    }
    analogWrite(LEFT_WHEEL_FORWARD, 0);
    analogWrite(LEFT_WHEEL_BACKWARDS, 255);
    analogWrite(RIGHT_WHEEL_FORWARD, 0);
    analogWrite(RIGHT_WHEEL_BACKWARDS, 255);
  } while (countL < 60 && countR < 60);
}

void openGripper() {
  int pulseWidth = map(OPEN_GRIP_ANGLE, 0, 180, 1000, 2000);
  sendPulse(pulseWidth);
}

void closeGripper() {
  int pulseWidth = map(CLOSE_GRIP_ANGLE, 0, 180, 1000, 2000);
  sendPulse(pulseWidth);
}

void sendPulse(int pulseWidth) {
  unsigned long startTime = micros();

  while (micros() - startTime < PULSE_PERIOD_MICROS) {
    digitalWrite(GRIPPER_PIN, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(GRIPPER_PIN, LOW);
    delayMicroseconds(PULSE_PERIOD_MICROS - pulseWidth);
  }
}

int getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);  //clear the trig pin
  delayMicroseconds(2);

  digitalWrite(TRIGGER_PIN, HIGH);  //generate sound
  delayMicroseconds(10);            //generate sound for 10ms
  digitalWrite(TRIGGER_PIN, LOW);   //stop generating sound

  long distance = pulseIn(ECHO_PIN, HIGH);  //distance calculation in CM,

  Serial.print("Distance: ");
  Serial.println(distance * 0.034 / 2);

  return distance * 0.034 / 2;
}