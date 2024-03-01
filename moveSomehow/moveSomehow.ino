int leftP=10;  //B1 - 13
int leftN=11;  //B2 - 12
int rightP=12; //A2 - 11
int rightN=13; //A1 - 10 
int sensorPins[]={A7,A6,A5,A4,A3,A2,A1,A0};

int sensors[] = {0,0,1,1,1,1,0,0};

void setup() {
  // put your setup code here, to run once:
  pinMode(leftP, OUTPUT);
  pinMode(leftN, OUTPUT);
  pinMode(rightP, OUTPUT);
  pinMode(rightN, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  Serial.begin(9600);
}

void loop() {
  lineFollower();
}

void lineFollower(){
  getSensorsValues();
  if((sensors[2]||sensors[5])&&sensors[4]&&sensors[3]||(sensors[5]&&sensors[4])){
    analogWrite(rightP, 255);
    analogWrite(rightN, 0);
    analogWrite(leftP, 255);
    analogWrite(leftN, 0);
  }
  else if(sensors[7]){
    analogWrite(rightP, 255);
    analogWrite(rightN, 0);
    analogWrite(leftP, 0);
    analogWrite(leftN, 0);
    }
    //not working for sure
    else if(sensors[0]){
    analogWrite(rightP, 0);
    analogWrite(rightN, 0);
    analogWrite(leftP, 255);
    analogWrite(leftN, 0);
    }
    else if(sensors[6]){
    analogWrite(rightP, 255);
    analogWrite(rightN, 0);
    analogWrite(leftP, 127);
    analogWrite(leftN, 0);
    }
    //not working
    else if(sensors[1]){
    analogWrite(rightP, 127);
    analogWrite(rightN, 0);
    analogWrite(leftP, 255);
    analogWrite(leftN, 0);
    }
    else if(sensors[5]){
    analogWrite(rightP, 255);
    analogWrite(rightN, 0);
    analogWrite(leftP, 230);
    analogWrite(leftN, 0);
    }
    else if(sensors[2]){
    analogWrite(rightP, 230);
    analogWrite(rightN, 0);
    analogWrite(leftP, 255);
    analogWrite(leftN, 0);
    }
    else{
    analogWrite(rightP, 0);
    analogWrite(rightN, 255);
    analogWrite(leftP, 0);
    analogWrite(leftN, 255);
  }
    lineFollower();
    delay(100);
  }

void solveTheMaze(){
  getSensorsValues();
  if(sensors[6]==1 && sensors[7]==1){do{turnRightForwardFast();getSensorsValues();} while(!((sensors[5]==1 || sensors[2]==1)||(sensors[4]==1 && sensors[3]==1)));}
  else if((sensors[5]==1 || sensors[2]==1)||(sensors[4]==1 && sensors[3]==1)){goForward();}
  else{
  do{turnLeftBackFast();getSensorsValues();} while(!((sensors[5]==1 || sensors[2]==1)||(sensors[4]==1 && sensors[3]==1)));}
  solveTheMaze();
  delay(10);
  }
  


void getSensorsValues(){
  for(int i = 0; i < sizeof(sensorPins)/sizeof(sensorPins[0]);i++){
    if(analogRead(sensorPins[i])>700){
      sensors[i]=1;
      }
    else{
      sensors[i]=0;
      }
      Serial.print(sensors[i]);
      Serial.print(" ");
    }
    Serial.println();
  }











//Movements 
void goBack(){
  digitalWrite(leftP, LOW);
  digitalWrite(leftN, HIGH);
  digitalWrite(rightP, LOW);
  digitalWrite(rightN, HIGH);
}
void stopMoving(){
  digitalWrite(leftP, LOW);
  digitalWrite(leftN, LOW);
  digitalWrite(rightP, LOW);
  digitalWrite(rightN, LOW);
}
void goForward(){
  digitalWrite(leftP, HIGH);
  digitalWrite(leftN, LOW);
  digitalWrite(rightP, HIGH);
  digitalWrite(rightN, LOW);
}
void turnLeftForward(){
  digitalWrite(leftP, HIGH);
  digitalWrite(leftN, LOW);
}
void turnRightForward(){
  digitalWrite(rightP, HIGH);
  digitalWrite(rightN, LOW);
}
void turnLeftForwardFast(){
  digitalWrite(leftP, HIGH);
  digitalWrite(leftN, LOW);
  digitalWrite(rightP, LOW);
  digitalWrite(rightN, HIGH);
}
void turnRightForwardFast(){
  digitalWrite(rightP, HIGH);
  digitalWrite(rightN, LOW);
  digitalWrite(leftP, LOW);
  digitalWrite(leftN, HIGH);
}
void turnRightBack(){
  digitalWrite(leftP, LOW);
  digitalWrite(leftN, HIGH);
}
void turnLeftBack(){
  digitalWrite(rightP, LOW);
  digitalWrite(rightN, HIGH);
}
void turnRightBackFast(){
  digitalWrite(leftP, LOW);
  digitalWrite(leftN, HIGH);
  digitalWrite(rightP, HIGH);
  digitalWrite(rightN, LOW);
}
void turnLeftBackFast(){
  digitalWrite(rightP, LOW);
  digitalWrite(rightN, HIGH);
  digitalWrite(leftP, HIGH);
  digitalWrite(leftN, LOW);
}
