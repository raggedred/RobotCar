#include <Wire.h>
#include <Servo.h>    //Standard Servo Library
#include <AFMotor.h>  //Adafruit Motor Shield Library

AF_DCMotor leftRear(3,MOTOR34_8KHZ);
AF_DCMotor rightRear(4,MOTOR34_8KHZ);

int speed = 255;//0 to 255
int speedSet = 0;
#define MAX_SPEED 255 // sets speed of DC  motors
boolean goesForward=false;
boolean eventRecieved = false;

Servo servo, servo2;   
#define UECHO A0 //ECHO pin is input
#define UTRIG A1 //TRIG pin is output

#define MAX_DISTANCE 100 
long distance = 0;
long distanceR = 0;
long distanceL = 0;

int i = 0, action, duration;

void setup()
{
  Serial.begin(9600);
  Serial.println("Robot Car Starting!");

  Wire.begin(9);   //Addreessed as I2C slave 9
  
  Wire.onReceive(receiveEvent);  //i2c Wire event reciever. When Master sends data, run these action.
  
  //Serial.println("Servo Settings!");
  servo2.attach(9);  //SERVO 2 on  XC4472 4ch Motor Controller, 74HC595 & L2930 Chipset
  servo.attach(10);  //SERVO 1 on  XC4472 4ch Motor Controller, 74HC595 & L2930 Chipset
  
  Serial.println("Ultrasonic Range Finder");
  
  pinMode(UECHO, INPUT);        //ECHO pin is input
  pinMode(UTRIG, OUTPUT);       //TRIG pin is output
    
  initialise();   //Run the Initialise function
  
  Serial.print("Ready: ");
}
 
void loop()
{

  long sonarDistance = sonarPing();  //Get the distance in millimeters
  
  //Serial.print("Distance Reading: ");
  //Serial.println(sonarDistance/10);
  
 if(eventRecieved){                 //If we received an Event take action.
      eventRecieved = false;        //Reset Event
      drive(action);                //Run The Action
      //Serial.print("Action: ");
      //Serial.println(action);
 }
}

void drive(int direction){

  switch (direction) {
        case 0:
          Serial.println("Stop: ");
          moveStop();
        break;
        
        case 1:
          Serial.println("Forward: ");
          moveForward();
        break;
        
        case 2:
          Serial.println("Backward: ");
          moveBackward();
        break;
        
        case 3:
          Serial.println("Turn Left: ");
          turnLeft();
        break;
        
        case 4:
          Serial.println("Turn Right: ");
          turnRight();
        break;
        
        case 5:
          Serial.println("Look Left: ");
          lookLeft();
        break;

        case 6:
          Serial.println("Look Right: ");
          lookRight();
        break;
        
        case 7:
          Serial.println("Look Centre: ");
          lookCenter();
        break;

  }
  
}

void receiveEvent(int howMany)
{

  action = Wire.read();    // receive byte as an integer

  eventRecieved = true;
}


long lookLeft()
{
   servo.write(0);      //Set Servo 1 to 0 Degrees
   delay(500);
   distanceR = sonarPing();
   delay(100);
   servo.write(90); 
   return distanceR;
}

int lookRight(){
    servo.write(180);    //Set Servo 1 to 180 Degrees
    delay(500);
    distanceL = sonarPing();
    delay(100);
    servo.write(90); 
    return distanceL;
    delay(100);
}

int lookCenter(){
    servo.write(90);     //Set Servo 1 to 90 Degrees
    delay(500);
    distance = sonarPing();
    return distance;
    delay(100);
}
int lookUp(){
    servo2.write(0);      //Set Servo 2 to 0 Degrees (Up)
    delay(500);
    distanceL = sonarPing();
    delay(100);
    servo2.write(90); 
    return distanceL;
    delay(100);
}

int lookDown(){
    servo2.write(180);     //Set Servo 2 to 180 Degrees  (Down).. Depends on which way you put your servo
    delay(500);
    distance = sonarPing();
    delay(100);
    servo2.write(90);      //back to centre position
    return distance;
    delay(100);
}

void moveStop() {
 rightRear.run(RELEASE);
 leftRear.run(RELEASE); 
} 


void moveForward() {
 if(!goesForward)
  {
    goesForward=true;
    rightRear.run(FORWARD);
    leftRear.run(FORWARD );     
   
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    leftRear.setSpeed(speed); 
    rightRear.setSpeed(speed);
    delay(5);
   }
  }
}

void moveBackward() {
    goesForward=false;
    leftRear.run(BACKWARD);
    rightRear.run(BACKWARD); 

    
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftRear.setSpeed(speed); 
    rightRear.setSpeed(speed);
    
    delay(5);
  }
}

void turnRight() {
    leftRear.setSpeed(speed); 
    rightRear.setSpeed(speed);
    rightRear.run(FORWARD);
    leftRear.run(BACKWARD);   
} 
 
void turnLeft() {
  rightRear.setSpeed(speed);
  rightRear.run(BACKWARD);
  leftRear.setSpeed(speed);
  leftRear.run(FORWARD);     
}

// -------------------------------------------------------------
// sonarPing function
//    This measures the distance (in milimeters) to the object infront of it
unsigned long sonarPing()
{
  //send a pulse and time how long it takes to come back.
  digitalWrite(UTRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(UTRIG, LOW);

  unsigned long duration = pulseIn(UECHO, HIGH);
  return duration * 0.17; //convert microseconds to millimeters; (speed of sound);
}

void initialise(){
 Serial.println("Initialising!");
 Serial.println("------------");

 Serial.println("Motor test!");
 Serial.println("Setting Speed: ");
 
 leftRear.setSpeed(255); 
 rightRear.setSpeed(255);
 
 Serial.println("------------");
 Serial.println("FORWARD");
 moveForward();
 Serial.println("------------");
 delay(1000);
 Serial.println("STOP");
 moveStop();
 Serial.println("------------");
 
 delay(1000);
 Serial.println("BACKWARD");
 moveBackward();
 Serial.println("------------");
 delay(1000);
 Serial.println("STOP");
 moveStop();
 Serial.println("------------");

 delay(1000);
 Serial.println("Right");
 turnRight();
 Serial.println("------------");
 delay(4000);
 Serial.println("STOP");
 moveStop();
 Serial.println("------------");

 delay(1000);
 Serial.println("BACKWARD");
 moveBackward();
 Serial.println("------------");
 delay(1000);
 Serial.println("STOP");
 moveStop();
 Serial.println("------------");
 
 delay(1000);
 Serial.println("Left");
 turnLeft();
 Serial.println("------------");
 delay(4000);
 Serial.println("STOP");
 moveStop();
 Serial.println("------------");
 
 delay(1000);
 Serial.println("BACKWARD");
 moveBackward();
 Serial.println("------------");
 delay(1000);
 Serial.println("STOP");
 moveStop();
 Serial.println("------------");
  
 delay(2000);
 Serial.println("Servo test!");
 Serial.println("------------");
 Serial.println("Center - 90*");
  
 lookRight();
 delay(1500);

 Serial.println("Left - 180*");
 lookLeft();
 delay(1500);
 
 Serial.println("Up - 180*");
 lookUp();
 delay(1500);
 
 Serial.println("Down - 90*");
 lookDown(); 
  delay(1500);
 Serial.println("Right - 0*");
 lookRight(); 
 delay(1500);
 
 Serial.println("------------"); 

}
