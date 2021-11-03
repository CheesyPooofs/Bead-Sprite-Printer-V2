#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>


int firstServoPin = 9; 
Servo firstServo;
int secondServoPin = 8; 
Servo secondServo;
int thirdServoPin = 46; 
Servo thirdServo;
int fourthServoPin = 44; 
Servo fourthServo;

//detect bead in tube
int beadLaserGNDPin = 3;
int lightdet = A4;

//calibration detection
int calLaserGNDPin = 2;
int calLaserDetPin = A0;

//storage tube detection
int storeLaserGNDPin = 13;
int storeLaserDetPin = A8;

//tube detection
int tubeDetPin = A3;
bool pusherDet = false;

bool beadLaserON = false;
bool calLaserON = false;
bool storeLaserON = false;

int toggleButton = 6;
//int vibmotor = 5;
int servoCompletePin = 2; //////////////////////////
int blockPin = 12; ////////////////////////////////
int instantblockPin = 3; ///////////////////////////////
int tubePin = 4; //////////////////////

volatile int msg = 0;
volatile byte buf[4];
volatile int msgStrNum = 0;

//generic for tracking
int firstServoPos = 110;
int secondServoPos = 2;
int thirdServoPos = 80; // compressor
int fourthServoPos = 60; //pusher
//real values
int rfirstServoPos = 112;
int rsecondServoPos = 0;
int rthirdServoPos = 48;
int rfourthServoPos = 58;
//generic for tracking
int firstServoVel = 1;
int secondServoVel = 1;
int thirdServoVel = 3;
int fourthServoVel = 2;



unsigned long timeOutTime = 2000;
unsigned long timeSince = 0;
unsigned long TimeLastChange = 0;
unsigned long TimeStart = 0;
unsigned long TimeNow = 0;
int buttonState = -1;
int block = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(toggleButton  , INPUT_PULLUP);


  pinMode(lightdet  , INPUT);
  pinMode(beadLaserGNDPin,OUTPUT);
  pinMode(storeLaserDetPin,INPUT);
  pinMode(storeLaserGNDPin,OUTPUT);
  pinMode(calLaserDetPin,INPUT);
  pinMode(calLaserGNDPin,OUTPUT);
  pinMode(tubeDetPin,INPUT);

  digitalWrite(beadLaserGNDPin,HIGH);
  digitalWrite(storeLaserGNDPin,HIGH);
  digitalWrite(calLaserGNDPin,HIGH);


  pinMode(blockPin,OUTPUT);
  pinMode(tubePin,OUTPUT);
  digitalWrite(tubePin,HIGH);
  pinMode(instantblockPin,OUTPUT);

  pinMode(servoCompletePin,OUTPUT);
  firstServo.attach(firstServoPin);   
  secondServo.attach(secondServoPin);  
  thirdServo.attach(thirdServoPin);   
  fourthServo.attach(fourthServoPin); 

  Wire.begin(32);               // join i2c bus with address #10
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(sendAnswer);
  Serial.begin(9600);
  TimeStart = millis();
}
int laserValue = 0;
int distMoved = 0;
void loop() {
  //Serial.println(" bead: " + String(analogRead(lightdet)) + " | cal: " + String(analogRead(calLaserDetPin))+ " | store: " + String(analogRead(storeLaserDetPin)));
  Serial.println("p: " + String(analogRead(tubeDetPin)));
  // put your main code here, to run repeatedly:
  if (beadLaserON){
    laserValue = analogRead(lightdet) ;
    if (laserValue > 400){
      block = 0;
      TimeStart = millis(); //start over
      digitalWrite(blockPin,LOW);
      digitalWrite(instantblockPin,LOW);
    }
    else{
      digitalWrite(instantblockPin,HIGH);
    }
    if (millis() - TimeStart > 1000){
      block = 1;
      digitalWrite(blockPin,HIGH);
      //Serial.print("BLOCK!");
    }

  }

  else if(calLaserON){
    if (analogRead(calLaserDetPin) < 300){
      digitalWrite(blockPin,HIGH);
    }
    else{
      digitalWrite(blockPin,LOW);
    }
  }

  else if(storeLaserON){
    if (analogRead(storeLaserDetPin) < 200){ //BLOCKED
      digitalWrite(blockPin,HIGH);
    }
    else{
      digitalWrite(blockPin,LOW);
    }
    

  }
  else if(pusherDet){
    if (analogRead(tubeDetPin)>60){
      digitalWrite(blockPin,HIGH);
    }
    else{
      digitalWrite(blockPin,LOW);
    }
  }

  if (Serial.available() > 0) {
    String msg1 = Serial.readString();//.toInt(); // read the incoming byte:
    msg = int(msg1.toInt());
    Serial.print("MSG: ");
    Serial.println(msg1);
  }
  if(msgStrNum >= 4){
    //Serial.print("-");
    msg = buf[0]*1000+buf[1]*100+buf[2]*10+buf[3];
    msgStrNum = 0;
    Serial.print("MSG1: ");
    Serial.println(msg);
  }


  if(msg>=1000){
    if(msg <2000){
      firstServoPos = msg-1000;
    }
    else if(msg <3000){
      secondServoPos = msg-2000;
    }
    else if(msg <4000){
      thirdServoPos = msg-3000;
    }
    else if(msg <5000){
      fourthServoPos = msg-4000;
    }
    //motor
    else if(msg == 9100){
      Serial.println("MOTOR ON");
      //digitalWrite(vibmotor,LOW); //turn on
    }
    else if(msg == 9101){
      Serial.println("MOTOR OFF");
      //digitalWrite(vibmotor,HIGH); //turn off
    }

    //bead detect laser
    else if(msg == 9102){
      //Serial.println("BEAD LASER ON");
      digitalWrite(beadLaserGNDPin,LOW);
      beadLaserON = true;
    }
    else if(msg == 9103){
      //Serial.println("BEAD LASER OFF");
      digitalWrite(beadLaserGNDPin,HIGH);
      beadLaserON = false;
      digitalWrite(blockPin,LOW);
    }

    //store detect laser
    else if(msg == 9104){
      //Serial.println("LASER ON");
      digitalWrite(storeLaserGNDPin,LOW);
      storeLaserON = true;
    }
    else if(msg == 9105){
      //Serial.println("LASER OFF");
      digitalWrite(storeLaserGNDPin,HIGH);
      storeLaserON = false;
      digitalWrite(blockPin,LOW);
    }

    //cal detect laser
    else if(msg == 9106){
      //Serial.println("LASER ON");
      digitalWrite(calLaserGNDPin,LOW);
      calLaserON = true;
    }
    else if(msg == 9107){
      //Serial.println("LASER OFF");
      digitalWrite(calLaserGNDPin,HIGH);
      calLaserON = false;
      digitalWrite(blockPin,LOW);
    }

    else if(msg == 9108){
      pusherDet = true;
    }
    else if(msg == 9109){
      pusherDet = false;
    }

    msg = 0;
  }

  delay(10);
  distMoved = 0;
  if(firstServoPos != rfirstServoPos){
    distMoved= moveServo2(firstServo,rfirstServoPos,firstServoPos,firstServoVel);
    rfirstServoPos = rfirstServoPos + distMoved;
  }
  if(secondServoPos != rsecondServoPos){
    distMoved = moveServo2(secondServo,rsecondServoPos,secondServoPos,secondServoVel);
    rsecondServoPos = rsecondServoPos + distMoved;
  }
  if(thirdServoPos != rthirdServoPos){
    distMoved = moveServo2(thirdServo,rthirdServoPos,thirdServoPos,thirdServoVel);
    rthirdServoPos = rthirdServoPos + distMoved;
  }
  if(fourthServoPos != rfourthServoPos){
    distMoved = moveServo2(fourthServo,rfourthServoPos,fourthServoPos,fourthServoVel);
    rfourthServoPos = rfourthServoPos + distMoved;
  }
  if(distMoved == 0){
    digitalWrite(servoCompletePin,1);
  }
  else{
    digitalWrite(servoCompletePin,0);
  }
}



void sendAnswer() {
  Serial.print("send response");
  Wire.write("9");
}

void receiveEvent(int howMany)
{
  buf[msgStrNum] = Wire.read();    // receive byte as an integer
  msgStrNum += 1;
}
int moveServo2(Servo servo,int startPos,int endPos,int servoSpeed){
  int pos;
  int distanceLeft = abs(endPos-startPos);
  if(distanceLeft<servoSpeed){
    servoSpeed = distanceLeft;
  }
  if(endPos>startPos){
    servo.write(startPos+servoSpeed);  
    return servoSpeed ;
  }
  else{
    servo.write(startPos-servoSpeed); 
    return -servoSpeed;
  }
}

